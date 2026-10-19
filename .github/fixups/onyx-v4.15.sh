#!/usr/bin/env bash
# Fixups for the v4.15 escalation. Idempotent: safe to re-run every dispatch.
#
# Every fix here is a "coupled pair" repair - one half came from upstream v4.15 and
# the other half is ours (vendor), and the two no longer agree.
set +e

echo "===== v4.15 fixups ====="

# ---------------------------------------------------------------------------
# Fix 1: scripts/mod/devicetable-offsets.c is UPSTREAM and emits
#        DEFINE(tb_service_id, ...) / SIZE_tb_service_id, but our (vendor)
#        include/linux/mod_devicetable.h never defined that struct.
#        Add upstream's definition to our header (purely additive).
# ---------------------------------------------------------------------------
if grep -q 'struct tb_service_id' include/linux/mod_devicetable.h; then
  echo "  [=] mod_devicetable.h already defines struct tb_service_id"
else
  echo "  [+] mod_devicetable.h: adding upstream tb_service_id definition"
  python3 - <<'PY'
p = 'include/linux/mod_devicetable.h'
s = open(p).read()
block = '''
/**
 * struct tb_service_id - Thunderbolt service identifiers
 * @match_flags: Flags used to match the structure
 * @protocol_key: Protocol key the service supports
 * @protocol_id: Protocol id the service supports
 * @protocol_version: Version of the protocol
 * @protocol_revision: Revision of the protocol software
 * @driver_data: Driver specific data
 *
 * Thunderbolt XDomain services are exposed as devices where each device
 * carries the protocol information the service supports. Thunderbolt
 * XDomain service drivers match against that information.
 */
struct tb_service_id {
\t__u32 match_flags;
\tchar protocol_key[8 + 1];
\t__u32 protocol_id;
\t__u32 protocol_version;
\t__u32 protocol_revision;
\tkernel_ulong_t driver_data;
};

#define TBSVC_MATCH_PROTOCOL_KEY\t0x0001
#define TBSVC_MATCH_PROTOCOL_ID\t\t0x0002
#define TBSVC_MATCH_PROTOCOL_VERSION\t0x0004
#define TBSVC_MATCH_PROTOCOL_REVISION\t0x0008

'''
i = s.rfind('#endif')          # the include-guard #endif is the last one
assert i > 0, 'no trailing #endif found'
open(p, 'w').write(s[:i] + block + s[i:])
print('    inserted %d bytes' % len(block))
PY
fi

# ---------------------------------------------------------------------------
# Fix 2: the arm64 rwlock cluster.
#        Our (vendor) arm64 tree does NOT use the generic qrwlock at all: our
#        arch/arm64/include/asm/spinlock_types.h defines
#              typedef struct { volatile unsigned int lock; } arch_rwlock_t;
#        and our arch/arm64/include/asm/spinlock.h implements rwlocks natively
#        on top of "->lock".
#        Upstream v4.15 replaced that spinlock_types.h with one that pulls in
#        <asm-generic/qrwlock_types.h> (arch_rwlock_t = struct qrwlock with
#        cnts/wait_lock), and because only upstream had touched that file it got
#        adopted -> "no member named 'lock' in 'struct qrwlock'".
#        Keep the whole cluster on our side so the pair agrees again.
# ---------------------------------------------------------------------------
for f in arch/arm64/include/asm/spinlock_types.h \
         include/asm-generic/qrwlock.h \
         include/asm-generic/qrwlock_types.h \
         kernel/locking/qrwlock.c ; do
  if ! git cat-file -e "origin/onyx:$f" 2>/dev/null; then
    echo "  [-] not in ours, skipped: $f"
  elif git checkout origin/onyx -- "$f" 2>/dev/null; then
    echo "  [+] reverted to ours: $f"
  else
    echo "  [!] could not revert $f"
  fi
done

# ---------------------------------------------------------------------------
# Fix 3: include/linux/jump_label_ratelimit.h is UPSTREAM and calls
#        STATIC_KEY_CHECK_USE(), which lives in upstream's jump_label.h.
#        Our (vendor) jump_label.h has no such macro, and jump_label.h itself
#        is OURS because the vendor modified it.  Keep our ratelimit header,
#        which matches our jump_label.h.
# ---------------------------------------------------------------------------
if git checkout origin/onyx -- include/linux/jump_label_ratelimit.h 2>/dev/null; then
  echo "  [+] reverted to ours: include/linux/jump_label_ratelimit.h"
else
  echo "  [!] could not revert include/linux/jump_label_ratelimit.h"
fi

# ---------------------------------------------------------------------------
# Fix 4: include/linux/blk-mq.h is UPSTREAM (v4.15) and uses the type
#        blk_mq_req_flags_t six times; upstream defines it in
#        include/linux/blk_types.h, which in our tree is OURS (the vendor had
#        touched that header), so the type is absent -> 1104 errors. Port the
#        definition across verbatim from upstream.
# ---------------------------------------------------------------------------
if grep -q 'blk_mq_req_flags_t' include/linux/blk_types.h; then
  echo "  [=] blk_types.h already has blk_mq_req_flags_t"
else
  echo "  [+] blk_types.h: porting blk_mq_req_flags_t from upstream"
  python3 - <<'PY'
import subprocess, os
up = os.environ.get('UPSTREAM_SHA')
if not up:
    up = subprocess.run(['git', 'rev-parse', 'FETCH_HEAD'],
                        capture_output=True, text=True).stdout.strip()
src = subprocess.check_output(['git', 'show', up + ':include/linux/blk_types.h']).decode()
lines = src.split('\n')
start = next((i for i, l in enumerate(lines)
              if l.startswith('typedef') and 'blk_mq_req_flags_t;' in l), None)
if start is None:
    raise SystemExit('blk_mq_req_flags_t typedef not found in upstream blk_types.h')
# Upstream has it as a lone typedef: take only the typedef statement itself.
end = start
while not lines[end].rstrip().endswith(';'):
    end += 1
block = '\n'.join(lines[start:end + 1]) + '\n'
p = 'include/linux/blk_types.h'
s = open(p).read()
i = s.rfind('#endif')
open(p, 'w').write(s[:i] + block + s[i:])
print('    ported %d bytes' % len(block))
PY
fi

echo "===== fixups done ====="
git status --short | head -30

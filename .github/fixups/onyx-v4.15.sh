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
# Fix 2: qrwlock cluster.
#        arch/arm64/include/asm/spinlock.h is OURS (vendor touched it) and
#        refers to qrwlock's member "->lock".  Upstream v4.15 renamed that
#        member to "wait_lock" in asm-generic/qrwlock_types.h, and that file
#        (plus qrwlock.h and qrwlock.c) were adopted from upstream because the
#        vendor had not touched THEM.  Result: "no member named 'lock'".
#        Keep the whole cluster on our side so the pair agrees again.
# ---------------------------------------------------------------------------
for f in include/asm-generic/qrwlock.h \
         include/asm-generic/qrwlock_types.h \
         kernel/locking/qrwlock.c ; do
  if git checkout origin/onyx -- "$f" 2>/dev/null; then
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

echo "===== fixups done ====="
git status --short | head -30

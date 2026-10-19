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

# ---------------------------------------------------------------------------
# Fix 5: adopted upstream files that our (vendor) code provably cannot build
#        against.  Each of these was established by blob hash + reading the
#        failing source line, NOT guessed:
#
#  include/linux/nfs_fs_sb.h    upstream v4.15 changed struct nfs_client's
#                               cl_count from atomic_t to refcount_t, while our
#                               fs/nfs/client.c still calls atomic_set() on it;
#                               atomic_set() expands to (v)->counter, and
#                               refcount_t has no 'counter' -> 23 errors.
#  include/linux/blk-mq.h       upstream changed blk_mq_run_hw_queue()'s
#                               signature; our block/blk-mq.{c,h} still use the
#                               old one -> "conflicting types".
#  include/linux/iomap.h        upstream v4.15 dropped struct iomap::blkno and
#                               IOMAP_NULL_BLOCK; our fs/iomap.c and fs/buffer.c
#                               still use them.
#  fs/btrfs/delayed-ref.h       upstream renamed BTRFS_DELAYED_REF members used
#                               by our fs/btrfs/{delayed-ref,extent-tree,backref,
#                               disk-io}.c.
#  include/scsi/scsi_devinfo.h  upstream turned BLIST_* enum constants into
#                               macros cast to blist_flags_t, which does not
#                               exist in our include/scsi/scsi_device.h
#                               (unsigned int sdev_bflags).
#  drivers/scsi/scsi_priv.h     upstream-only file; its structs use blist_flags_t.
#  include/linux/blk-cgroup.h   upstream-only; bio_blkcg() calls kthread_blkcg()
#                               which our include/linux/cgroup.h never declared.
#  include/net/pkt_cls.h        upstream-only; needs tc_setup_cb_t,
#                               tcf_chain_head_change_t and struct tcf_block::q
#                               which our net/sched core does not have.
#
#  include/linux/trace.h        upstream v4.15 added a third parameter to
#                               struct trace_export::write (ours is
#                               `void (*write)(const void *, unsigned int)`,
#                               upstream's `... (struct trace_export *, const void *,
#                               unsigned int)`); our kernel/trace/trace.c calls it
#                               with two arguments.
#  include/linux/fwnode.h       upstream changed struct fwnode_operations::get to
#                               return a struct fwnode_handle *; our
#                               drivers/of/property.c installs a void-returning
#                               of_fwnode_get.
#  fs/xfs/Makefile              the adopted Makefile builds xfs_iext_tree.o, a file
#                               that does not exist in our tree at all, and which
#                               implements upstream's new extent-list B-tree.  Our
#                               XFS is the earlier (self-consistent) generation, so
#                               the objects it needs are the ones our Makefile lists.
#
#  The remaining entries were not guessed either: they are exactly the files
#  clang itself names in a "note: previous definition is here" / "note: 'x'
#  declared here" / "note: expanded from macro" line that explains one of the
#  build errors (F-024).  Reverting them restores our (older) declaration so it
#  matches the vendor .c files underneath.  No driver, config option or source
#  file is removed by any of this -- only the generation of these declarations
#  is held back.
# ---------------------------------------------------------------------------
REVERT_TO_OURS="
include/linux/nfs_fs_sb.h
include/linux/blk-mq.h
include/linux/iomap.h
fs/btrfs/delayed-ref.h
fs/btrfs/backref.h
fs/btrfs/sysfs.h
include/scsi/scsi_devinfo.h
drivers/scsi/scsi_priv.h
include/linux/blk-cgroup.h
include/net/pkt_cls.h
include/linux/gpio/driver.h
include/linux/gpio/machine.h
include/linux/of_gpio.h
include/linux/irqdomain.h
include/linux/page-isolation.h
include/linux/rtnetlink.h
include/linux/iio/iio.h
fs/ecryptfs/ecryptfs_kernel.h
fs/xfs/libxfs/xfs_types.h
fs/xfs/libxfs/xfs_bmap.h
fs/xfs/libxfs/xfs_bmap_btree.h
fs/xfs/libxfs/xfs_ialloc.h
fs/xfs/libxfs/xfs_inode_fork.h
fs/xfs/xfs_inode.h
fs/xfs/xfs_bmap_util.h
drivers/mmc/core/host.h
drivers/of/of_private.h
net/bridge/br_private_tunnel.h
block/bfq-iosched.h
include/linux/configfs.h
include/net/netfilter/nf_conntrack_l4proto.h
arch/arm64/include/asm/irqflags.h
arch/arm64/include/asm/daifflags.h
include/linux/trace.h
include/linux/fwnode.h
fs/xfs/Makefile
fs/xfs/libxfs/xfs_bmap_btree.c
fs/xfs/xfs_buf.h
arch/arm64/lib/delay.c
mm/sparse-vmemmap.c
drivers/base/property.c
drivers/clk/clk-gpio.c
drivers/hwtracing/stm/policy.c
drivers/pinctrl/core.h
drivers/rtc/class.c
drivers/target/target_core_configfs.c
fs/btrfs/lzo.c
fs/btrfs/send.h
fs/btrfs/zlib.c
fs/btrfs/zstd.c
fs/configfs/item.c
fs/notify/group.c
fs/udf/dir.c
fs/xfs/libxfs/xfs_btree.c
fs/xfs/libxfs/xfs_btree.h
fs/xfs/libxfs/xfs_dir2.c
fs/xfs/libxfs/xfs_ialloc.c
fs/xfs/libxfs/xfs_inode_fork.c
fs/xfs/libxfs/xfs_log_format.h
fs/xfs/xfs_dir2_readdir.c
fs/xfs/xfs_error.h
fs/xfs/xfs_icache.h
fs/xfs/xfs_inode_item.h
fs/xfs/xfs_iomap.c
fs/xfs/xfs_itable.c
fs/xfs/xfs_itable.h
fs/xfs/xfs_linux.h
fs/xfs/xfs_symlink.c
include/linux/ioport.h
include/linux/pinctrl/consumer.h
include/linux/sched/cputime.h
include/net/netfilter/ipv4/nf_conntrack_ipv4.h
include/net/netfilter/ipv6/nf_conntrack_ipv6.h
include/trace/events/dma_fence.h
kernel/audit_tree.c
kernel/irq/autoprobe.c
kernel/irq/spurious.c
kernel/sched/wait_bit.c
mm/page_isolation.c
net/bridge/br_ioctl.c
net/bridge/br_netlink_tunnel.c
net/bridge/br_stp.c
net/bridge/br_stp_if.c
net/bridge/br_stp_timer.c
net/ipv4/netfilter/nf_conntrack_proto_icmp.c
net/ipv6/netfilter/nf_conntrack_proto_icmpv6.c
net/netfilter/nf_conntrack_proto_generic.c
net/netfilter/nf_conntrack_proto_gre.c
net/netfilter/nf_conntrack_proto_udp.c
net/sched/sch_htb.c
fs/xfs/libxfs/xfs_dir2.h
"
for f in $REVERT_TO_OURS; do
  if ! git cat-file -e "origin/onyx:$f" 2>/dev/null; then
    echo "  [-] not in ours, skipped: $f"
  elif git checkout origin/onyx -- "$f" 2>/dev/null; then
    echo "  [+] reverted to ours: $f"
  else
    echo "  [!] could not revert $f"
  fi
done

# ---------------------------------------------------------------------------
# Fix 6: additive port, for a place where the adopted upstream file is a NEW
#        feature rather than a conflicting rewrite.
#        Upstream v4.15 added the bridge ARP/ND proxy (net/bridge/br_arp_nd_proxy.c)
#        and the adopted net/bridge/Makefile builds it.  That file needs exactly one
#        thing our vendor tree lacks: the field it stores its sysfs setting in.
#            upstream net/bridge/br_private.h:407  ->  bool neigh_suppress_enabled;
#        We add just that field to our struct net_bridge instead of disabling the
#        feature.  It is appended at the END of the struct, so the offset of every
#        existing member is unchanged -- which is what makes this safe.
# ---------------------------------------------------------------------------
if [ -f net/bridge/br_private.h ]; then
  if grep -q 'neigh_suppress_enabled' net/bridge/br_private.h; then
    echo "  [=] br_private.h already has neigh_suppress_enabled"
  else
    echo "  [+] br_private.h: adding neigh_suppress_enabled to struct net_bridge"
    python3 - <<'PY'
p = 'net/bridge/br_private.h'
s = open(p).read()
start = s.index('struct net_bridge {')
end = s.index('\n};', start)
field = '\n\tbool\t\t\t\tneigh_suppress_enabled;'
s = s[:end] + field + s[end:]
open(p, 'w').write(s)
print('    inserted %d bytes at the end of struct net_bridge' % len(field))
PY
  fi
fi

echo "===== fixups done ====="
git status --short | head -30

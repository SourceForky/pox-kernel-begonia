#!/usr/bin/env bash
# Diagnostic-only fixup for onyx-v4.15.
# Makes NO changes; prints ground truth about the merged tree so the
# compile errors can be fixed precisely instead of guessed at.

set +e

echo "########## ATOMIC ##########"
echo "--- who defines __atomic_add_unless? ---"
grep -rn "__atomic_add_unless" arch/arm64/include/asm/ include/asm-generic/ include/linux/atomic.h 2>/dev/null | head -40

echo "--- atomic_ll_sc.h / atomic_lse.h present? ---"
ls -l arch/arm64/include/asm/atomic_ll_sc.h arch/arm64/include/asm/atomic_lse.h 2>&1

echo "--- add_unless symbols in arm64 atomic impl ---"
grep -rn "add_unless" arch/arm64/include/asm/atomic_ll_sc.h arch/arm64/include/asm/atomic_lse.h 2>/dev/null | head -40

echo "--- atomic64_inc_not_zero / atomic64_add_unless definitions ---"
grep -rn "atomic64_inc_not_zero\|atomic64_add_unless" include/linux/atomic.h include/asm-generic/atomic-long.h arch/arm64/include/asm/ 2>/dev/null | head -40

echo "--- atomic_fetch_add_unless (the newer name) ---"
grep -rn "atomic_fetch_add_unless\|__ll_sc_atomic_fetch_add_unless" arch/arm64/include/asm/ include/linux/atomic.h 2>/dev/null | head -20

echo "########## RWSEM ##########"
grep -rn "RWSEM_OWNER_UNKNOWN" include/ kernel/locking/ arch/arm64/ 2>/dev/null | head -20

echo "########## KPTI / cpucaps ##########"
grep -rn "ARM64_UNMAP_KERNEL_AT_EL0" arch/arm64/ 2>/dev/null | head -20
echo "--- CONFIG_UNMAP_KERNEL_AT_EL0 in Kconfig ---"
grep -rn "UNMAP_KERNEL_AT_EL0" arch/arm64/Kconfig 2>/dev/null | head

echo "########## SELINUX ##########"
ls -l include/linux/selinux.h 2>&1
grep -n "selinux.h" include/linux/cred.h 2>/dev/null
echo "--- who else includes linux/selinux.h ---"
grep -rln "include <linux/selinux.h>" --include=*.c --include=*.h . 2>/dev/null | head -20

echo "########## VERSION MARKERS (how new is this tree?) ##########"
for s in mmap_lock mmap_sem set_fs kmap_local_page kmap_atomic io_uring CLONE_PIDFD pidfd; do
  n=$(grep -rl "\b$s\b" --include=*.c --include=*.h kernel/ mm/ fs/ include/ 2>/dev/null | wc -l)
  echo "  $s : $n files"
done

echo "########## DTC yylloc ##########"
grep -n "yylloc" scripts/dtc/dtc-lexer.l scripts/dtc/dtc-parser.y 2>/dev/null | head

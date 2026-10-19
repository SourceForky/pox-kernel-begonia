### Escalation probe

- base branch : onyx
- base commit : 05d4e543c302
- base version: 4.14.357
- upstream    : https://github.com/torvalds/linux @ v4.15
- target      : onyx-v4.15
- strategy    : default / resolve=both-ours

- upstream sha: `f5b7ec87f701326704545ebfc8fe4a2aa42086aa`
- merge-base: `bebc6082da0a9f5d47a1ea2edc099bf671058bd4`
- commits vendor-side (ahead): 44288
- commits upstream-side (behind): 16223

## Merge result

- merge exit code: 1
- **conflicted files: 1047**

### merge output (tail)
```
Auto-merging tools/testing/selftests/memfd/memfd_test.c
Auto-merging tools/testing/selftests/net/rtnetlink.sh
Auto-merging tools/testing/selftests/powerpc/benchmarks/context_switch.c
Auto-merging tools/testing/selftests/powerpc/dscr/dscr_sysfs_test.c
Auto-merging tools/testing/selftests/rcutorture/bin/configinit.sh
Auto-merging tools/testing/selftests/rcutorture/bin/kvm.sh
Auto-merging tools/testing/selftests/vm/.gitignore
Auto-merging tools/testing/selftests/vm/Makefile
Auto-merging tools/testing/selftests/x86/Makefile
CONFLICT (content): Merge conflict in tools/testing/selftests/x86/Makefile
Auto-merging tools/testing/selftests/x86/entry_from_vm86.c
CONFLICT (content): Merge conflict in tools/testing/selftests/x86/entry_from_vm86.c
Auto-merging tools/testing/selftests/x86/protection_keys.c
CONFLICT (content): Merge conflict in tools/testing/selftests/x86/protection_keys.c
Auto-merging tools/testing/selftests/x86/test_vsyscall.c
CONFLICT (add/add): Merge conflict in tools/testing/selftests/x86/test_vsyscall.c
Auto-merging tools/usb/usbip/libsrc/vhci_driver.c
Auto-merging tools/vm/slabinfo-gnuplot.sh
Auto-merging tools/vm/slabinfo.c
Auto-merging virt/kvm/arm/arch_timer.c
CONFLICT (content): Merge conflict in virt/kvm/arm/arch_timer.c
Auto-merging virt/kvm/arm/arm.c
CONFLICT (content): Merge conflict in virt/kvm/arm/arm.c
Auto-merging virt/kvm/arm/hyp/vgic-v2-sr.c
Auto-merging virt/kvm/arm/hyp/vgic-v3-sr.c
Auto-merging virt/kvm/arm/mmio.c
Auto-merging virt/kvm/arm/mmu.c
Auto-merging virt/kvm/arm/vgic/vgic-init.c
Auto-merging virt/kvm/arm/vgic/vgic-its.c
CONFLICT (content): Merge conflict in virt/kvm/arm/vgic/vgic-its.c
Auto-merging virt/kvm/arm/vgic/vgic-mmio-v2.c
Auto-merging virt/kvm/arm/vgic/vgic-mmio.c
CONFLICT (content): Merge conflict in virt/kvm/arm/vgic/vgic-mmio.c
Auto-merging virt/kvm/arm/vgic/vgic-v2.c
Auto-merging virt/kvm/arm/vgic/vgic-v3.c
Auto-merging virt/kvm/arm/vgic/vgic.c
Auto-merging virt/kvm/arm/vgic/vgic.h
Auto-merging virt/kvm/kvm_main.c
CONFLICT (content): Merge conflict in virt/kvm/kvm_main.c
Automatic merge failed; fix conflicts and then commit the result.
```

### conflict count by top-level dir
```
     86 arch/x86
     67 drivers/net
     64 include/linux
     42 drivers/gpu
     40 arch/arm64
     38 drivers/usb
     38 arch/powerpc
     23 net/ipv4
     23 arch/s390
     17 kernel/bpf
     16 tools/testing
     16 net/sched
     16 fs/f2fs
     16 drivers/scsi
     16 drivers/media
     16 drivers/infiniband
     15 include/net
     14 net/ipv6
     12 drivers/mmc
      9 sound/soc
      9 net/core
      9 fs/ext4
      9 drivers/md
      9 drivers/iio
      9 arch/arm
      8 kernel/trace
      8 kernel/sched
      8 drivers/tty
      7 net/decnet
      7 fs/crypto
      7 fs/btrfs
      6 net/dsa
      6 include/uapi
      6 fs/nfs
      6 drivers/staging
      6 drivers/pci
      6 drivers/nvme
      6 drivers/lightnvm
      6 drivers/input
      6 drivers/char
```

### first 100 conflicted files
```
.gitignore
Documentation/ABI/testing/sysfs-devices-system-cpu
Documentation/ABI/testing/sysfs-fs-f2fs
Documentation/admin-guide/kernel-parameters.txt
Documentation/arm64/cpu-feature-registers.txt
Documentation/cgroup-v2.txt
Documentation/dev-tools/kcov.rst
Documentation/devicetree/bindings/mmc/mtk-sd.txt
Documentation/devicetree/bindings/soc/mediatek/pwrap.txt
Documentation/devicetree/bindings/usb/usb-device.txt
Documentation/filesystems/fscrypt.rst
Documentation/filesystems/index.rst
Documentation/printk-formats.txt
Documentation/x86/x86_64/mm.txt
MAINTAINERS
Makefile
arch/arc/include/asm/uaccess.h
arch/arm/Kconfig
arch/arm/boot/dts/Makefile
arch/arm/boot/dts/imx1.dtsi
arch/arm/boot/dts/logicpd-som-lv.dtsi
arch/arm/configs/multi_v7_defconfig
arch/arm/include/asm/assembler.h
arch/arm/include/asm/kvm_host.h
arch/arm/kernel/head-common.S
arch/arm/net/bpf_jit_32.c
arch/arm64/Kconfig
arch/arm64/Makefile
arch/arm64/boot/dts/Makefile
arch/arm64/boot/dts/amlogic/meson-gxl.dtsi
arch/arm64/boot/dts/marvell/armada-37xx.dtsi
arch/arm64/boot/dts/mediatek/Makefile
arch/arm64/boot/dts/mediatek/pca_pps_ta.dtsi
arch/arm64/configs/defconfig
arch/arm64/include/asm/assembler.h
arch/arm64/include/asm/barrier.h
arch/arm64/include/asm/cpucaps.h
arch/arm64/include/asm/cpufeature.h
arch/arm64/include/asm/cputype.h
arch/arm64/include/asm/efi.h
arch/arm64/include/asm/fixmap.h
arch/arm64/include/asm/kvm_host.h
arch/arm64/include/asm/mmu_context.h
arch/arm64/include/asm/pgtable.h
arch/arm64/include/asm/processor.h
arch/arm64/include/asm/sysreg.h
arch/arm64/include/asm/thread_info.h
arch/arm64/include/asm/topology.h
arch/arm64/include/asm/traps.h
arch/arm64/include/uapi/asm/hwcap.h
arch/arm64/include/uapi/asm/sigcontext.h
arch/arm64/kernel/Makefile
arch/arm64/kernel/cpufeature.c
arch/arm64/kernel/cpuinfo.c
arch/arm64/kernel/entry.S
arch/arm64/kernel/ftrace.c
arch/arm64/kernel/module.lds
arch/arm64/kernel/process.c
arch/arm64/kernel/ptrace.c
arch/arm64/kernel/smp.c
arch/arm64/kernel/traps.c
arch/arm64/kvm/handle_exit.c
arch/arm64/kvm/hyp/switch.c
arch/arm64/mm/context.c
arch/arm64/mm/fault.c
arch/arm64/mm/kasan_init.c
arch/mips/kernel/ptrace.c
arch/mips/lib/multi3.c
arch/parisc/include/asm/ldcw.h
arch/parisc/kernel/hpmc.S
arch/parisc/kernel/process.c
arch/powerpc/Kconfig
arch/powerpc/include/asm/cputable.h
arch/powerpc/include/asm/exception-64s.h
arch/powerpc/include/asm/feature-fixups.h
arch/powerpc/include/asm/hvcall.h
arch/powerpc/include/asm/paca.h
arch/powerpc/include/asm/page_64.h
arch/powerpc/include/asm/setup.h
arch/powerpc/include/asm/uaccess.h
arch/powerpc/kernel/asm-offsets.c
arch/powerpc/kernel/cpu_setup_power.S
arch/powerpc/kernel/dt_cpu_ftrs.c
arch/powerpc/kernel/exceptions-64s.S
arch/powerpc/kernel/idle_book3s.S
arch/powerpc/kernel/kprobes-ftrace.c
arch/powerpc/kernel/optprobes.c
arch/powerpc/kernel/prom.c
arch/powerpc/kernel/setup-common.c
arch/powerpc/kernel/setup_64.c
arch/powerpc/kernel/vmlinux.lds.S
arch/powerpc/kernel/watchdog.c
arch/powerpc/lib/code-patching.c
arch/powerpc/lib/feature-fixups.c
arch/powerpc/mm/hash_native_64.c
arch/powerpc/mm/hugetlbpage-radix.c
arch/powerpc/mm/mmap.c
arch/powerpc/mm/mmu_context_book3s64.c
arch/powerpc/mm/slice.c
arch/powerpc/mm/tlb-radix.c
```
resolved with: both-ours (kept=5966 dropped=56 restored_deleted=290)

## Fixups (.github/fixups/onyx-v4.15.sh)

```
  [+] reverted to ours: fs/xfs/xfs_itable.c
  [+] reverted to ours: fs/xfs/xfs_itable.h
  [+] reverted to ours: fs/xfs/xfs_linux.h
  [+] reverted to ours: fs/xfs/xfs_symlink.c
  [+] reverted to ours: include/linux/ioport.h
  [+] reverted to ours: include/linux/pinctrl/consumer.h
  [+] reverted to ours: include/linux/sched/cputime.h
  [+] reverted to ours: include/net/netfilter/ipv4/nf_conntrack_ipv4.h
  [+] reverted to ours: include/net/netfilter/ipv6/nf_conntrack_ipv6.h
  [+] reverted to ours: include/trace/events/dma_fence.h
  [+] reverted to ours: kernel/audit_tree.c
  [+] reverted to ours: kernel/irq/autoprobe.c
  [+] reverted to ours: kernel/irq/spurious.c
  [+] reverted to ours: kernel/sched/wait_bit.c
  [+] reverted to ours: mm/page_isolation.c
  [+] reverted to ours: net/bridge/br_ioctl.c
  [+] reverted to ours: net/bridge/br_netlink_tunnel.c
  [+] reverted to ours: net/bridge/br_stp.c
  [+] reverted to ours: net/bridge/br_stp_if.c
  [+] reverted to ours: net/bridge/br_stp_timer.c
  [+] reverted to ours: net/ipv4/netfilter/nf_conntrack_proto_icmp.c
  [+] reverted to ours: net/ipv6/netfilter/nf_conntrack_proto_icmpv6.c
  [+] reverted to ours: net/netfilter/nf_conntrack_proto_generic.c
  [+] reverted to ours: net/netfilter/nf_conntrack_proto_gre.c
  [+] reverted to ours: net/netfilter/nf_conntrack_proto_udp.c
  [+] reverted to ours: net/sched/sch_htb.c
  [+] reverted to ours: fs/xfs/libxfs/xfs_dir2.h
  [+] br_private.h: adding neigh_suppress_enabled to struct net_bridge
    inserted 33 bytes at the end of struct net_bridge
===== fixups done =====
M  arch/arm64/include/asm/irqflags.h
M  arch/arm64/include/asm/spinlock_types.h
M  arch/arm64/lib/delay.c
M  block/bfq-iosched.h
M  drivers/base/property.c
M  drivers/clk/clk-gpio.c
M  drivers/hwtracing/stm/policy.c
M  drivers/mmc/core/host.h
M  drivers/of/of_private.h
M  drivers/pinctrl/core.h
M  drivers/rtc/class.c
M  drivers/scsi/scsi_priv.h
M  drivers/target/target_core_configfs.c
M  fs/btrfs/backref.h
M  fs/btrfs/delayed-ref.h
M  fs/btrfs/lzo.c
M  fs/btrfs/send.h
M  fs/btrfs/sysfs.h
M  fs/btrfs/zlib.c
M  fs/btrfs/zstd.c
M  fs/configfs/item.c
M  fs/ecryptfs/ecryptfs_kernel.h
M  fs/notify/group.c
M  fs/udf/dir.c
M  fs/xfs/Makefile
M  fs/xfs/libxfs/xfs_bmap.h
M  fs/xfs/libxfs/xfs_bmap_btree.c
M  fs/xfs/libxfs/xfs_bmap_btree.h
M  fs/xfs/libxfs/xfs_btree.c
M  fs/xfs/libxfs/xfs_btree.h
```

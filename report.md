### Escalation probe

- base branch : onyx-v4.15
- base commit : 3d889e5ebaea
- base version: 4.14.357
- upstream    : https://github.com/torvalds/linux @ v4.16
- target      : onyx-v4.16
- strategy    : default / resolve=both-ours

- upstream sha: `3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d`
- align sha (v4.15): `f5b7ec87f701326704545ebfc8fe4a2aa42086aa`
- commits in this release: 14896
- merge-base: `d8a5b80568a9cb66810e75b182018e9edb68e8ff`
- commits vendor-side (ahead): 44293
- commits upstream-side (behind): 14896

## Merge result

- merge exit code: 1
- **conflicted files: 1479**

### merge output (tail)
```
Auto-merging tools/testing/selftests/vm/compaction_test.c
Auto-merging tools/testing/selftests/vm/run_vmtests
CONFLICT (content): Merge conflict in tools/testing/selftests/vm/run_vmtests
Auto-merging tools/testing/selftests/x86/Makefile
CONFLICT (content): Merge conflict in tools/testing/selftests/x86/Makefile
Auto-merging tools/testing/selftests/x86/mpx-mini-test.c
Auto-merging tools/testing/selftests/x86/protection_keys.c
CONFLICT (content): Merge conflict in tools/testing/selftests/x86/protection_keys.c
Auto-merging tools/testing/selftests/x86/ptrace_syscall.c
Auto-merging tools/testing/selftests/x86/test_vdso.c
CONFLICT (content): Merge conflict in tools/testing/selftests/x86/test_vdso.c
Auto-merging tools/testing/selftests/x86/test_vsyscall.c
Auto-merging tools/virtio/linux/kernel.h
Auto-merging tools/vm/Makefile
Auto-merging tools/vm/page-types.c
Auto-merging virt/kvm/arm/arch_timer.c
CONFLICT (content): Merge conflict in virt/kvm/arm/arch_timer.c
Auto-merging virt/kvm/arm/arm.c
CONFLICT (content): Merge conflict in virt/kvm/arm/arm.c
Auto-merging virt/kvm/arm/hyp/vgic-v2-sr.c
Auto-merging virt/kvm/arm/hyp/vgic-v3-sr.c
Auto-merging virt/kvm/arm/mmu.c
Auto-merging virt/kvm/arm/psci.c
CONFLICT (content): Merge conflict in virt/kvm/arm/psci.c
Auto-merging virt/kvm/arm/vgic/vgic-its.c
Auto-merging virt/kvm/arm/vgic/vgic-mmio.c
CONFLICT (content): Merge conflict in virt/kvm/arm/vgic/vgic-mmio.c
Auto-merging virt/kvm/arm/vgic/vgic-v2.c
CONFLICT (content): Merge conflict in virt/kvm/arm/vgic/vgic-v2.c
Auto-merging virt/kvm/arm/vgic/vgic-v3.c
CONFLICT (content): Merge conflict in virt/kvm/arm/vgic/vgic-v3.c
Auto-merging virt/kvm/arm/vgic/vgic.c
CONFLICT (content): Merge conflict in virt/kvm/arm/vgic/vgic.c
Auto-merging virt/kvm/arm/vgic/vgic.h
CONFLICT (content): Merge conflict in virt/kvm/arm/vgic/vgic.h
Auto-merging virt/kvm/eventfd.c
CONFLICT (content): Merge conflict in virt/kvm/eventfd.c
Auto-merging virt/kvm/kvm_main.c
CONFLICT (content): Merge conflict in virt/kvm/kvm_main.c
Automatic merge failed; fix conflicts and then commit the result.
```

### conflict count by top-level dir
```
    145 drivers/net
    116 drivers/gpu
     68 arch/arm64
     57 include/linux
     53 arch/x86
     43 drivers/infiniband
     37 drivers/usb
     36 tools/testing
     33 drivers/scsi
     30 arch/powerpc
     29 drivers/media
     26 net/sched
     25 arch/arm
     22 net/ipv4
     21 tools/perf
     21 drivers/staging
     19 include/net
     19 fs/btrfs
     18 net/ipv6
     17 arch/s390
     16 fs/f2fs
     15 net/netfilter
     14 kernel/bpf
     12 sound/soc
     12 net/core
     12 drivers/char
     11 net/batman-adv
     11 include/uapi
     11 fs/xfs
     11 drivers/pinctrl
     11 drivers/pci
     10 virt/kvm
     10 drivers/tty
     10 drivers/md
     10 drivers/hid
     10 drivers/crypto
      9 drivers/nvme
      9 drivers/mmc
      8 fs/overlayfs
      8 drivers/platform
```

### first 100 conflicted files
```
.gitignore
Documentation/ABI/testing/sysfs-ata
Documentation/ABI/testing/sysfs-fs-f2fs
Documentation/arm64/cpu-feature-registers.txt
Documentation/core-api/printk-formats.rst
Documentation/devicetree/bindings/eeprom/eeprom.txt
Documentation/devicetree/bindings/mmc/mtk-sd.txt
Documentation/devicetree/bindings/usb/dwc3.txt
Documentation/devicetree/bindings/usb/usb-device.txt
Documentation/devicetree/bindings/usb/usb-xhci.txt
Documentation/filesystems/overlayfs.txt
Documentation/media/uapi/rc/lirc-dev-intro.rst
Documentation/media/uapi/rc/lirc-get-rec-mode.rst
Documentation/networking/segmentation-offloads.txt
Documentation/networking/xfrm_device.txt
Documentation/sphinx/kerneldoc.py
Documentation/sphinx/kfigure.py
Documentation/sysctl/vm.txt
Documentation/virtual/kvm/api.txt
Documentation/virtual/kvm/cpuid.txt
MAINTAINERS
Makefile
arch/Kconfig
arch/alpha/kernel/osf_sys.c
arch/arm/Kconfig
arch/arm/Kconfig.debug
arch/arm/Makefile
arch/arm/boot/dts/Makefile
arch/arm/boot/dts/dra7.dtsi
arch/arm/boot/dts/exynos5422-odroidxu3-audio.dtsi
arch/arm/boot/dts/imx53.dtsi
arch/arm/boot/dts/imx6q-bx50v3.dtsi
arch/arm/boot/dts/imx6qdl.dtsi
arch/arm/boot/dts/imx6sl.dtsi
arch/arm/boot/dts/imx6sx.dtsi
arch/arm/boot/dts/imx6ul.dtsi
arch/arm/boot/dts/logicpd-som-lv.dtsi
arch/arm/boot/dts/logicpd-torpedo-som.dtsi
arch/arm/boot/dts/omap4.dtsi
arch/arm/boot/dts/spear1340.dtsi
arch/arm/boot/dts/spear13xx.dtsi
arch/arm/configs/multi_v7_defconfig
arch/arm/include/asm/dma-mapping.h
arch/arm/include/asm/kvm_host.h
arch/arm/include/asm/kvm_mmu.h
arch/arm/kernel/head-common.S
arch/arm/mach-davinci/board-omapl138-hawk.c
arch/arm/mm/Makefile
arch/arm/mm/proc-v7.S
arch/arm64/Kconfig
arch/arm64/Kconfig.platforms
arch/arm64/boot/dts/altera/socfpga_stratix10.dtsi
arch/arm64/boot/dts/amlogic/meson-gx.dtsi
arch/arm64/boot/dts/amlogic/meson-gxbb-nanopi-k2.dts
arch/arm64/boot/dts/amlogic/meson-gxbb-odroidc2.dts
arch/arm64/boot/dts/marvell/armada-7040-db.dts
arch/arm64/boot/dts/marvell/armada-8040-db.dts
arch/arm64/boot/dts/marvell/armada-8040-mcbin.dts
arch/arm64/boot/dts/marvell/armada-ap806.dtsi
arch/arm64/boot/dts/marvell/armada-cp110-master.dtsi
arch/arm64/boot/dts/marvell/armada-cp110-slave.dtsi
arch/arm64/boot/dts/mediatek/mt8173.dtsi
arch/arm64/boot/dts/nvidia/tegra186.dtsi
arch/arm64/boot/dts/qcom/apq8096-db820c.dtsi
arch/arm64/boot/dts/qcom/msm8996.dtsi
arch/arm64/boot/dts/socionext/uniphier-ld20.dtsi
arch/arm64/configs/defconfig
arch/arm64/include/asm/arm_dsu_pmu.h
arch/arm64/include/asm/asm-uaccess.h
arch/arm64/include/asm/assembler.h
arch/arm64/include/asm/barrier.h
arch/arm64/include/asm/cpucaps.h
arch/arm64/include/asm/cputype.h
arch/arm64/include/asm/dma-mapping.h
arch/arm64/include/asm/fixmap.h
arch/arm64/include/asm/kvm_arm.h
arch/arm64/include/asm/kvm_asm.h
arch/arm64/include/asm/kvm_host.h
arch/arm64/include/asm/kvm_mmu.h
arch/arm64/include/asm/memory.h
arch/arm64/include/asm/mmu.h
arch/arm64/include/asm/mmu_context.h
arch/arm64/include/asm/pgtable-hwdef.h
arch/arm64/include/asm/pgtable-prot.h
arch/arm64/include/asm/pgtable.h
arch/arm64/include/asm/processor.h
arch/arm64/include/asm/sections.h
arch/arm64/include/asm/sysreg.h
arch/arm64/include/asm/uaccess.h
arch/arm64/include/uapi/asm/hwcap.h
arch/arm64/kernel/Makefile
arch/arm64/kernel/asm-offsets.c
arch/arm64/kernel/bpi.S
arch/arm64/kernel/cpu_errata.c
arch/arm64/kernel/cpufeature.c
arch/arm64/kernel/cpuinfo.c
arch/arm64/kernel/entry.S
arch/arm64/kernel/fpsimd.c
arch/arm64/kernel/hibernate.c
arch/arm64/kernel/irq.c
```
resolved with: both-ours (kept=5466 dropped=22 restored_deleted=350)

## Fixups (.github/fixups/onyx-v4.16.sh)

```
===== v4.16 fixups =====
  [+] reverted to ours: scripts/kconfig/.gitignore
  [+] reverted to ours: scripts/kconfig/Makefile
  [+] reverted to ours: scripts/kconfig/conf.c
  [+] reverted to ours: scripts/kconfig/kconf_id.c
  [+] reverted to ours: scripts/kconfig/kxgettext.c
  [+] reverted to ours: scripts/kconfig/lkc.h
  [+] reverted to ours: scripts/kconfig/lkc_proto.h
  [+] reverted to ours: scripts/kconfig/lxdialog/check-lxdialog.sh
  [+] reverted to ours: scripts/kconfig/mconf.c
  [+] reverted to ours: scripts/kconfig/util.c
  [+] reverted to ours: scripts/kconfig/zconf.lex.c_shipped
  [+] reverted to ours: scripts/kconfig/zconf.tab.c_shipped
  [=] lkc_proto.h matches our symbol.c
  [=] lkc.h declares menu_end_entry (what our zconf.tab.c calls)
===== fixups done =====
 M fixup.log
 M markers.txt
 M report.md
M  scripts/kconfig/.gitignore
M  scripts/kconfig/Makefile
M  scripts/kconfig/conf.c
M  scripts/kconfig/kconf_id.c
M  scripts/kconfig/kxgettext.c
M  scripts/kconfig/lkc.h
M  scripts/kconfig/lkc_proto.h
M  scripts/kconfig/lxdialog/check-lxdialog.sh
M  scripts/kconfig/mconf.c
M  scripts/kconfig/util.c
```

## Build probe

- build exit code: 2
- error-line matches: 11
- object compiles seen: 4
- reached the link: 0
- image produced: 0

### head of build log (first 40 lines)
```
[1;32m[*] =================================================[0m
[1;32m[*] Building Pox[0m
[1;32m[*] Version:      1.0.0-Beta[0m
[1;32m[*] Version Name: Granite (Rock-Solid Stability Edition)[0m
[1;32m[*] Branch:       main[0m
[1;32m[*] Commit ID:    8b9bb78[0m
[1;32m[*] Device:       Redmi Note 8 Pro (begonia)[0m
[1;32m[*] Maintainer:   TXO R (Pox Project)[0m
[1;32m[*] Localversion: -Pox-1.0.0-Beta-Granite-8b9bb78[0m
[1;32m[*] Defconfig:    begonia_user_defconfig[0m
[1;32m[*] Output Dir:   /home/runner/work/pox-kernel-begonia/pox-kernel-begonia/build[0m
[1;32m[*] Object Dir:   /home/runner/work/pox-kernel-begonia/pox-kernel-begonia/build/out[0m
[1;32m[*] Jobs:         4[0m
[1;32m[*] Toolchain:    /home/runner/work/pox-kernel-begonia/pox-kernel-begonia/kerdevdep[0m
[1;32m[*] =================================================[0m
[1;32m[*] Using ccache (cache dir: /home/runner/work/pox-kernel-begonia/pox-kernel-begonia/build/.ccache)[0m
[1;32m[*] Configuring with begonia_user_defconfig ...[0m
make[1]: Entering directory '/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/build/out'
  HOSTCC  scripts/basic/fixdep
  GEN     ./Makefile
  HOSTCC  scripts/kconfig/conf.o
  SHIPPED scripts/kconfig/zconf.tab.c
  SHIPPED scripts/kconfig/zconf.lex.c
  HOSTCC  scripts/kconfig/zconf.tab.o
  HOSTLD  scripts/kconfig/conf
arch/arm64/configs/begonia_user_defconfig:5259:warning: override: reassigning to symbol REISERFS_FS
arch/arm64/configs/begonia_user_defconfig:5260:warning: override: reassigning to symbol JFS_FS
arch/arm64/configs/begonia_user_defconfig:5261:warning: override: reassigning to symbol XFS_FS
arch/arm64/configs/begonia_user_defconfig:5262:warning: override: reassigning to symbol BTRFS_FS
arch/arm64/configs/begonia_user_defconfig:5263:warning: override: reassigning to symbol NILFS2_FS
arch/arm64/configs/begonia_user_defconfig:5264:warning: override: reassigning to symbol F2FS_FS_COMPRESSION
arch/arm64/configs/begonia_user_defconfig:5265:warning: override: reassigning to symbol AUTOFS4_FS
arch/arm64/configs/begonia_user_defconfig:5266:warning: override: reassigning to symbol UDF_FS
arch/arm64/configs/begonia_user_defconfig:5267:warning: override: reassigning to symbol ADFS_FS
arch/arm64/configs/begonia_user_defconfig:5268:warning: override: reassigning to symbol AFFS_FS
arch/arm64/configs/begonia_user_defconfig:5269:warning: override: reassigning to symbol ECRYPT_FS
arch/arm64/configs/begonia_user_defconfig:5270:warning: override: reassigning to symbol HFS_FS
arch/arm64/configs/begonia_user_defconfig:5271:warning: override: reassigning to symbol HFSPLUS_FS
arch/arm64/configs/begonia_user_defconfig:5272:warning: override: reassigning to symbol BEFS_FS
arch/arm64/configs/begonia_user_defconfig:5273:warning: override: reassigning to symbol BFS_FS
```

### first 80 error lines
```
/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/scripts/mod/devicetable-offsets.c:206:2: error: invalid application of 'sizeof' to an incomplete type 'struct sdw_device_id'
/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/scripts/mod/devicetable-offsets.c:207:2: error: offsetof of incomplete type 'struct sdw_device_id'
/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/scripts/mod/devicetable-offsets.c:208:2: error: offsetof of incomplete type 'struct sdw_device_id'
make[3]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/scripts/Makefile.build:155: scripts/mod/devicetable-offsets.s] Error 1
/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/include/linux/mmzone.h:1169:23: error: use of undeclared identifier 'CONFIG_ARM64_PA_BITS'
/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/include/linux/mmzone.h:1255:32: error: use of undeclared identifier 'CONFIG_ARM64_PA_BITS'
make[2]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/./Kbuild:56: arch/arm64/kernel/asm-offsets.s] Error 1
make[1]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/Makefile:1286: prepare0] Error 2
make[2]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/scripts/Makefile.build:674: scripts/mod] Error 2
make[1]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/Makefile:615: scripts] Error 2
make: *** [Makefile:153: sub-make] Error 2
```

### distinct error messages (top 40)
```
      2 use of undeclared identifier 'CONFIG_ARM64_PA_BITS'
      2 offsetof of incomplete type 'struct sdw_device_id'
      1 invalid application of 'sizeof' to an incomplete type 'struct sdw_device_id'
```

### tail of build log
```
#define NR_MEM_SECTIONS         (1UL << SECTIONS_SHIFT)
                                        ^
/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/include/linux/page-flags-layout.h:31:25: note: expanded from macro 'SECTIONS_SHIFT'
#define SECTIONS_SHIFT  (MAX_PHYSMEM_BITS - SECTION_SIZE_BITS)
                         ^
/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/arch/arm64/include/asm/sparsemem.h:20:26: note: expanded from macro 'MAX_PHYSMEM_BITS'
#define MAX_PHYSMEM_BITS        CONFIG_ARM64_PA_BITS
                                ^
2 errors generated.
make[2]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/./Kbuild:56: arch/arm64/kernel/asm-offsets.s] Error 1
make[2]: Target '__build' not remade because of errors.
make[1]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/Makefile:1286: prepare0] Error 2
  HOSTCC  scripts/mod/sumversion.o
  HOSTCC  scripts/selinux/genheaders/genheaders
  HOSTCC  scripts/selinux/mdp/mdp
mali MTK evironment, building r25p0 DDK
mtk-Kbuild CONFIG_MALI_PLATFORM_THIRDPARTY := y
mtk-Kbuild CONFIG_MALI_PLATFORM_THIRDPARTY_NAME := "mt6785"
*GPU defined GPU BM QoS 2.0
mali MTK evironment, building r25p0 DDK
mtk-Kbuild CONFIG_MALI_PLATFORM_THIRDPARTY := y
mtk-Kbuild CONFIG_MALI_PLATFORM_THIRDPARTY_NAME := "mt6785"
*GPU defined GPU BM QoS 2.0
mali MTK evironment, building r25p0 DDK
mtk-Kbuild CONFIG_MALI_PLATFORM_THIRDPARTY := y
mtk-Kbuild CONFIG_MALI_PLATFORM_THIRDPARTY_NAME := "mt6785"
*GPU defined GPU BM QoS 2.0
  HOSTCC  scripts/kallsyms
  HOSTCC  scripts/pnmtologo
make[3]: Target '__build' not remade because of errors.
make[2]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/scripts/Makefile.build:674: scripts/mod] Error 2
  HOSTCC  scripts/sortextable
  HOSTCC  scripts/asn1_compiler
  HOSTCC  scripts/extract-cert
make[2]: Target '__build' not remade because of errors.
make[1]: *** [/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/Makefile:615: scripts] Error 2
make[1]: Target '_all' not remade because of errors.
make[1]: Leaving directory '/home/runner/work/pox-kernel-begonia/pox-kernel-begonia/build/out'
make: *** [Makefile:153: sub-make] Error 2
make: Target '_all' not remade because of errors.
```

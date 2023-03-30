# Lab 1

## Linux kernel compilation (Linux 编译)

### Configuration for a small kernel

To obtain a small kernel, one would run these commands repeatedly. They may also need to check whether the kernel can run correctly, since simply excluding all features will end up with a kernel panic.

```sh
make distclean
make defconfig
make menuconfig
time make -j$(nproc)
du -b arch/x86/boot/bzImage
```

My routine of shrinking the kernel size is listed below. Each step is based on the previous step.

```text
(Raw)
1326.18s user 93.82s system 362% cpu 6:31.36 total
12290112

(Using XZ compression mode)
General Setup --->
  Kernel compression mode (XZ)
1375.75s user 95.69s system 360% cpu 6:47.67 total
8927520

(Using -Os compilation)
General Setup --->
  Compiler otimization level (Optimize for size (-Os))
1234.59s user 88.31s system 359% cpu 6:08.13 total
7919712

(Excluding all features (except for 64-bit kernel) on the main menu)
[ ] Mitigations for speculative execution vulnerabilities
[ ] Virtualization
[ ] Enable loadable module support
[ ] Networking support
793.81s user 57.14s system 360% cpu 3:56.17 total
4799968

(Excluding some other inessential features, such as all features in "Processor type and features")
658.42s user 43.45s system 360% cpu 3:14.61 total
3816800
```

The [.config](.config) file stores my configuration. To use my config, load it by the `make menuconfig` UI, or use `make oldconfig` command.

Typically, using XZ compression mode can decrease kernel size but increase compilation time; using -Os compilation decrease kernel size and decrease compilation time; also, excluding features can both decrease size and decrease time.

> Reproducibility notes: Linux kernel version [6.2.8](https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.2.8.tar.xz), gcc version [9.4.0](https://packages.ubuntu.com/search?keywords=gcc-9) (Ubuntu 9.4.0-1ubuntu1~16.04).

---

Furthermore, there is a command, `make tinyconfig`, that can create the config for the minimal kernel. (see `make help`)

```sh
make distclean
make tinyconfig
make menuconfig
make -j$(nproc)
```

After running these commands, we can get the bzImage file of about 488 KiB.

> However, this kernel lacks the basic support for TTY, so it outputs nothing in QEMU emulation. In addition, we need to include some features to support initramfs and ELF binaries if we want to start up the shell. Nonetheless, the kernel size is sitll much smaller than 1 MiB. Check [this tutorial](https://weeraman.com/building-a-tiny-linux-kernel-8c07579ae79d) and [this how-to](https://github.com/geirha/shbot-initramfs/blob/master/kernel-howto.md) for detailed information.

### Cross-compilation

#### 1. Preparation

Install GNU C compiler for the armel architecture:

```sh
sudo apt install gcc-arm-linux-gnueabi 
```

Download the source of the latest stable version of busybox:

```sh
wget https://busybox.net/downloads/busybox-1.33.2.tar.bz2
```

Install QEMU ARM and AArch64 emulator:

```sh
sudo apt install qemu-system-arm
```

#### 2. Cross-compile Linux kernel

Go to the directory of the Linux kernel, which contains a `Makefile`, and run the following commands:

```sh
make CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm vexpress_defconfig 
make CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm -j$(nproc)
```

These commands will generate 2 files we will use later. One is the kernel image `arch/arm/boot/zImage`, another is the device tree binary `arch/arm/boot/dts/vexpress-v2p-ca9.dtb`.

> Note: vexpress is the kernel driver for ARM Ltd. Versatile Express platform. In QEMU, we will emulate the vexpress-a9 development board, which models the combination of the Versatile Express motherboard and the CoreTile Express A9x4 daughterboard.

#### 3. Cross-compile BusyBox

BusyBox is a software suite that provides several stripped-down Unix tools and utilities in a single executable file. It includes many common Unix tools, such as grep, ls and cat.

Extract files of BusyBox source:

```sh
tar -xf busybox-1.33.2.tar.bz2
```

Then go to `busybox-1.33.2` directory, run:

```sh
make CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm menuconfig
```

Include the following feature:

```text
Settings --->
  --- Build Options
  [*] Build static binary (no shared libs)
```

Cross-compile BusyBox:

```sh
make CROSS_COMPILE=arm-linux-gnueabi- ARCH=arm install -j$(nproc)
```

A `_install` directory will be created after the compilation.

#### 4. Build root filesystem (initramfs)

Go to `_install` directory, run:

```sh
mkdir -p etc/init.d/ dev mnt
```

Go to `_install/etc/init.d/` directory, create a file named `rcS`:

```sh
mkdir -p /proc
mkdir -p /tmp
mkdir -p /sys
mkdir -p /mnt
/bin/mount -a
mkdir -p /dev/pts
mount -t devpts devpts /dev/pts
```

Then run `chmod +x rcS`.

Go to `_install/etc` directory, create a file named `fstab`:

```sh
proc /proc proc defaults 0 0
tmpfs /tmp tmpfs defaults 0 0
sysfs /sys sysfs defaults 0 0
tmpfs /dev tmpfs defaults 0 0
```

And a file named `inittab`:

```sh
::sysinit:/etc/init.d/rcS
::respawn:-/bin/sh
::askfirst:-/bin/sh
::ctrlaltdel:/bin/umount -a -r
```

Go to `_install/dev` directory, run the following commands to create filesystem nodes:

```sh
sudo mknod console c 5 1
sudo mknod null c 1 3
```

Finally, we pack all files in `_install/` directory to a compressed cpio file. Go to `_install/` directory and run:

```sh
find . | cpio --quiet -H newc -o | gzip -9 -n > ../initrd.cpio.gz
```

#### 5. QEMU emulation

Run the command:

```sh
qemu-system-arm -M vexpress-a9 \
                -kernel path-to-zImage \
                -initrd path-to-initrd.cpio.gz \
                -dtb path-to-vexpress-v2p-ca9.dtb \
                -append "rdinit=/linuxrc console=ttyAMA0" \
                -nographic
```

The emulation is success when the system prompts `Please press Enter to activate this console.` (The boot may take some time.)

Press Enter, and run `cat /proc/cpuinfo` to check the architecture of the kernel. It should output something similar to the contents below:

```sh
processor       : 0
model name      : ARMv7 Processor rev 0 (v7l)
BogoMIPS        : 1626.93
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpd32
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xc09
CPU revision    : 0

Hardware        : ARM-Versatile Express
Revision        : 0000
Serial          : 0000000000000000
```

## Build initial RAM disk (创建初始内存盘)

### How to make kernel panic?

If the init process is terminated, there will be this message:

`Kernel panic - not syncing - attempted to kill init!`

A simple C program that can cause a kernel panic is shown below.

```C
int main() {
  return 0;
}
```

### Reason of kernel panic

`Kernel panic - not syncing - attempted to kill init!` can be divided into 3 parts:

- `kernel panic` is an unrecoverable error condition in, or detected by, the Linux kernel. It represents a situation where Linux literally finds it *impossible* to continue running, and stops dead in its tracks. Linux prints messages on the console and halts.
- `not syncing` means that the device buffers have not been flushed to the actual devices. It can prevent damage to the data.
- `attempted to kill init` means that the init process, which is (typically) process #1 in Linux systems, has died. Init is a daemon process that continues running until the system is shut down. It is the direct or indirect ancestor of all other processes.

To conclude, Linux cannot run without the init process, so terminating it gives rise to a kernel panic.

<<<<<<< HEAD
# hailo-10h.SlackBuild

**SAIRPi Project** — Slackware AI on Raspberry Pi  
https://sairpi.penthux.net

Bash build system that produces a Slackware AArch64 package set with full support for the Hailo-10H M.2 AI Accelerator Module on a Raspberry Pi 5. This is the first known build of HailoRT for Slackware AArch64 Linux.

---

## Hardware

- Raspberry Pi 5
- Hailo-10H M.2 AI Accelerator Module

---

## What it builds

| Package | Description |
|---|---|
| `kernel_rpi5` | RPi Linux kernel (Image.gz + DTBs) |
| `kernel-modules-rpi5` | Kernel modules |
| `kernel-headers-rpi5` | Kernel headers |
| `rpi5-boot-firmware` | RPi boot firmware blobs + config.txt.new |
| `hailort_rpi5` | HailoRT runtime library (libhailort, hailortcli) |
| `hailort-pcie-drv-rpi5` | HailoRT PCIe driver (hailo1x_pci.ko) |
| `hailort-nnc-drv-rpi5` | HailoRT integrated NNC driver (hailo_integrated_nnc.ko) |
| `hailort-10h-firmware` | Hailo-10H device firmware |

Packages follow the naming convention:
```
<name>-<version>-<arch>-<build>_slack<slackvers>_<date>_sai.txz
```
Each package has a corresponding `.md5` checksum file. On completion, all built packages are copied to `/tmp/Hailo-10H_sbopkg_YYYYMMDD-HHMMSS/` on the host.

---

## Build overview

Two-stage process:

**Stage 1** runs on the RPi 5 host. Downloads all source (RPi Linux kernel, boot firmware, HailoRT, HailoRT drivers), downloads and installs Slackware AArch64 packages into a chroot at `/tmp/hailo-chroot`, then invokes Stage 2.

**Stage 2** runs inside the chroot. Builds cmake 3.31.12 from source (required — HailoRT is incompatible with cmake 4.x), builds the RPi Linux kernel, HailoRT runtime, PCIe and NNC drivers, packages the boot firmware, and downloads the Hailo-10H device firmware from Hailo's AWS S3.

---

## Usage

Must be run as **root** from within the `_hailo-10h.SlackBuild` directory:

```bash
cd _hailo-10h.SlackBuild
./hailo-10h.SlackBuild
```

You will be prompted:

```
[Y]  Start a clean build
[K]  Keep existing data and continue
Any other key to exit
```

---

## Configuration

Edit the user section at the top of `.settings.inc` before building:

| Variable | Default | Purpose |
|---|---|---|
| `RPIVERS` | `rpi5` | Target Raspberry Pi version |
| `SLACKVERS` | `current` | Slackware AArch64 version |
| `RPI_KERNEL_BRANCH` | `rpi-6.18.y` | RPi Linux kernel Git branch |
| `RPI_BOOTFW_BRANCH` | `next` | Raspberry Pi boot firmware Git branch |
| `HAILORT_DRIVERS_BRANCH` | `master` | Hailo drivers Git branch |
| `BUILD` | `1` | Package build number |
| `LOCALSERVER` | `192.168.1.242` | Local Slackware mirror IP |

Note the `EDIT ANYTHING BELOW THIS LINE AT YOUR OWN RISK` line marker.

---

## Notes

- cmake 3.31.12 is built from source during Stage 2. Slackware AArch64 ships cmake 4.x which is incompatible with HailoRT. The legacy cmake installs to `/usr/local/cmake-3.31/` alongside the system cmake — it does not replace it.
- Two patches are applied automatically to the HailoRT source: one fixes the protobuf `lib64` cmake path on AArch64, the other replaces `del_timer_sync()` with `timer_delete_sync()` for Linux 6.15+.
- The `config.txt.new` produced by the boot firmware package includes `dtparam=nvme` and `dtoverlay=pciex1-compat-pi5,no-mip` under `[pi5]` — both required for MSI to work through the PCIe switch on the Raspberry Pi 5.

---

## License

MIT — see [LICENSE](LICENSE)

---
## Credits
=======
# Hailo-10H SlackBuild

Slackware AArch64 Linux build scripts for HailoRT software and drivers supporting Hailo-10H M.2 AI Accelerator Module on a Raspberry Pi 5.

Build scripts and process are currently being revised for public release. They will be uploaded in the fullness of time.

## Credits

[SAIRPi Project](https://sairpi.penthux.net)

Hailo-10H M.2 AI Accelerator Module - [Hailo Technologies Ltd.](https://hailo.ai/products/ai-accelerators/hailo-10h-m-2-ai-acceleration-module/)
>>>>>>> 9ee50a77d05fa8ec5f2c5bea11f4cb83ac162fdc

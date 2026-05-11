# hailo-10h.SlackBuild

**SAIRPi Project** - Slackware AI on Raspberry Pi  
https://sairpi.penthux.net

Bash script build process that produces a Slackware AArch64 package set with full support for the Hailo-10H M.2 AI Accelerator Module on a Raspberry Pi 5. 

---

## Build overview

This is the first known build of HailoRT for Slackware AArch64 Linux.

It involves a two-stage build process:

**Stage 1** runs on the Raspberry Pi 5 host. Downloads all source (Raspberry Pi Linux kernel, boot firmware, HailoRT, HailoRT drivers), downloads and installs Slackware AArch64 packages into a chroot at `/tmp/hailo-chroot`, then invokes Stage 2.

**Stage 2** runs inside the chroot. Builds cmake 3.31.12 from source (required — HailoRT is incompatible with cmake 4.x), builds the Raspberry Pi Linux kernel, HailoRT runtime, PCIe and NNC drivers, packages the boot firmware, and downloads the Hailo-10H device firmware from Hailo's AWS S3.

- Both Hailo 10H SlackBuild stages use a `/tmp/SBo` directory on the host system and in the chroot (where applicable) for storing files and source data. 

---

## Usage

You must have git installed on your Slackware AArch64 Linux system with Internet access in order to clone this repository.

Download this **hailo-10h-slackbuild** repository by cloning it:

```bash
  git clone https://github.com/SAIRPi/hailo-10h-slackbuild
```
Must be run as **root** from within the `_hailo-10h.SlackBuild` directory:

```bash
  cd _hailo-10h.SlackBuild
  ./hailo-10h.SlackBuild
```

You will be prompted to:

```
[Y]  Start a clean build
[K]  Keep existing data and continue
Any other key to exit
```

---

## Operating System

- [Slackware](http://www.slackware.com/) 15.0 aarch64 -current (Slackware 15.0+)

```bash
root@slackware:~# cat /etc/slackware-version
Slackware 15.0+
root@slackware:~# cat /etc/os-release
PRETTY_NAME="Slackware 15.0 aarch64 (post 15.0 -current)"
```

---

## Hardware

- [Raspberry Pi 5](https://www.raspberrypi.com/products/raspberry-pi-5/)
- [Hailo-10H M.2 AI Accelerator Module](https://hailo.ai/products/ai-accelerators/hailo-10h-m-2-ai-acceleration-module/)

---

## Packages this hailo-10h.SlackBuild creates

| Package | Description |
|---|---|
| `kernel_rpi5` | Raspberry Pi Linux kernel (Image.gz + DTBs) |
| `kernel-modules-rpi5` | Kernel modules |
| `kernel-headers-rpi5` | Kernel headers |
| `rpi5-boot-firmware` | Raspberry Pi boot firmware blobs with config.txt.new |
| `hailort_rpi5` | HailoRT runtime library (libhailort, hailortcli) |
| `hailort-pcie-drv-rpi5` | HailoRT PCIe driver (hailo1x_pci.ko) |
| `hailort-nnc-drv-rpi5` | HailoRT integrated NNC driver (hailo_integrated_nnc.ko) |
| `hailort-10h-firmware` | Hailo-10H device firmware |

Packages follow the Slackware package file naming convention with an additional custom tag suffix:
```
<name>-<version>-<arch>-<build>_<tag>.txz
```
Where <tag> = slack<slackware_environment>_<release_date>_sai
* <slackware_environment> = slackcurrent
* <release_date> = $(date '+%d%b%y')
* sai = SAIRPi Project ID tag (3 characters) for Slackware packages created by the project

Each package has a corresponding `.md5` checksum file. On completion, all built packages are copied to `/tmp/Hailo-10H_sbopkg_YYYYMMDD-HHMMSS/` directory on the host system.

---

## Configuration

Edit the user section at the top of `.settings.inc` before building:

| Variable | Default | Purpose |
|---|---|---|
| `RPIVERS` | `rpi5` | Target Raspberry Pi version |
| `SLACKVERS` | `current` | Slackware AArch64 version |
| `RPI_KERNEL_BRANCH` | `rpi-6.18.y` | Raspberry Pi Linux kernel Git branch |
| `RPI_BOOTFW_BRANCH` | `next` | Raspberry Pi boot firmware Git branch |
| `HAILORT_DRIVERS_BRANCH` | `master` | Hailo drivers Git branch |
| `BUILD` | `1` | Package build number |
| `LOCALSERVER` | `192.168.10.70` | Local Slackware mirror IP |

- `LOCALSERVER` setting is for when there's a local network Slackware mirror repository available. It's not absolutely necessary in order to use these build scripts, but it's easier and much more convenient because you're not relying on Internet speeds or sharing bandwidth with other users.

Note the `EDIT ANYTHING BELOW THIS LINE AT YOUR OWN RISK` line marker.

---

## Build Notes

- cmake 3.31.12 is built from source during Stage 2. Slackware AArch64 ships cmake 4.x which is incompatible with HailoRT. The legacy cmake installs to `/usr/local/cmake-3.31/` alongside the system cmake — it does not replace it.
- Patches are applied automatically to the HailoRT source: one fixes the protobuf `lib64` cmake path on AArch64, the other replaces `del_timer_sync()` with `timer_delete_sync()` for Linux 6.15+.
- The `config.txt.new` produced by the boot firmware package includes `dtparam=nvme` and `dtoverlay=pciex1-compat-pi5,no-mip` under `[pi5]` — both required for MSI to work through the PCIe switch on the Raspberry Pi 5.

**NB:** MSI (Message Signalled Interrupts) is the interrupt mechanism the Hailo PCIe driver uses. Without those two dtoverlay lines in `config.txt` the driver fails to allocate an MSI vector and the device will not be detected.

---

## License

Released under the [MIT License](LICENSE)

---

## Credits

[SAIRPi Project](https://sairpi.penthux.net)

Hailo-10H M.2 AI Accelerator Module - [Hailo Technologies Ltd.](https://hailo.ai/products/ai-accelerators/hailo-10h-m-2-ai-acceleration-module/)
HailoRT runtime library and driver software - [Hailo Technologies Ltd.](https://github.com/hailo-ai/)

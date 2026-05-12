# Hailo-10H SlackBuild

**SAIRPi Project** - Slackware AI on Raspberry Pi - https://sairpi.penthux.net

Bash script build process that creates Slackware AArch64 packages with full support for the Hailo-10H M.2 AI Accelerator Module on a Raspberry Pi 5. 

---

## Build overview

This is the first known successful build of HailoRT for Slackware AArch64 Linux. 

Once each run of the build process is executed it requires an initial input selection of `clean start` or `use existing data` and is fully automated thereafter.

It involves a two-stage build process:

**Stage 1** runs on the Raspberry Pi 5 host. It downloads all sources (Raspberry Pi Linux kernel, boot firmware, HailoRT, HailoRT drivers, HailoRT firmware), downloads and installs Slackware AArch64 packages and [SARPi Project](https://sarpi.penthux.net/index.php?p=downloads) board support packages for the Raspberry Pi 5, into a created chroot directory: `/tmp/hailo-chroot`, then invokes Stage 2.

**Stage 2** runs inside the chroot. It builds cmake 3.31.12 from source and installs it into `/usr/bin` (which is required because HailoRT source is currently incompatible with cmake 4.x), builds the Raspberry Pi Linux kernel 6.18.x with headers and modules, HailoRT runtime software, HailoRT PCIe and NNC drivers, and packages the Raspberry Pi boot firmware and Hailo-10H device firmware. Then it creates Slackware packages from the compiled sources and downloaded firmware.

- Included in stage 1 and 2 are .settings.inc files which contain settings that can be modified by the user. See: **Configuration** in this README.
- SARPi board support packages are required to build legacy cmake 3.32.12 source in the chroot environment.
- Both Hailo 10H SlackBuild stages use a `/tmp/SBo` directory on the host system and in the chroot (where applicable) for storing files and source data. 

---

## Usage

You must have git installed on your Slackware AArch64 Linux system with Internet access in order to clone this repository.

To download this **hailo-10h-slackbuild** repository, use `git clone`. For example:

```bash
  cd /tmp
  git clone https://github.com/SAIRPi/hailo-10h-slackbuild _hailo-10h.SlackBuild
```  
Make all .SlackBuild and .sh files executable:

```bash
  cd _hailo-10h.SlackBuild
  find . \( -name "*.SlackBuild" -o -name "*.sh" \) -exec chmod +x {} +
```  

Run the build process as **root** user from within the download directory:

```bash
  ./hailo-10h.SlackBuild
```

You will be prompted:

```bash
 To start a clean hailo-10h.SlackBuild press [Y] ...
 To start a hailo-10h.SlackBuild using existing data press [K] ...
 Or press any other key to EXIT ...
```

- To start a clean build (i.e. the first time it's run or to download updated sources) then [`Y`] should be selected. 
- On subsequent build runs, once the source(s) have been compiled, to build with existing data [`K`] should be selected.
- The build process will create a chroot directory, mount it, and unmount it automatically once the process has completed. Or if/when it's exited with [`CTRL+C`].

---

## Configuration

It's possible to edit the user section at the top of `.settings.inc` before building:

| Variable | Default | Purpose |
|---|---|---|
| `RPIVERS` | `rpi5` | Target Raspberry Pi version |
| `SLACKVERS` | `current` | Slackware AArch64 version |
| `RPI_KERNEL_BRANCH` | `rpi-6.18.y` | Raspberry Pi Linux kernel Git branch |
| `RPI_BOOTFW_BRANCH` | `next` | Raspberry Pi boot firmware Git branch |
| `HAILORT_DRIVERS_BRANCH` | `master` | HailoRT and drivers Git branch |
| `BUILD` | `1` | Package build number |
| `LOCALSERVER` | `192.168.10.70` | Local Slackware mirror server IP |

- `LOCALSERVER` setting is for when there's a local network Slackware mirror repository available. It's not absolutely necessary in order to use these build scripts, but it's easier and much more convenient because you're not relying on Internet speeds or sharing bandwidth with other users.

Note the `EDIT ANYTHING BELOW THIS LINE AT YOUR OWN RISK` comment in the `.settings.inc` file(s). You can, of course, make whatever modifications suit your requirements to any of the build scripts or support files from this repository. 

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
<name>-<version>-<arch>-<build>_<-tag->.txz
```
Where <-tag-> = slack<slackware_environment>_<release_date>_sai
* <slackware_environment> = current
* <release_date> = $(date '+%d%b%y')
* sai = SAIRPi Project ID tag (3 characters) for Slackware packages created by the project

Each package has a corresponding `.md5` checksum file. On completion, all built packages are copied to `/tmp/Hailo-10H_sbopkg_YYYYMMDD-HHMMSS/` directory on the host system for convenience and manageability.

---

## Directory Tree
```bash
user@slackware:/tmp/_hailo-10h.SlackBuild# tree -a
.
├── .functions.inc
├── .settings.inc
├── BuildLogs
├── Dev-Docs
│   ├── HAILO_AFTERBUILD_TEST.txt
│   ├── HAILO_BUILD_CHANGES.txt
│   ├── HAILO_BUILD_DEV_DOC.txt
│   ├── HAILO_TODO_DEV_DOC.txt
│   └── SAIRPi_JAFFAWORKS_R&D_DOC.txt
├── LICENSE
├── README.md
├── _HAILO.SlackBuild
│   ├── .functions.inc
│   ├── .settings.inc
│   ├── build-legacy-cmake-source.sh
│   ├── hailo.SlackBuild
│   ├── hailort-10h-firmware
│   │   ├── doinst.sh
│   │   ├── hailort-10h-firmware.SlackBuild
│   │   └── slack-desc
│   ├── hailort-nnc-drv-rpi5
│   │   ├── doinst.sh
│   │   ├── hailort-nnc-drv-rpi5.SlackBuild
│   │   ├── patch
│   │   │   └── hailort-drivers-del-timer-sync-kernel-6.15.patch
│   │   └── slack-desc
│   ├── hailort-pcie-drv-rpi5
│   │   ├── doinst.sh
│   │   ├── hailort-pcie-drv-rpi5.SlackBuild
│   │   ├── patch
│   │   │   └── hailort-drivers-del-timer-sync-kernel-6.15.patch
│   │   └── slack-desc
│   ├── hailort_rpi5
│   │   ├── doinst.sh
│   │   ├── hailort_rpi5.SlackBuild
│   │   ├── patch
│   │   │   └── hailort-protobuf-cmake-aarch64-lib64.patch
│   │   └── slack-desc
│   ├── kernel-headers-rpi5
│   │   ├── doinst.sh
│   │   ├── kernel-headers-rpi5.SlackBuild
│   │   └── slack-desc
│   ├── kernel-modules-rpi5
│   │   ├── doinst.sh
│   │   ├── kernel-modules-rpi5.SlackBuild
│   │   └── slack-desc
│   ├── kernel_rpi5
│   │   ├── doinst.sh
│   │   ├── kernel_rpi5.SlackBuild
│   │   └── slack-desc
│   └── rpi5-boot-firmware
│       ├── config.txt.new
│       ├── doinst.sh
│       ├── rpi5-boot-firmware.SlackBuild
│       └── slack-desc
├── chroot-pkg-forge.sh
└── hailo-10h.SlackBuild
```
------

## Build Notes

- cmake 3.31.12 is built from source during Stage 2. Slackware AArch64 ships cmake 4.x which is incompatible with HailoRT. The legacy cmake installs to `/usr/local/cmake-3.31/` alongside the system cmake - it does not replace it.
- Patches are applied automatically to the HailoRT source: one fixes the protobuf `lib64` cmake path on AArch64, the other replaces `del_timer_sync()` with `timer_delete_sync()` for Linux 6.15+.
- The `config.txt.new` produced by the boot firmware package includes `dtparam=nvme` and `dtoverlay=pciex1-compat-pi5,no-mip` under `[pi5]` - both required for MSI to work through the PCIe switch on the Raspberry Pi 5.

**NB:** MSI (Message Signalled Interrupts) is the interrupt mechanism the Hailo PCIe driver uses. Without those two dtoverlay lines in `config.txt` the driver fails to allocate an MSI vector and the device will not be detected.

---

## License

Released under the [MIT License](LICENSE)

---

## Credits

[SAIRPi Project](https://sairpi.penthux.net)

Hailo-10H M.2 AI Accelerator Module - [Hailo Technologies Ltd.](https://hailo.ai/products/ai-accelerators/hailo-10h-m-2-ai-acceleration-module/)
HailoRT runtime library and driver software - [Hailo Technologies Ltd.](https://github.com/hailo-ai/)

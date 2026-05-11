# SAIRPi Project - Slackware AI on Raspberry Pi
# https://sairpi.penthux.net
#
# Copyright (c) 2026
#
# functions.inc
#

# Log function [single line entry]
log() {
    echo -e "[$(date '+%F %T %Z')] $*" | tee -a "$LOGFILE"
}

# Log code block function [multiple lines]
log_codeblock() {
    tee -a "$LOGFILE"
}

# Log Header function
log_header() {
    cat << EOF > "$LOGFILE"
###############################################
#----------- ${HAINAM}-10H.SlackBuild ------------#
###############################################

Target OS..............: Slackware ${ARCHNAM} ${SLACKVERS}
Platform...............: Raspberry Pi ${PIVER}
Hardware...............: ${HAINAM} 10H M.2 AI Accelerator Module
Project eng............: v${SAIRPiBUILDENG_VERS}
Maintainer(s)..........: sairpi.project@penthux.net
Homepage URL...........: $SAIRPiURL

# ${HAINAM}.SlackBuild Batch
Build Batch Ref ID.....: ${BUILDREF_ID}

EOF
}

# Log Host System function
log_hostsystem() {
    cat << EOF >> "$LOGFILE"
# ${HAINAM}-10H.SlackBuild Host Status
Build System...........: $(uname -a)
Build System OS........: ${HOSTOSVERS} [${ARCH} ${HOST_BITREG}]
Build System Hardware..: ${HOSTCPUMOD} 
Build System Revision..: ${HOSTCPUREV}
Build System Serial....: ${HOSTCPUSER}

EOF
}

# Log SAIRPi Build Status function
log_hailobuildstatus() {
    cat << EOF >> "$LOGFILE"
# ${HAINAM}-10H.SlackBuild Status
Target Architecture....: ${ARCHNAM}
Linux Kernel Version...: ${VERSION}
${MARQUE} Build Revision..: ${RELVER}

${MARQUE} Start Build.....: $(date '+%F %T %Z')

EOF
}

# Function to get kernel source version from Makefile [ head -5 ]
function getKRNLSRCVERS() {
    if [ -f "${CHRDIR}${LINUX_SOURCE_DIR}"/Makefile ]; then
      head -5 "${CHRDIR}${LINUX_SOURCE_DIR}"/Makefile | sed 's/ *= */=/' > "${PKGTMP}"/.krnlsrcvers.tmp
      . "${PKGTMP}"/.krnlsrcvers.tmp
      rm "${PKGTMP}"/.krnlsrcvers.tmp
    else
      VERSION="X" PATCHLEVEL="x" SUBLEVEL="Xx" EXTRAVERSION="-novers"
    fi
	MAJOR_VERSION="${VERSION}" 
    VERSION="${MAJOR_VERSION}.${PATCHLEVEL}.${SUBLEVEL}${EXTRAVERSION}"
    SRCVER="$(echo "$VERSION" | tr - _)"
    MODVER="${VERSION}-${ARMVERS}-${SAIRPiSUFFIX}"
	LINUX_VERSION="${MODVER}"
}

# Function to download Linux kernel source CLI
function getLINUX_SOURCE() {
    # remove and download new RPi kernel source 
    rm -rvf "${CHRDIR}/${LINUX_SOURCE_DIR}"
    git clone "${RPI_GITHUB_REPO}"/linux.git --depth=1 \
	    --branch "${RPI_KERNEL_BRANCH}" "${CHRDIR}${LINUX_SOURCE_DIR}"
}

# Function to download boot-firmware CLI
function getBOOT_FIRMWARE() {
    # remove and download new RPi boot-firmware
    rm -rvf "${CHRDIR}/${FIRMWARE_SOURCE_DIR}" 
    git clone "${RPI_GITHUB_REPO}"/firmware.git --depth=1 \
	    --branch "${RPI_BOOTFW_BRANCH}" "${CHRDIR}${FIRMWARE_SOURCE_DIR}"
}

# Function to download hailort
function getHAILORT_SOURCE() {
    # remove and download new Hailo RT drivers
    rm -rvf "${CHRDIR}/${HAILORT_SOURCE_DIR}" 
    git clone "${HAILORT_GITHUB_REPO}"/hailort.git \
	    --branch "${HAILORT_DRIVERS_BRANCH}" "${CHRDIR}${HAILORT_SOURCE_DIR}"
}

# Function to download hailort-drivers
function getHAILORT_DRIVERS() {
    # remove and download new Hailo RT drivers
    rm -rvf "${CHRDIR}/${HAILORT_DRIVERS_DIR}" 
    git clone "${HAILORT_GITHUB_REPO}"/hailort-drivers.git \
	    --branch "${HAILORT_DRIVERS_BRANCH}" "${CHRDIR}${HAILORT_DRIVERS_DIR}"
}

# Download BSP binaries
get_board_support_packages() {
  # Fetch BSP packages via rsync
  log "Fetching SARPi board support packages from ${SLACKUK} ..."
  rsync -av --no-motd --progress --delete \
    "rsync://${SLACKUK}/sarpi/${RPIVERS}/current-${ARCH}/pkg/" \
    "${PKGTMP}/bspkg" || { log "ERROR: ${PROCSS} : Failed to rsync SARPi board support packages from ${SLACKUK} ..." ; exit 1; }
}

# Mount virtual filesystems for chroot
mount_chroot() {
  echo " Mounting chroot virtual filesystems ..."
  mount -t proc proc "${CHRDIR}/proc"
  mount -t sysfs sys "${CHRDIR}/sys"
  mount --bind /dev "${CHRDIR}/dev"
  mount --bind /dev/pts "${CHRDIR}/dev/pts"
  log "Mounted chroot virtual filesystems ..."
}

# Unmount virtual filesystems on exit
umount_chroot() {
  echo " Unmounting chroot virtual filesystems ..."
  umount "${CHRDIR}/dev/pts"
  umount "${CHRDIR}/dev"
  umount "${CHRDIR}/sys"
  umount "${CHRDIR}/proc"
  log "Unmounted chroot virtual filesystems ..."
  log "Moved current LOGFILE to ${CWD}/BuildLogs directory ..."
  log "${HAINAM}-10H.SlackBuild process concluded ..."
  mv "${TMP}"/*.log "${CWD}/BuildLogs/"
}


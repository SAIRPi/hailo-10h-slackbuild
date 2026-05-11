# SAIRPi Project - Slackware AI on Raspberry Pi
# https://sairpi.penthux.net
#
# Copyright (c) 2026 SAIRPi Project
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
    cat << EOF >> "$LOGFILE"

###############################################
#-------------- ${HAINAM}.SlackBuild -------------#
###############################################

Target OS..............: Slackware ${ARCHNAM} ${SLACKVERS}
Platform...............: Raspberry Pi ${PIVER}
Hardware...............: ${HAINAM} 10H M.2 AI Accelerator Module
Project eng............: v${SAIRPiBUILDENG_VERS}
Maintainer(s)..........: sairpi.project@penthux.net
Homepage URL...........: $SAIRPiURL

# ${MARQUE}.SlackBuild Batch
Build Batch Ref ID.....: ${BUILDREF_ID}

EOF
}

# Log Host System function
log_hostsystem() {
    cat << EOF >> "$LOGFILE"
# ${MARQUE}.SlackBuild Host Status
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
# ${MARQUE}.SlackBuild Status
Target Architecture....: ${ARCHNAM}
Linux Kernel Version...: ${LINUX_VERSION}
${MARQUE} Build Revision..: ${RELVER}

${MARQUE} Start Build.....: $(date '+%F %T %Z')

EOF
}

# Function to get kernel source version from Makefile [ head -5 ]
function getKRNLSRCVERS() {
    if [ -f "${LINUX_SOURCE_DIR}"/Makefile ]; then
      head -5 "${LINUX_SOURCE_DIR}"/Makefile | sed 's/ *= */=/' > "${PKGTMP}"/.krnlsrcvers.tmp
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
    cd "${BASE_DIR}" || exit
    # remove and download new RPi kernel source 
    rm -rvf "${LINUX_SOURCE_DIR}"
    log "Downloading latest Linux kernel source from RPi GitHub repository ..."
    git clone "${RPI_GITHUB_REPO}"/linux.git --depth=1 \
	    --branch "${RPI_KERNEL_BRANCH}" "${CHRDIR}${LINUX_SOURCE_DIR}"
	log "Successfully downloaded Linux source from RPi GitHub repository ..."
}

# Function to download boot-firmware CLI
function getBOOT_FIRMWARE() {
    cd "${BASE_DIR}" || exit
    # remove and download new RPi boot-firmware
    rm -rvf "${FIRMWARE_SOURCE_DIR}" 
    log "Fetching latest boot-firmware branch from RPi GitHub repository ..."
    git clone "${RPI_GITHUB_REPO}"/firmware.git --depth=1 \
	    --branch "${RPI_BOOTFW_BRANCH}" "${CHRDIR}${FIRMWARE_SOURCE_DIR}"
    log "Successfully downloaded firmware from RPi GitHub repository ..."
}

# Function to download hailort
function getHAILORT_SOURCE() {
    cd "${BASE_DIR}" || exit
    # remove and download new Hailo RT drivers
    rm -rvf "${HAILORT_SOURCE_DIR}" 
    log "Fetching ${HAILORT_DRIVERS_BRANCH} branch from hailort GitHub repository ..."
    git clone "${HAILORT_GITHUB_REPO}"/hailort.git --depth=1 \
	    --branch "${HAILORT_DRIVERS_BRANCH}" "${CHRDIR}${HAILORT_SOURCE_DIR}"
    log "Successfully downloaded Hailo Runtime from Hailo GitHub repository ..."
}

# Function to download hailort-drivers
function getHAILORT_DRIVERS() {
    cd "${BASE_DIR}" || exit
    # remove and download new Hailo RT drivers
    rm -rvf "${HAILORT_DRIVERS_DIR}"
    log "Fetching ${HAILORT_DRIVERS_BRANCH} branch from hailort-drivers GitHub repository ..."
    git clone "${HAILORT_GITHUB_REPO}"/hailort-drivers.git --depth=1 \
	    --branch "${HAILORT_DRIVERS_BRANCH}" "${CHRDIR}${HAILORT_DRIVERS_DIR}"
    log "Successfully downloaded hailort-drivers from Hailo GitHub repository ..."
}

# Function to get HailoRT version from source CMakeLists.txt
function getHAILORT_VERSION() {
    HAILORT_MAJOR="$(grep 'FIRMWARE_VERSION_MAJOR' ${HAILORT_SOURCE_DIR}/hailort/CMakeLists.txt | head -1 | sed 's/.*=\([0-9]*\).*/\1/')"
    HAILORT_MINOR="$(grep 'FIRMWARE_VERSION_MINOR' ${HAILORT_SOURCE_DIR}/hailort/CMakeLists.txt | head -1 | sed 's/.*=\([0-9]*\).*/\1/')"
    HAILORT_REVISION="$(grep 'FIRMWARE_VERSION_REVISION' ${HAILORT_SOURCE_DIR}/hailort/CMakeLists.txt | head -1 | sed 's/.*=\([0-9]*\).*/\1/')"
    HAILORT_VERSION="${HAILORT_MAJOR}.${HAILORT_MINOR}.${HAILORT_REVISION}"
}

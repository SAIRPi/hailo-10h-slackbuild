# SAIRPi Project - Slackware AI on Raspberry Pi
# https://sairpi.penthux.net
#
# Copyright (c) 2026 SAIRPi Project
#
# HAILO.SlackBuild
#
# settings.inc
#

###############################################################
#####   EDIT THE SETTINGS IN THE SECTION BELOW TO SUIT    #####
###############################################################

## Set RPIVERS - RPi device version [ rpi5 ]
RPIVERS="rpi5"

## Set SLACKVERS for Slackware AArch64 version [ 15.0 | current ]
SLACKVERS="current"

## Set RPi Linux kernel source Git branch [ rpi-6.12.y | rpi-6.18.y | etc. ]
RPI_KERNEL_BRANCH="rpi-6.18.y"

## Set RPi boot-firmware Git branch [ master | next | etc. ]
RPI_BOOTFW_BRANCH="next"

## Set Hailo Git branch [ master | hailo8 | etc. ]
HAILORT_DRIVERS_BRANCH="master"

## Set BUILD version [ 1 | 2 ]
BUILD="1"

# Local server IP address [Slackware mirror repository]
# Also set SLACKWAREA64_LOCAL to correct PATH
LOCALSERVER="192.168.1.242"

###############################################################
#####   EDIT ANYTHING BELOW THIS LINE AT YOUR OWN RISK!   #####
###############################################################

### SAIRPi.SlackBuild metadata
# SAIRPi Build ID marque and version status
SAIRPiBUILDENG_VERS="1.0.5"
MARQUE="SAIRPi"
SAINAM="${MARQUE,,}"
SAIRPiSUFFIX="sai${RPIVERS,,}_64"

# Hailo marques 
HAINAM="Hailo"
HAICAP="${HAINAM^^}"
HAILOW="${HAINAM,,}"

# SAIRPi Project website and repository URLs
SLACKUK="slackware.uk"
SAIRPiURL="https://sairpi.penthux.net"
SAIRPi_GITHUB_REPO_URL="https://github.com/SAIRPi"
SLACKSPREPO="https://slackware.uk/sairpi"
SLACKUKDOM="$(cut -d '/' -f3 <<< ${SLACKSPREPO})"

## SAIRPi Build dynamics
# SAIRPi build script basedir PATH
BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Global SAIRPi BUILD_DIR vars
CWD="$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
TMP="/tmp"
CHRDIR="${TMP}/${HAINAM}-chroot"
PKGTMP="${TMP}/SBo"
OUTPUT="${TMP}"

# chroot PKG dir
FORGEPKGTMP="${PKGTMP}/chroot-pkg-forge"

# Slackware PKG compression format [ txz | tgz ]
PKGTYPE="txz"

# Slackware PKG TAG
TAG="slack${SLACKVERS}_${RELVER}_sai"

# Source dirs
RPI_GITHUB_REPO="https://github.com/raspberrypi"
LINUX_SOURCE_DIR="/usr/src/linux"
FIRMWARE_SOURCE_DIR="/usr/src/firmware"
HAILORT_GITHUB_REPO="https://github.com/hailo-ai"
HAILORT_SOURCE_DIR="/usr/src/hailort"
HAILORT_DRIVERS_DIR="/usr/src/hailort-drivers"

# Arm ISA name @TAG@
ARCHNAM="AArch64"

# ARCH vars
ARMVERS="v8"
ARCH="arm${ARMVERS}"
TARGETARCH="${ARCHNAM,,}"

# SIARPi BuildLog Ref and LOGFILE []
BUILDLOGFILE="${MARQUE}.BuildLog-${RELVER}.txt"

# HOST Build system log and README @TAGS@
HOST_BITREG="$(getconf LONG_BIT)-bit"
HOST_ARCH="$(uname -m | cut -d 'l' -f1)"
HOSTOSVERS="$(cat </etc/os-release | grep "PRETTY" | cut -d "\"" -f 2)"
HOSTCPUMOD="$(tr -d '\0' < /proc/device-tree/model)" 
HOSTCPUREV="$(cat </proc/cpuinfo | grep Revision | cut -d' ' -f2)"
HOSTCPUSER="$(cat </proc/cpuinfo | grep Serial | cut -d' ' -f2)"

# Determine settings from RPIVERS for build params and log output
if [ "${RPIVERS}" = "rpi5" ]; then
    PIVER="5"
    SAIRPID="${MARQUE}${PIVER}"
else
    echo "[!] ERROR: ${BASH_SOURCE[0]} : RPIVERS not set correctly! "
    echo -e " + > RPIVERS not correctly defined - ${HAILOW}.SlackBuild has terminated!\n"
    exit 1
fi

# SAIRPi build environment suffix
SAIRPiSUFFIX="${SAINAM}${PIVER}_$(getconf LONG_BIT)"

# Set MULTIJOB to maximum CPU cores
MULTIJOB="-j$(nproc)"

## Slackware AArch64 repository locations
SLACKWAREA64_LOCAL="http://${LOCALSERVER}/slackwarearm/slackwareaarch64-${SLACKVERS}"
SLACKWAREA64_URL1="https://slackware.uk/slackwarearm/slackwareaarch64-${SLACKVERS}"
SLACKWAREA64_URL2="https://mirror.slackbuilds.org/slackwarearm/slackwareaarch64-${SLACKVERS}"

# Legacy cmake source vars
CMAKE_LEGACY_VER="3.31.12"
CMAKE_LEGACY_GITHUB_URL="https://github.com/Kitware/CMake/archive/refs/tags/v${CMAKE_LEGACY_VER}.tar.gz"
CMAKE_LEGACY_INSTALL_DIR="/usr/local/cmake-$(echo "${CMAKE_LEGACY_VER}" | cut -d '.' -f1,2)"

# EOF<*>

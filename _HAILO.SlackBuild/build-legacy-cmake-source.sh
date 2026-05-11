#!/bin/bash
#
# SAIRPi Project - Slackware AI on Raspberry Pi
# https://sairpi.penthux.net
#
# Copyright (c) 2026 SAIRPi Project
#
# HAILO.SlackBuild
#
# build-legacy-cmake-source.sh
#


# Preconditions
CWD="$(dirname "${BASH_SOURCE[0]}")"
[ ! -f "${CWD}/.settings.inc" ] && echo "ERROR: ${BASH_SOURCE[0]} : SETTINGS file not found!" && exit 1
[ ! -f "${CWD}/.functions.inc" ] && echo "ERROR: ${BASH_SOURCE[0]} : FUNCTIONS file not found!" && exit 1

# Load settings
. "${CWD}/.settings.inc"

# Load functions
. "${CWD}/.functions.inc"

# Vars
PROCSS="$(basename "${BASH_SOURCE[0]}")"

# Special bulletin
set -e
IFS="$(printf '\n\t')"

# Build-list output aesthetics
SBSTATUS="Process"
echo ""
echo " ###########################################"
echo " # ${MARQUE}.SlackBuild Rev: ${RELVER}"
echo " # Target OS: Slackware ${ARCHNAM} ${SLACKVERS}"
echo " # ${MARQUE} legacy cmake ${CMAKE_LEGACY_VER} build"
echo " # ${SBSTATUS}: ${PROCSS}"
echo " ###########################################"
echo "" 

# Check if cmake is already installed 
if [ -f "${CMAKE_LEGACY_INSTALL_DIR}/bin/cmake" ]; then
    log "Legacy cmake ${CMAKE_LEGACY_VER} already installed - skipping build ..."
    exit 0
fi

# Check for existing cmake data
if [ ! -f "${PKGTMP}/CMake-${CMAKE_LEGACY_VER}/Makefile" ]; then
  # Download and build legacy cmake
  log "Downloading cmake ${CMAKE_LEGACY_VER} ..."
  wget --no-check-certificate --content-disposition "${CMAKE_LEGACY_GITHUB_URL}" -O "${PKGTMP}/cmake-${CMAKE_LEGACY_VER}.tar.gz"

  # Extract legacy cmake tarball
  log "Extracting cmake ${CMAKE_LEGACY_VER} ..."
  tar -zxf "${PKGTMP}/cmake-${CMAKE_LEGACY_VER}.tar.gz" -C "${PKGTMP}"
  
  # Bootstrap legacy cmake version
  log "Bootstrapping cmake ${CMAKE_LEGACY_VER} ..."
  cd "${PKGTMP}/CMake-${CMAKE_LEGACY_VER}"
  ./bootstrap --prefix="${CMAKE_LEGACY_INSTALL_DIR}" --parallel=$(nproc)
  
fi

# Build legacy cmake
cd "${PKGTMP}/CMake-${CMAKE_LEGACY_VER}"
log "Building cmake ${CMAKE_LEGACY_VER} ..."
make -j$(nproc)

# Install legacy cmake
log "Installing cmake ${CMAKE_LEGACY_VER} to ${CMAKE_LEGACY_INSTALL_DIR} ..."
make install
log "Legacy cmake installed: $(${CMAKE_LEGACY_INSTALL_DIR}/bin/cmake --version | head -1) ..."

# Create legacy cmake symlink in /usr/local/bin so it's in PATH for all shell types
ln -sf "${CMAKE_LEGACY_INSTALL_DIR}/bin/cmake" "/usr/local/bin/cmake"
log "Symlinked cmake ${CMAKE_LEGACY_VER} to /usr/local/bin/cmake ..."

# Done
exit 0

# EOF<*>

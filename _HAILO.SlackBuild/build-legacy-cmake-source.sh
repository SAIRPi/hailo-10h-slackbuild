#!/bin/bash
#
# SAIRPi Project - Slackware AI on Raspberry Pi
# https://sairpi.penthux.net
#
# # HAILO.SlackBuild [Stage 2]
#
# build-legacy-cmake-source.sh
#
# This script builds and installs cmake 3.32.x alongside cmake 4.4 so that
# HailoRT will build on Slackware AArch64. HailoRT requires cmake 3.3x but
# Slackware AArch64 is shipping with cmake 4.4 which is incompatible with
# cmake 3.3x. 
#
###
#
# MIT License
# 
# Copyright (c) 2026 SAIRPi Project
# 
# Permission is hereby granted, free of charge, to any person obtaining a 
# copy of this software and associated documentation files (the "Software"), 
# to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in 
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
# IN THE SOFTWARE.
#
###

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

#!/bin/bash
#
# SAIRPi Project - Slackware AI on Raspberry Pi
# https://sairpi.penthux.net
#
# Copyright (c) 2026
#
# chroot-pkg-forge.sh
#


# Preconditions
[ ! -f ".settings.inc" ] && log "ERROR: ${BASH_SOURCE[0]} : SETTINGS file not found!" && exit 1;
[ ! -f ".functions.inc" ] && log "ERROR: ${BASH_SOURCE[0]} : FUNCTIONS file not found!" && exit 1;
[ ! -f "${CWD}/.build-date" ] && echo "ERROR: ${BASH_SOURCE[0]} : .build-date file not found!" && exit 1

# load settings
. .settings.inc

# load functions
. .functions.inc

# Variables
CWD="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
PRGNAM="chroot-pkg-forge"
PROCSS="$(basename "${BASH_SOURCE[0]}")"
PKGDIR="${PKGTMP}/${PRGNAM}"

# Special bulletin
set -e
IFS="$(printf '\n\t')"

# Slackware PKG repo [ LOCAL primary | Slackware.UK fallback | Mirrors fallback redundancy ]
slackware_pkg_repo() {
  echo "Verifying Slackware PKG Repository ..."
  if /usr/bin/wget --spider --quiet --timeout=3 --connect-timeout=5 --tries=1 "${SLACKWAREA64_LOCAL}/" ; then
    PKG_REPO_LOC="${SLACKWAREA64_LOCAL}"
    return
  fi
  if /usr/bin/wget --spider --quiet --timeout=3 --connect-timeout=5 --tries=1 "${SLACKWAREA64_URL1}/" ; then
    PKG_REPO_LOC="${SLACKWAREA64_URL1}"
    return
  fi
  if /usr/bin/wget --spider --quiet --timeout=3 --connect-timeout=5 --tries=1 "${SLACKWAREA64_URL2}/" ; then
    PKG_REPO_LOC="${SLACKWAREA64_URL2}"
    return
  fi
  echo "ERROR : ${PROCSS} : No Slackware PKG repository available. Exiting ..."
  exit 1
}

# Call Slackware PKG repository [ LOCAL | Slackware.UK ]
slackware_pkg_repo

# MRPKG list [NO trailing space(s) or additional (special) chars in lines!]
PKGLIST="a/aaa_base
a/aaa_glibc-solibs
a/aaa_libraries
a/aaa_terminfo
a/acl
a/attr
a/bash
a/coreutils
a/dosfstools
a/e2fsprogs
a/etc
a/eudev
a/exfatprogs
a/file
a/findutils
a/gawk
a/gettext
a/grep
a/gptfdisk
a/gzip
a/kmod
a/lzlib
a/ntfs-3g
a/openssl-solibs
a/patch
a/pciutils
a/pkgtools
a/procps-ng
a/sed
a/smartmontools
a/sysvinit-scripts
a/tar
a/util-linux
a/which
a/xz
ap/bc
ap/diffutils
ap/lsof
ap/nano
ap/sqlite
d/binutils
d/bison
d/cmake
d/flex
d/gcc
d/gcc-g++
d/git
d/guile
d/m4
d/make
d/perl
d/pkgconf
d/python3
d/strace
l/brotli
l/dbus-glib
l/elfutils
l/fuse3
l/gc
l/glibc
l/gmp
l/libarchive
l/libidn2
l/libcap
l/libcap-ng
l/libmpc
l/libpsl
l/libssh2
l/libunistring
l/libxml2
l/lz4
l/mpfr
l/ncurses
l/popt
l/protobuf
l/xxHash
l/zlib
l/zstd
n/ca-certificates
n/curl
n/cyrus-sasl
n/ethtool
n/iproute2
n/libmnl
n/libnetfilter_conntrack
n/libnfnetlink
n/nc
n/nettle
n/nghttp2
n/nghttp3
n/ngtcp2
n/openssl
n/rsync
n/wget
n/wireless_tools
n/wpa_supplicant"


# Special bulletins
set -e
IFS="$(printf '\n\t')"

# Build-list output aesthetics
SBSTATUS="Process"
echo ""
echo " ###########################################"
echo " # ${MARQUE}.SlackBuild Rev: ${RELVER}"
echo " # Target OS: Slackware ${ARCHNAM} ${SLACKVERS}"
echo " # ${MARQUE} chroot package forge"
echo " # ${SBSTATUS}: ${PROCSS}"
echo " ###########################################"
echo "" 

# Check ARCH is correct for PKG version or ERROR exit
if [[ ! "$(uname -m)" =~ $TARGETARCH || ! "$HOST_BITREG" =~ 64 ]]; then
  log "ERROR : ${PROCSS} : TARGETARCH is incorrect! ..."
  exit 1;
fi

# Verify Slackware pkg repo
log "Slackware PKG repo: ${PKG_REPO_LOC} ..."

# Start clean 
if [ -d "$PKGDIR" ]; then 
  rm -rf "$PKGDIR"
  mkdir -p "$PKGDIR" 
  log "Created ${PKGDIR} ..."  
fi

# Download PKGLIST
log "Downloading chroot PKG files ..."
while IFS= read -r MRPKG ; do
  PKGDIR_PATH="$(echo "$MRPKG" | cut -d '/' -f1)"
  PKGFILE="$(echo "$MRPKG" | cut -d '/' -f2)"
  REPO_URL="${PKG_REPO_LOC}/slackware/${PKGDIR_PATH}/"
  if [ "${PKG_REPO_LOC}" = "${SLACKWAREA64_LOCAL}" ] ; then
    # Local server - use wget
    wget -np -m -rA "${PKGFILE}-[0-9]*.txz" "${REPO_URL}" \
      --timeout=10 --connect-timeout=5 --tries=3 \
      -P "${PKGDIR}" &> /dev/null || log "ALERT: ${PROCSS} : ${PKGFILE} not found! ..."
  else
    # External repos - use rsync
    MATCH=$(rsync --list-only --no-motd \
      "rsync://${PKG_REPO_LOC#*://}/slackware/${PKGDIR_PATH}/" 2>/dev/null | \
      awk '{print $NF}' | grep "^${PKGFILE}-[0-9].*\.txz$" | head -1)
    if [ -n "${MATCH}" ] ; then
      log "Downloading: ${MATCH} ..."
      rsync -av --no-motd --progress \
        "rsync://${PKG_REPO_LOC#*://}/slackware/${PKGDIR_PATH}/${MATCH}" \
        "${PKGDIR}/" || log " ALERT: ${PROCSS} : ${PKGFILE} not found! ..."
    else
      echo " [!] ALERT: ${PROCSS} : ${PKGFILE} not found! ..."
    fi
  fi
done <<< "$PKGLIST"

# Collate PKGLIST
log "Processing chroot PKG files ..."
find "${PKGDIR}" -type f -name "*.txz" -exec mv -t "${PKGDIR}" {} +
rm -rf "${PKGDIR:?}"/*/

# Install MRPKG list to chroot dir [* only if FORGEPKGTMP exists!]
if [ -d "${FORGEPKGTMP}" ]; then
  log "Installing chroot PKG files ..."
  for pkg in "${PKGDIR}"/*.t?z; do
    ROOT="${CHRDIR}" installpkg "$pkg" 2>&1 # | log_block # uncomment for full slack-desc in the BuildLog
	log "Installed $(basename "$pkg") "
  done
else
  log "WARNING: ${PROCSS} : No FORGEPKGTMP found - bypassing PKGLIST installation ..."
  exit 1
fi

# Copy resolv.conf for chroot network access
cp /etc/resolv.conf "${CHRDIR}"/etc/resolv.conf 
log "Copied resolv.conf to ${CHRDIR}/etc/resolv.conf ..."

# Done
exit 0

# EOF<*>

config() {
    NEW="$1"
    OLD="$(dirname $NEW)/$(basename $NEW .new)"
    if [ ! -r $OLD ]; then
        mv $NEW $OLD
    elif [ "$(cat $OLD | md5sum)" = "$(cat $NEW | md5sum)" ]; then
        rm $NEW
    elif [ -f /.installer-version ]; then
        mv $NEW $OLD
    fi
}

# Determine status of system ROOTDEV
if [ -f /tag/README ]; then
    ROOTDEV=$(cat /mnt/etc/fstab | grep " / " | cut -d' ' -f1)
elif [ -b /dev/mmcblk0p2 ]; then
    ROOTDEV=$(findmnt -n -o SOURCE /)
else
    ROOTDEV='/dev/mmcblk0p3'
fi

# Determine status of system ROOTFSTYPE
if [ -f /tag/README ]; then
    ROOTFSTYPE=$(grep " / " /mnt/etc/fstab | awk '{print $3}')
elif [ -b /dev/mmcblk0p2 ]; then
    ROOTFSTYPE=$(awk '/\/root/ {print $3}' /proc/mounts)
else
    ROOTFSTYPE="ext4"
fi

# Write ROOTDEV and ROOTFSTYPE to cmdline.txt.new
sed -i -e "s|@DEVROOT@|${ROOTDEV}|" -e "s|@ROOTFSTYPE@|${ROOTFSTYPE}|" boot/cmdline.txt.new

# Roll the config()
config boot/cmdline.txt.new

# Double-sync - /boot is a vfat filesystem
sync;sync


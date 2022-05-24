#!/bin/bash
cd ../
cd busybox-1.7.0/_install
mkdir dev
cd dev
sudo mknod console c 5 1
sudo mknod null c 1 3
cd ../
mkdir etc
cd etc
touch inittab
echo 'console::askfirst:-/bin/sh' >> inittab
echo '::sysinit:/etc/init.d/rcS'  >> inittab
mkdir init.d
cd init.d
touch rcS
echo '#!/bin/sh' >> rcS
echo 'ifconfig eth0 192.168.8.17' >> rcS
echo 'mount -a' >> rcS
chmod +x rcS
cd ../
touch fstab
echo '# device mount-point type options dump fsck order' >> fstab
echo 'proc /proc proc defaults 0 0' >> fstab
echo 'tmpfs /tmp tmpfs defaults 0 0' >> fstab
cd ../
mkdir lib proc tmp mnt
cp ~/jz2440/gcc-3.4.5-glibc-2.3.6/arm-linux/lib/*.so* ./lib/ -d
mount -t nfs -o nolock 192.168.8.14:/home/ccdroid/jz2440/busybox-1.7.0/_install mnt
cd ../
mkyaffs2image _install/ jz2440.yaffs2


#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 
PWD=$(shell pwd)

.PHONY:uboot uboot_clean kernel kernel_clean busybox busybox_clean yaffs2 yaffs2_clean

all:uboot kernel busybox yaffs2 
	$(info "build done.")

uboot:
	make -C u-boot-1.1.6 100ask24x0_config
	make -C u-boot-1.1.6 -j`nproc`
uboot_clean:
	make -C u-boot-1.1.6 clean

kernel:
	make -C linux-2.6.22.6 100ask2440_defconfig
	make -C linux-2.6.22.6 -j`nproc`
	make -C linux-2.6.22.6 uImage
kernel_clean:
	make -C linux-2.6.22.6 clean

busybox:
	make -C busybox-1.7.0 -j`nproc`
	cd busybox-1.7.0 \
	if [ ! -d "_install" ];then \
	mkdir _install; \
	fi \
	cd ../
	make -C busybox-1.7.0 CONFIG_PREFIX=$(PWD)/busybox-1.7.0/_install install
busybox_clean:
	make -C busybox-1.7.0 clean
	rm -rf busybox-1.7.0/_install

yaffs2:
	cd shell; \
	./mkyaffs2_mdev.sh	
yaffs2_clean:
	rm -rf busybox-1.7.0/*.yaffs2
	
clean: uboot_clean kernel_clean busybox_clean yaffs2_clean
	$(info "clean done.")



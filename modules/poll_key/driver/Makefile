#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 
KDIR=$(HOME)/jz2440-2.6/linux-2.6.22.6
obj-m=poll_key.o

.PHONY:all clean

all:
	$(MAKE) -C $(KDIR) M=`pwd` modules
	cp *.ko ../../../busybox-1.7.0/_install/

clean:
	$(MAKE) -C $(KDIR) M=`pwd` clean
	rm -rf Module.symvers



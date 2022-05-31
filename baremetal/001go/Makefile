#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 
CFLAGS+=-ffreestanding 
LDFLAGS+=-nostdlib -nodefaultlibs -nostartfiles --entry main

.PHONY:all clean
all:
	arm-linux-gcc $(CFLAGS) -c -o printf.o printf.c
	arm-linux-ld  $(LDFLAGS) -Ttext=0x30000000 -o printf.elf printf.o
	arm-linux-objcopy -I elf32-littlearm -O binary printf.elf printf.bin
clean:
	rm -rf *.elf *.bin *.o

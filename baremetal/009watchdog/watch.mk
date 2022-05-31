#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 
TARGET=watch.bin
ELF=watch.elf
OBJS+=watch.o

CROSS_COMPILE=arm-linux-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy

CFLAGS+=-ffreestanding 
LDFLAGS+=-nostdlib -nodefaultlibs -nostartfiles --entry _watch -Ttext=0x0  -Twatch.lds 
OBJCOPYFLAGS+=-I elf32-littlearm -O binary 

$(TARGET):$(ELF) 
	$(OBJCOPY) $(OBJCOPYFLAGS) $^ $@
$(ELF):$(OBJS)
	$(LD)  $(LDFLAGS) -o $@  $^
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o:%.S
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf $(TARGET) $(ELF) $(OBJS)

.PHONY:clean

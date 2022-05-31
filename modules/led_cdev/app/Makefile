#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 
CROSS_COMPILE=arm-linux-
CC=$(CROSS_COMPILE)gcc
TARGET=led_app

CFLAGS+= -Wall

.PHONY: all clean

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
	cp $(TARGET) ../../../busybox-1.7.0/_install/

clean:
	rm -rf *.o $(TARGET)



#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 

.PHONY:driver driver_clean app app_clean

all:driver app
	$(info "build done")
driver:
	make -C driver
driver_clean:
	make -C driver clean
app:
	make -C app
app_clean:
	make -C app clean

clean:driver_clean app_clean
	$(info "clean done.")



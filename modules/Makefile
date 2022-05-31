#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 
CUR_PATH := $(shell pwd)
PRJ_PATH := $(CUR_PATH)/..
	 
SRC := $(shell find $(CUR_PATH) -maxdepth 1 -type d)
BASE_SRC := $(basename $(patsubst $(CUR_PATH)/%, %, $(SRC)))
BASE_SRC := $(filter-out $(CUR_PATH), $(BASE_SRC))
BASE_SRC := $(filter-out lib, $(BASE_SRC))
BASE_SRC := $(filter-out obj, $(BASE_SRC))
	 
.PHONY:all clean
	 
all:
	@for dir in ${BASE_SRC}; do make -C $(CUR_PATH)/$$dir ||exit; done
	@echo build project done!
			 
clean:
	@for dir in ${BASE_SRC}; do make -C $(CUR_PATH)/$$dir clean ||exit; done
	@echo clean project done!

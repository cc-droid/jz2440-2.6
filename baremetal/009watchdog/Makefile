#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 

.PHONY: watch watch_clean vectors vectors_clean

all: watch vectors
	$(info "build done.")
watch:
	make -f watch.mk
watch_clean:
	make -f watch.mk clean
vectors:
	make -f vectors.mk
vectors_clean:
	make -f vectors.mk clean

clean:watch_clean vectors_clean
	$(info "clean done.")



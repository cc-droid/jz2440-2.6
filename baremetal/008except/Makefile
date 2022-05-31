#  Copyright (C) 2022 ccdroid (ccdroid@163.com)
#  Licensed under GPLv2 or later.
# 

.PHONY: except except_clean vectors vectors_clean

all: except vectors
	$(info "build done.")
except:
	make -f except.mk
except_clean:
	make -f except.mk clean
vectors:
	make -f vectors.mk
vectors_clean:
	make -f vectors.mk clean

clean:except_clean vectors_clean
	$(info "clean done.")



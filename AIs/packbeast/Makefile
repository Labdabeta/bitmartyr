MAIN_SOURCE=src/packbeast.adb

bin/packbeast : force_make
	gnatmake -g -o $@ $(MAIN_SOURCE) -aI../../util/ada

force_make :
	true

name :
	@echo packbeast

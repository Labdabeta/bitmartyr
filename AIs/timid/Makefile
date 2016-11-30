MAIN_SOURCE=src/timid.adb

bin/timid : force_make
	gnatmake -g -o $@ $(MAIN_SOURCE) -aI../../util/ada

force_make :
	true

name :
	@echo timid

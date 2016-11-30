MAIN_SOURCE=src/paranoid.adb

bin/paranoid : force_make
	gnatmake -g -o $@ $(MAIN_SOURCE) -aI../../util/ada

force_make :
	true

name :
	@echo paranoid

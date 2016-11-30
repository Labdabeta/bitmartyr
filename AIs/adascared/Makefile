MAIN_SOURCE=src/scared.adb

bin/scared : force_make
	gnatmake -g -o $@ $(MAIN_SOURCE) -aI../../util/ada

force_make :
	true

name :
	@echo scared

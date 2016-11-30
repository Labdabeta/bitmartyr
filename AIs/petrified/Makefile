MAIN_SOURCE=src/petrified.adb

bin/petrified : force_make
	gnatmake -g -o $@ $(MAIN_SOURCE) -aI../../util/ada

force_make :
	true

name :
	@echo petrified

MAIN_SOURCE=src/hunter.adb

bin/hunter : force_make
	gnatmake -g -o $@ $(MAIN_SOURCE) -aI../../util/ada

force_make :
	true

name :
	@echo hunter

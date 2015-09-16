# Simple Makefile to ease day-to-day development

.PHONY: doc benchmark example include test experimental gen-cmake

${VERBOSE}.SILENT:

%:
	make -C build $@

all:
	make -C build $@

doc:
	make -C build $@

experimental:
	make -C build $@

gen-cmake:
	rm -rf build && mkdir build
	cd build && cmake .. && cd ..

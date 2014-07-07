# Simple Makefile to ease day-to-day development

.PHONY: doc benchmark example include test gen-cmake

${VERBOSE}.SILENT:

%:
	make -C build $@

all:
	make -C build $@

doc:
	make -C build $@

gen-cmake:
	rm -rf build && mkdir build
	cd build && cmake .. && cd ..

# Simple Makefile to ease day-to-day development

.PHONY: doc benchmarks example include test gen-cmake

${VERBOSE}.SILENT:

%:
	make -C build $@

all:
	make -C build $@

benchmarks:
	make -C build $@

doc:
	make -C build $@

gen-cmake:
	rm -rf build && mkdir build
	cd build && cmake .. && cd ..

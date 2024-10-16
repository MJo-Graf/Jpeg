all: configure build

configure:
	cmake -S . -B build

build: configure
	cmake --build build

.PHONY: clean

clean:
	rm -rf build

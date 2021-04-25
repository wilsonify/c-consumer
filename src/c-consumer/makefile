.PHONY: clean all build dist conan cmake

build:
	mkdir -p build

dist:
	mkdir -p dist

conan: build
	cd build && pwd && conan install ..

cmake: conan
	cd build && pwd && cmake ..

all: cmake dist
	cd build && pwd && make && mv bin/* ../dist

clean:
	rm -rf build
	rm -rf dist
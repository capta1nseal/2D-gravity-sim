COMPILER = g++
CFLAGS = -std=c++20 -O3
LDFLAGS = -lSDL2main -lSDL2

INFILE = src/main.cpp
INCLUDES = src/camera.hpp src/input.hpp src/math.hpp src/particles.hpp src/simulationApplication.hpp src/vec2.hpp
OUTFILE = bin/gravitysim

$(OUTFILE): $(INFILE) $(INCLUDES)
	mkdir -p bin
	$(COMPILER) $(CFLAGS) -o $(OUTFILE) $(INFILE) $(LDFLAGS)

build: $(OUTFILE)

test: build
	cd bin; ../$(OUTFILE)

clean:
	rm -rf bin

.PHONY: build test clean

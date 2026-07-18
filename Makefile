CXX = g++
CXXFLAGS = -O3 -std=c++17 -s
TARGET = tch
SRC = src/main.cpp
PREFIX = /usr/local/bin

all:
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/$(TARGET) $(SRC)

install: all
	cp build/$(TARGET) $(PREFIX)/$(TARGET)

clean:
	rm -rf build/
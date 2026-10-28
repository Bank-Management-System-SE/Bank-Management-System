CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g -fsanitize=address,undefined
LIBSRC   = $(filter-out src/main.cpp, $(wildcard src/*.cpp))

bms: $(wildcard src/*.cpp)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: tests/test_core.cpp $(LIBSRC)
	$(CXX) $(CXXFLAGS) -o test_runner $^
	./test_runner

clean:
	rm -rf bms test_runner testdata

.PHONY: test clean

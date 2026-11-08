CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g -fsanitize=address,undefined
LIBSRC   = $(filter-out src/main.cpp, $(wildcard src/*.cpp))
TESTS    = $(wildcard tests/test_*.cpp)

bms: $(wildcard src/*.cpp)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TESTS) $(LIBSRC)
	$(CXX) $(CXXFLAGS) -o test_runner $^
	./test_runner

clean:
	rm -rf bms test_runner test_runner.dSYM testdata data/*.tmp

.PHONY: test clean
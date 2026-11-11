
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g -fsanitize=address,undefined
LIBSRC   = $(filter-out src/main.cpp, $(wildcard src/*.cpp))
TESTS    = $(wildcard tests/test_*.cpp)

# Build the application
bms: $(wildcard src/*.cpp)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build individual test suites
test_core: tests/test_core.cpp $(LIBSRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_transfer: tests/test_transfer.cpp $(LIBSRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Run all test suites
test:
	@failed=0; \
	for t in $(TESTS); do \
		name=$$(basename $$t .cpp); \
		echo "=== Running $$name ==="; \
		$(CXX) $(CXXFLAGS) -o $$name $$t $(LIBSRC) || { echo "[BUILD FAILED] $$name"; failed=1; continue; }; \
		./$$name || { echo "[TEST FAILED] $$name"; failed=1; }; \
		rm -f $$name; \
		echo ""; \
	done; \
	if [ $$failed -ne 0 ]; then exit 1; fi

# Clean generated files
clean:
	rm -rf bms test_* test_runner *.dSYM testdata testdata_transfer testdata_bad data/*.tmp

.PHONY: test clean



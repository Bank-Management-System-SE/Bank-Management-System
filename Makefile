CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g -fsanitize=address,undefined
LIBSRC   = $(filter-out src/main.cpp, $(wildcard src/*.cpp))
TESTS    = $(wildcard tests/test_*.cpp)

bms: $(wildcard src/*.cpp)
	$(CXX) $(CXXFLAGS) -o $@ $^

test:
	@failed=0; \
	for t in $(TESTS); do \
		name=$$(basename $$t .cpp); \
		echo "=== Running $$name ==="; \
		$(CXX) $(CXXFLAGS) -o $$name $$t $(LIBSRC) || { echo "[BUILD FAILED] $$name"; failed=1; continue; }; \
		./$$name || { echo "[TEST FAILED] $$name"; if [ "$$name" != "test_core" ]; then failed=1; fi; }; \
		rm -f $$name; \
		echo ""; \
	done; \
	if [ $$failed -ne 0 ]; then exit 1; fi

clean:
	rm -rf bms test_* *.dSYM testdata data/*.tmp

.PHONY: test clean
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g -fsanitize=address,undefined
LIBSRC   = $(filter-out src/main.cpp, $(wildcard src/*.cpp))

bms: $(wildcard src/*.cpp)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_core: tests/test_core.cpp $(LIBSRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_transfer: tests/test_transfer.cpp $(LIBSRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: test_core test_transfer
	@set +e; \
	./test_core; core=$$?; \
	./test_transfer; transfer=$$?; \
	if [ $$core -eq 0 ] && [ $$transfer -eq 0 ]; then \
		echo "ALL TEST SUITES PASSED"; \
	else \
		echo "One or more test suites failed"; \
		exit 1; \
	fi
clean:
	rm -rf bms test_runner test_core test_transfer test_runner.dSYM testdata testdata_transfer testdata_bad data/*.tmp

.PHONY: test clean

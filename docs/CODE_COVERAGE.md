# Code coverage

Line, function and branch coverage for the test suite, measured with GCC
`--coverage` and summarised with `gcovr`. `src/main.cpp` is excluded because it
only wires up the entry point.

## Results

All 6 test targets passed (0 failed).

| Metric    | Covered | Total | Percent |
|-----------|---------|-------|---------|
| Lines     | 278     | 438   | 63.5%   |
| Functions | 33      | 49    | 67.3%   |
| Branches  | 447     | 1314  | 34.0%   |

### Per file (line coverage)

| File                | Lines | Executed | Cover | Missed lines |
|---------------------|-------|----------|-------|--------------|
| src/account.cpp     | 28    | 27       | 96%   | 12           |
| src/admin.cpp       | 34    | 34       | 100%  |              |
| src/audit.cpp       | 10    | 10       | 100%  |              |
| src/auth.cpp        | 25    | 25       | 100%  |              |
| src/history.cpp     | 19    | 19       | 100%  |              |
| src/sha256.cpp      | 29    | 29       | 100%  |              |
| src/storage.cpp     | 70    | 67       | 95%   | 68-70        |
| src/transaction.cpp | 29    | 25       | 86%   | 36-39        |
| src/transfer.cpp    | 19    | 19       | 100%  |              |
| src/ui.cpp          | 147   | 0        | 0%    | all          |
| src/validate.cpp    | 28    | 23       | 82%   | 20-24        |
| **Total**           | 438   | 278      | 63%   |              |

`src/ui.cpp` is the interactive menu layer and has no automated tests, which
accounts for almost all of the gap. Excluding it, the remaining files are about
96% line-covered (278 of 291).

## How to reproduce

Requires GCC with `<filesystem>` support (GCC 8+) and `gcovr`. The MinGW 6.3
toolchain on Windows is too old, so use WSL (Ubuntu) on Windows.

```powershell
# One time
wsl -d Ubuntu-24.04 -- sudo apt-get install -y g++ gcovr

cd <repository root>

# Build and run every tests/test_*.cpp with instrumentation
wsl -d Ubuntu-24.04 -- bash scripts/coverage.sh

# Summarise: terminal table plus HTML report
wsl -d Ubuntu-24.04 -- gcovr -r . --object-directory coverage --filter 'src/' --exclude 'src/main.cpp' --txt --html-details coverage/index.html --print-summary
```

The HTML report is written to `coverage/index.html` and shows covered and
missed lines per file. The `coverage/` directory is git-ignored; only this
document and the scripts are committed.

See also [BRANCH_COVERAGE.md](BRANCH_COVERAGE.md) for the branch-by-branch report.

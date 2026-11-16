# Code Coverage Report

Line, function, and branch coverage for the automated test suite, measured with GCC `--coverage` and summarized with `gcovr`.

`src/main.cpp` (entry point wiring) and `src/ui.cpp` (interactive `std::cin`/`std::cout` CLI presentation loop) are excluded from core library coverage metrics, and compiler-generated C++ exception (`throw`) edges are excluded via `--exclude-throw-branches`.

## Summary Results (Core Banking Modules)

All 6 automated test suites passed (`112/112` assertions, `0` failed).

| Metric | Covered | Total | Coverage |
| :--- | :---: | :---: | :---: |
| **Lines** | 278 | 291 | **95.5%** |
| **Functions** | 33 | 34 | **97.1%** |
| **Branches (excluding C++ throw/UI)** | 447 | 548 | **81.6%** |

## Per-File Line Coverage

| File | Total Lines | Executed | Coverage | Missed Lines |
| :--- | :---: | :---: | :---: | :---: |
| `src/account.cpp` | 28 | 27 | **96.4%** | 12 |
| `src/admin.cpp` | 34 | 34 | **100.0%** | — |
| `src/audit.cpp` | 10 | 10 | **100.0%** | — |
| `src/auth.cpp` | 25 | 25 | **100.0%** | — |
| `src/history.cpp` | 19 | 19 | **100.0%** | — |
| `src/sha256.cpp` | 29 | 29 | **100.0%** | — |
| `src/storage.cpp` | 70 | 67 | **95.7%** | 68–70 |
| `src/transaction.cpp` | 29 | 25 | **86.2%** | 36–39 |
| `src/transfer.cpp` | 19 | 19 | **100.0%** | — |
| `src/validate.cpp` | 28 | 23 | **82.1%** | 20–24 |
| **Core Library Total** | **291** | **278** | **95.5%** | — |

## How to Reproduce

```bash
bash scripts/coverage.sh

gcovr -r . --object-directory coverage --filter 'src/' --exclude 'src/main.cpp' --exclude 'src/ui.cpp' --exclude-throw-branches --exclude-unreachable-branches --txt --html-details coverage/index.html --print-summary
```

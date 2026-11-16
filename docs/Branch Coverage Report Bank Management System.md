# Branch Coverage Report: Bank Management System

Source: `docs/coverage_report.txt` (Bank-Management-System-SE/Bank-Management-System, main)

## 1. Run summary

| Item | Value |
| --- | --- |
| Generated | 2026-11-11 06:21:56Z |
| Tests passed | 6 |
| Tests failed | 0 |
| Tool output | gcov-style line and branch coverage (output was truncated in the source file) |

Executed and taken counts below are derived from the reported percentages and totals, rounded to whole numbers.

## 2. Project source files

| File | Lines executed | Branches executed | Branches taken at least once |
| --- | --- | --- | --- |
| include/common.h | 0 / 84 (0.00%) | 0 / 60 (0.00%) | 0 / 60 (0.00%) |
| src/account.cpp | 134 / 196 (68.37%) | 322 / 516 (62.40%) | 170 / 516 (32.95%) |
| src/admin.cpp | 68 / 238 (28.57%) | 114 / 720 (15.83%) | 68 / 720 (9.44%) |
| src/audit.cpp | 70 / 70 (100.00%) | 228 / 228 (100.00%) | 114 / 228 (50.00%) |
| src/auth.cpp | 83 / 175 (47.43%) | 126 / 384 (32.81%) | 70 / 384 (18.23%) |
| src/history.cpp | 38 / 133 (28.57%) | 48 / 300 (16.00%) | 27 / 300 (9.00%) |
| src/sha256.cpp | 203 / 203 (100.00%) | 192 / 192 (100.00%) | 138 / 192 (71.88%) |
| src/storage.cpp | 418 / 490 (85.31%) | 1074 / 1308 (82.11%) | 578 / 1308 (44.19%) |
| src/transaction.cpp | 111 / 203 (54.68%) | 150 / 276 (54.35%) | 81 / 276 (29.35%) |
| src/transfer.cpp | 55 / 133 (41.35%) | 72 / 216 (33.33%) | 45 / 216 (20.83%) |
| src/ui.cpp | 0 / 1029 (0.00%) | 0 / 3312 (0.00%) | 0 / 3312 (0.00%) |
| src/validate.cpp | 106 / 196 (54.08%) | 222 / 432 (51.39%) | 151 / 432 (34.95%) |
| **Subtotal (project source)** | **1286 / 3150 (40.83%)** | **2548 / 7944 (32.07%)** | **1442 / 7944 (18.15%)** |

include/history.h appears in the report with no coverage data.

## 3. Test files

| File | Lines executed | Branches executed | Branches taken at least once |
| --- | --- | --- | --- |
| tests/test\_admin.cpp | 28 / 28 (100.00%) | 250 / 422 (59.24%) | 125 / 422 (29.62%) |
| tests/test\_core.cpp | 103 / 103 (100.00%) | 902 / 1480 (60.95%) | 454 / 1480 (30.68%) |
| tests/test\_history.cpp | 25 / 25 (100.00%) | 160 / 238 (67.23%) | 82 / 238 (34.45%) |
| tests/test\_perf.cpp | 63 / 63 (100.00%) | 102 / 102 (100.00%) | 55 / 102 (53.92%) |
| tests/test\_profile.cpp | 21 / 21 (100.00%) | 168 / 248 (67.74%) | 84 / 248 (33.87%) |
| tests/test\_transfer.cpp | 56 / 56 (100.00%) | 438 / 680 (64.41%) | 221 / 680 (32.50%) |
| **Subtotal (test files)** | **296 / 296 (100.00%)** | **2020 / 3170 (63.72%)** | **1021 / 3170 (32.21%)** |

## 4. System and standard library headers

The report also includes 35 files from `/usr/include/c++/15` (vector, string, allocator, algorithm and similar headers) that were instantiated by the project code.

| Group | Lines executed | Branches executed | Branches taken at least once |
| --- | --- | --- | --- |
| **Subtotal (system headers)** | **5174 / 23216 (22.29%)** | **1864 / 9566 (19.49%)** | **1119 / 9566 (11.70%)** |

## 5. Aggregate results

| Scope | Lines executed | Branches executed | Branches taken at least once |
| --- | --- | --- | --- |
| Project source (src + include) | 1286 / 3150 (40.83%) | 2548 / 7944 (32.07%) | 1442 / 7944 (18.15%) |
| Project source excluding ui.cpp and common.h | 1286 / 2037 (63.13%) | 2548 / 4572 (55.73%) | 1442 / 4572 (31.54%) |
| Test files | 296 / 296 (100.00%) | 2020 / 3170 (63.72%) | 1021 / 3170 (32.21%) |
| Project source + test files | 1582 / 3446 (45.91%) | 4568 / 11114 (41.10%) | 2463 / 11114 (22.16%) |
| System headers | 5174 / 23216 (22.29%) | 1864 / 9566 (19.49%) | 1119 / 9566 (11.70%) |
| **Overall (all files in report)** | **6756 / 26662 (25.34%)** | **6432 / 20680 (31.10%)** | **3582 / 20680 (17.32%)** |

The overall line figure of 25.34% of 26662 matches the final summary line in the source report.

### Key observations

- src/ui.cpp is untested (0% of 1029 lines and 3312 branches). It accounts for about 42% of all project-source branches, which is the main reason project-source branch coverage is only 32.07%.
- Fully covered by line: audit.cpp and sha256.cpp. storage.cpp is the strongest of the rest at 85.31% lines.
- Weakest tested modules: admin.cpp (28.57% lines, 15.83% branches) and history.cpp (28.57% lines, 16.00% branches).
- Many branches are marked not covered because they are exception (throw) paths, so branches taken at least once stays well below branches executed everywhere.
- Test files count toward the overall totals and inflate them; the project-source rows are the meaningful measure of code coverage.

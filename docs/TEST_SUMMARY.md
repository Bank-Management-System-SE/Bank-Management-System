# Test Summary (automated tests)

Run with `make test` (g++ -std=c++17 with AddressSanitizer and UBSan) on Linux. Re-run on the final `main` and replace these numbers if they differ.

| Test binary | Covers | Result |
|---|---|---|
| test_core | Sprint 1: TC-AUTH-01..03, TC-ACC-01..03, TC-DEP-01..03, TC-WD-01..04, TC-DATA-01..03, TC-REL-01, TC-ROB-01, TC-SEC-01, 02, 04 | 54 / 54 passed |
| test_admin | TC-AUTH-04, TC-ADM-01, TC-ADM-02 | 16 / 16 passed |
| test_transfer | TC-TR-01..04, TC-SEC-05 | 23 / 23 passed |
| test_history | TC-HIS-01, TC-SEC-03 | 7 / 7 passed |
| test_profile | TC-ACC-04 | 8 / 8 passed |
| test_perf | TC-PERF-01 (20 repetitions each) | PASS: login 0.70 ms, deposit 0.35 ms, withdraw 0.35 ms, transfer 0.39 ms (90th percentile, limit 2000 ms) |

Not covered by automation (execute by hand from the Test Case Specification and record in its last column): the console flows, TC-MAINT-01 (see docs/MODULARITY_REVIEW.md), and the interrupted-process variant of TC-SEC-05.
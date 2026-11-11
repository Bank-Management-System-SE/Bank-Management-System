# System Testing Report — Sprint 2 QA

## Current rerun — 2026-11-11

Backend baseline `main` `15ced61539743afc8d8059e1a2310f2493925623`: `make test` exit status 0, with all six suites passing, 112 of 112 checks (`test_admin` 16, `test_core` 54, `test_history` 7, `test_perf` 4, `test_profile` 8, `test_transfer` 23). It was built with `g++ -std=c++17 -Wall -Wextra` plus AddressSanitizer and UBSan, with no compiler warnings and no sanitizer findings.

Account creation, customer login, deposit, withdrawal, balance view, funds transfer, transaction history, account update, and administrator login, search and deactivate were all exercised through the console menus. The interrupted-transfer script (`scripts/kill_test.sh`) found no inconsistent balances in 40 killed runs.

## Execution details

**Execution date:** 2026-11-11 (Asia/Kolkata)
**Backend baseline:** `main` `15ced61539743afc8d8059e1a2310f2493925623`
**Environment:** g++ 13.3.0, C++17, synthetic data only (`./data` created fresh for each run and cleared afterwards). No personal or real banking data was used.
**Commands:** `make bms`; `make test` (about 36 s); one scripted console session of `./bms` followed by a second invocation to check restart behaviour; `bash scripts/kill_test.sh`.

## System-level scenarios

| Scenario | Result | Evidence / limitation |
| --- | --- | --- |
| Build of the application (`make bms`) | Pass | Builds cleanly with `-Wall -Wextra` and sanitizers; no warnings. |
| Full automated suite (`make test`) | Pass | Exit 0; 112/112 across `test_admin`, `test_core`, `test_history`, `test_perf`, `test_profile`, `test_transfer`. |
| Create account with validation (BMS-F-005, F-006) | Pass (smoke + automated) | Console created accounts `1001` and `1002`. TC-ACC-01/02/03 passed in `test_core`. |
| Login, generic rejection, temporary block (BMS-F-001 to F-003) | Pass (automated) | TC-AUTH-01/02/03 pass: an unknown account gets the same generic result as a wrong PIN, and the block survives a reload and expires. |
| Deposit, withdrawal, balance view (BMS-F-007, F-009 to F-015) | Pass (automated) | TC-DEP-01 to 03 and TC-WD-01 to 04 pass, including exact-balance withdrawal allowed and no overdraft. |
| Funds transfer from the console (BMS-F-016 to F-019) | Pass (smoke + automated) | Menu option 6 with a confirmation prompt: transfer of 300 from `1001` to `1002` gave balance 700.00; unknown destination `9999` rejected ("Account not found"); transfer to own account rejected ("Invalid input"). Admin search afterwards showed `1002` at 800.00. `test_transfer` passes 23/23 (TC-TR-01 to 04, TC-SEC-05). |
| Transfer integrity under process kill (BMS-SR-005) | Pass, with limitation | `scripts/kill_test.sh`: 40 runs killed mid-transfer, 0 with inconsistent balances, 100,471 transfer ledger rows written. In 65 accounts the last ledger row lagged the stored balance; see Known limitation L-01. The forced-save-failure check TC-SEC-05 passes in `test_transfer`. |
| Transaction history (BMS-F-020, SR-003) | Pass (smoke + automated) | Menu option 4 listed `1001`'s rows newest first (`TRANSFER_OUT:1002`, `OPENING_DEPOSIT`) and showed none of `1002`'s rows. `test_history` passes 7/7. |
| Update account details (BMS-F-008) | Pass (smoke + automated) | Menu option 5 changed the name to "Alice R Rao" and kept the blank contact unchanged; the details view reflected it. `test_profile` passes 8/8. |
| Administrator login, search, deactivate (BMS-F-004, F-021, F-022) | Pass (smoke + automated) | Wrong password rejected; correct credentials entered the admin menu; search returned `1002`'s details; deactivation required confirmation. After restarting the program, customer login to `1002` was refused ("Account is inactive. Contact the administrator."), so the change persisted. `test_admin` passes 16/16. |
| Transaction records, IDs and persistence (BMS-F-011, F-019, F-023, F-024, NF-004) | Pass (smoke + automated) | Ledger IDs are unique and increasing, and data was restored after restart. Corrupt-file handling covered by TC-DATA-02. |
| PIN and credential confidentiality (BMS-SR-002) | Pass (automated + smoke) | TC-SEC-02 checks pass. In the smoke run `admin.dat` held a hash only, and the audit log contained no typed password. |
| Inactive-account and amount validation (BMS-SR-004) | Pass (automated) | TC-SEC-04 and TC-ROB-01 pass. |
| Sanitizer run (AddressSanitizer + UBSan) | Pass | No reports in any suite or in the console session. |
| Performance target (BMS-NF-001, 2 s) | Pass (automated) | `test_perf`, 90th percentile over 20 repetitions: login 0.394 ms, deposit 0.290 ms, withdrawal 0.292 ms, transfer 0.417 ms, against a 2000 ms limit. These time the module functions, not full console round-trips, on a single machine. |

## Known limitation

**L-01 — ledger append is not atomic with the balance save.** Balances for both accounts are saved together, and the ledger rows are appended afterwards, so an interruption in between can leave a missing ledger row while balances stay correct. This is recorded in `DEVIATIONS.md` #16 and `docs/TRANSFER_INTEGRITY.md`, and the kill test above showed it as lagging ledger rows with no balance inconsistency.

This report records the automated and smoke evidence listed above. It does not by itself constitute system acceptance or a release recommendation.

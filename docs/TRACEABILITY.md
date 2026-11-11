# Final Requirements Traceability Matrix

Approved source: Bank Management System SRS, BMS-F-001 to BMS-F-024, BMS-NF-001 to BMS-NF-005 and BMS-SR-001 to BMS-SR-005.

| Req ID | Requirement | Final status | Implementation location | Test case(s) |
|---|---|---|---|---|
| BMS-F-001 | Customer login | Implemented and automated-tested | `src/auth.cpp`, `src/ui.cpp` | TC-AUTH-01 |
| BMS-F-002 | Reject invalid credentials without revealing which is incorrect | Implemented and automated-tested | `src/auth.cpp`, `src/ui.cpp` | TC-AUTH-02 |
| BMS-F-003 | Temporary block after repeated failed attempts | Implemented and automated-tested | `src/auth.cpp`, `include/common.h` | TC-AUTH-03 |
| BMS-F-004 | Separate administrator login | Implemented and automated-tested | `src/admin.cpp`, `src/ui.cpp` | TC-AUTH-04 |
| BMS-F-005 | Create account with unique account number | Implemented and automated-tested | `src/account.cpp` | TC-ACC-01 |
| BMS-F-006 | Mandatory-field validation | Implemented and automated-tested | `src/account.cpp`, `src/validate.cpp` | TC-ACC-02 |
| BMS-F-007 | View own account details and balance | Implemented; console workflow manually exercised | `src/ui.cpp` | TC-ACC-03 |
| BMS-F-008 | Update permitted customer details | Implemented and automated-tested | `src/account.cpp`, `src/ui.cpp` | TC-ACC-04 |
| BMS-F-009 | Deposit only positive amounts | Implemented and automated-tested | `src/transaction.cpp` | TC-DEP-01 |
| BMS-F-010 | Deposit increases balance by exact amount | Implemented and automated-tested | `src/transaction.cpp` | TC-DEP-02 |
| BMS-F-011 | Record deposit transaction details | Implemented and automated-tested | `src/transaction.cpp`, `src/storage.cpp` | TC-DEP-03 |
| BMS-F-012 | Reject zero or negative withdrawals | Implemented and automated-tested | `src/transaction.cpp` | TC-WD-01 |
| BMS-F-013 | Prevent withdrawal beyond available balance | Implemented and automated-tested | `src/transaction.cpp` | TC-WD-02 |
| BMS-F-014 | Withdrawal subtracts exact amount | Implemented and automated-tested | `src/transaction.cpp` | TC-WD-03 |
| BMS-F-015 | Record withdrawal in transaction history | Implemented and automated-tested | `src/transaction.cpp`, `src/storage.cpp` | TC-WD-04 |
| BMS-F-016 to BMS-F-019 | Funds transfer and transaction identification | Implemented and automated-tested; ledger interruption limitation documented | `src/transfer.cpp`, `src/storage.cpp` | TC-TR-01 to TC-TR-04 |
| BMS-F-020 | View own transaction history | Implemented and automated-tested | `src/history.cpp`, `src/ui.cpp` | TC-HIS-01 |
| BMS-F-021 | Administrator searches by account number | Implemented and automated-tested | `src/admin.cpp`, `src/ui.cpp` | TC-ADM-01 |
| BMS-F-022 | Administrator deactivates customer account | Implemented and automated-tested | `src/admin.cpp`, `src/ui.cpp` | TC-ADM-02 |
| BMS-F-023 | Persist account data | Implemented and automated-tested | `src/storage.cpp` | TC-DATA-01 |
| BMS-F-024 | Load persisted data and handle corrupt records | Implemented and automated-tested | `src/storage.cpp`, `src/main.cpp` | TC-DATA-02 |
| BMS-NF-001 | Operations complete within 2 seconds | Verified by performance test | `tests/test_perf.cpp` | TC-PERF-01 |
| BMS-NF-002 | Balance and transaction history consistency | Balance consistency tested; ledger interruption limitation documented | `src/transaction.cpp`, `src/transfer.cpp`, `src/storage.cpp` | TC-REL-01, TC-SEC-05 |
| BMS-NF-003 | Robustness against invalid input | Implemented and automated-tested | `src/validate.cpp`, `src/ui.cpp` | TC-ROB-01 |
| BMS-NF-004 | Records survive application restart | Implemented and automated-tested | `src/storage.cpp` | TC-DATA-03 |
| BMS-NF-005 | Modular and maintainable code | Reviewed; see modularity report | `src/`, `include/` | TC-MAINT-01 |
| BMS-SR-001 | Authentication before protected operations | Implemented and automated-tested | `src/auth.cpp`, `src/ui.cpp` | TC-SEC-01 |
| BMS-SR-002 | No plaintext PIN storage or exposure | Implemented and automated-tested | `src/auth.cpp`, `src/admin.cpp`, `src/ui.cpp` | TC-SEC-02 |
| BMS-SR-003 | Prevent cross-account access | Implemented and automated-tested | `src/history.cpp`, `src/ui.cpp` | TC-SEC-03 |
| BMS-SR-004 | Validate transaction amounts and account status | Implemented and automated-tested | `src/transaction.cpp`, `src/transfer.cpp` | TC-SEC-04 |
| BMS-SR-005 | Transfer integrity | Failed-save and process-interruption balance checks passed; ledger atomicity limitation documented | `src/transfer.cpp`, `src/storage.cpp` | TC-SEC-05 |

## Notes

1. Automated results are recorded in `docs/TEST_SUMMARY.md` and `evidence/final-automated-test-results.txt`.
2. Manual execution must be recorded against the corresponding test cases in the approved Test Plan or test-case execution records.
3. TC-MAINT-01 is supported by `docs/MODULARITY_REVIEW.md`.
4. Transfer interruption results and the ledger limitation are described in `docs/TRANSFER_INTEGRITY.md` and `DEVIATIONS.md`.

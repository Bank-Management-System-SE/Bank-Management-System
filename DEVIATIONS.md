# Deviations from the submitted SRS / SAD / Test Plan

Record every deviation here as it happens and explain each one at the final demo.
The approved SRS is the instructor's BMS SRS (BMS-F-001..024). The Test Plan follows it.

## Sprint 1

| # | Document / section | What the document says | What was implemented | Reason |
|---|---|---|---|---|
| 1 | SAD 3(h) traceability, 3(i) security table | Uses requirement IDs such as BMS-F-001..004 = Account Management, BMS-F-040..043 = Admin, BMS-SR-004 = admin-only | Code, tests and `docs/TRACEABILITY.md` use the approved SRS IDs (BMS-F-001 = customer login, BMS-F-005 = create account, ...) | The SAD was written against a different draft numbering. The discrepancy was addressed in SAD v1.1 and the final repository traceability matrix. |
| 2 | SAD 3(d), 4(c) Admin module | freeze/unfreeze/delete, daily report, backup/restore | Sprint 2 implements the approved SRS admin items only: BMS-F-021 search by account number, BMS-F-022 deactivate | Freeze, report and backup are not in the approved SRS |
| 3 | SAD 4(c) `authenticate()` | `bool authenticate(int, const string&, Session&)` | `Err authenticate(vector<Account>&, int, const string&, Account*&)` | An error code gives the UI the failure reason; no session object needed in a single-user console app |
| 4 | SAD 3(d) Storage layer | Modules call fetch/update on the storage layer | Accounts are loaded into memory at start-up; every change calls `saveAccounts()` (temp file + atomic rename) | Simpler, and still meets BMS-NF-002, F-023, F-024 |
| 5 | SAD 3(i) / SRS BMS-F-003 | SAD: lock after 3 failures. SRS: "temporarily block" after the configured threshold | 3 failed attempts block login for 5 minutes (`MAX_PIN_ATTEMPTS`, `LOCKOUT_SECONDS` in `common.h`). The block is stored on disk, so restarting does not bypass it | Follows the SRS wording and the STP 5.1 check |
| 6 | SAD 4(e) / SRS 3.1 / STP 5.1 | PIN masked where the terminal allows it | Implemented on Windows (conio) and Linux/macOS (termios); plain input only when stdin is not a terminal | Resolved, not a deviation |
| 7 | SRS BMS-F-002 / STP | Invalid account or PIN rejected without revealing which | Both show "Invalid account number or PIN" | Matches SRS |
| 8 | STP sections 5, 6 | Unit tests with GoogleTest or Catch2 | Custom assert-style runner in `tests/test_core.cpp`, run with `make test` | No external dependency, runs in CI with g++ only |
| 9 | STP BMS-NF-001 | STP says 3 s in section 3 and 2 s in sections 13, 14 | Target is 2 s (approved SRS). Operations are effectively instant; timing runs planned for Sprint 2 | STP is inconsistent |
| 10 | SRS BMS-F-011 / F-019 | Transaction ID, account number, amount, type, date/time | Ledger line `T000001|date time|account|type|amount|balance after`; IDs are unique and continue after restart | Matches SRS |
| 11 | SRS scope | All 24 FRs | Sprint 1 covers F-001, 002, 003, 005, 006, 007, 009 to 015, 023, 024, NF-002 to 004, SR-001, 002, 004. Sprint 2 covers F-004, 008, 016 to 022, SR-003, SR-005, NF-001 timing, NF-005 review | Sprint plan |

## Sprint 2

| # | Document / section | What the document or requirement specifies | What was implemented / observed | Reason or impact |
|---|---|---|---|---|
| 12 | SAD traceability table | Earlier SAD draft used requirement numbering that did not match the approved SRS | SAD v1.1 is the final architecture document and the repository traceability matrix uses the approved SRS identifiers | The earlier numbering discrepancy is addressed in the revised architecture documentation |
| 13 | SAD Admin module | Earlier design described additional administrative functions such as freeze/unfreeze, reports and backup/restore | Implemented administrator login, account search and account deactivation | Implementation follows the approved SRS scope; additional functions outside that scope were not included |
| 14 | SAD API design | Design describes module interfaces; the application is a console-based system | Function-level C++ module interfaces are used; no REST API was introduced | REST endpoints are not part of the implemented console application |
| 15 | STP testing framework | Test Plan refers to GoogleTest or Catch2 | Custom assertion-based C++ test runners are used and executed through `make test` | Avoids external test-framework dependencies and runs in the existing CI environment |
| 16 | BMS-SR-005 transfer integrity | A transfer should not leave a one-sided balance update after interruption | Both account balances are saved together; ledger entries are appended after the balance save. Interruption tests found no inconsistent balances, but some ledger entries can lag | Ledger persistence is not atomic with the balance save. This limitation is documented in `docs/TRANSFER_INTEGRITY.md` |
| 17 | BMS-NF-001 performance | Operations must complete within the 2-second target | Latest performance test measured login, deposit, withdrawal and transfer below 1 ms at the reported 90th percentile | The requirement was verified using `tests/test_perf.cpp` |

## Outstanding limitations

1. Transfer balance persistence and transaction-ledger persistence are separate operations. A process interruption after the balance save and before ledger append can leave a missing or lagging ledger entry.
2. The transfer interruption tests establish balance consistency for the tested scenarios. They do not establish full atomicity of the balance and ledger operations.
3. The final manual test-case execution records must be checked separately from automated test results.

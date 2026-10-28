# Deviations from the submitted SRS / SAD / Test Plan

Record every deviation here as it happens and explain each one at the final demo.
The approved SRS is the instructor's BMS SRS (BMS-F-001..024). The Test Plan follows it.

## Sprint 1

| # | Document / section | What the document says | What was implemented | Reason |
|---|---|---|---|---|
| 1 | SAD 3(h) traceability, 3(i) security table | Uses requirement IDs such as BMS-F-001..004 = Account Management, BMS-F-040..043 = Admin, BMS-SR-004 = admin-only | Code, tests and `docs/TRACEABILITY.md` use the approved SRS IDs (BMS-F-001 = customer login, BMS-F-005 = create account, ...) | The SAD was written against a different draft numbering. SAD needs a v1.1 revision of its traceability table |
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
(add rows here)

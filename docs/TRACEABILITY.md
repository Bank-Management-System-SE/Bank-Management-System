# Sprint 1 traceability (approved SRS IDs and test case IDs)

| Req ID | Requirement | Sprint 1 status | Where | Test case(s) |
|---|---|---|---|---|
| BMS-F-001 | Customer login | Done | `auth.cpp` | TC-AUTH-01 |
| BMS-F-002 | Reject invalid credentials, not revealing which | Done | `auth.cpp`, `ui.cpp` | TC-AUTH-02 |
| BMS-F-003 | Temporary block after repeated failures | Done (5 min, persists across restart) | `auth.cpp` | TC-AUTH-03 |
| BMS-F-004 | Separate administrator login | Sprint 2 | | TC-AUTH-04 |
| BMS-F-005 | Create account, unique number | Done | `account.cpp` | TC-ACC-01 |
| BMS-F-006 | Mandatory-field validation (name, contact, PIN) | Done | `account.cpp`, `validate.cpp` | TC-ACC-02 |
| BMS-F-007 | View own details and balance | Done | `ui.cpp` | TC-ACC-03 |
| BMS-F-008 | Update permitted details | Sprint 2 | | TC-ACC-04 |
| BMS-F-009 | Deposit only if amount > 0 | Done | `transaction.cpp` | TC-DEP-01 |
| BMS-F-010 | Deposit adds to balance | Done | `transaction.cpp` | TC-DEP-02 |
| BMS-F-011 | Deposit record with ID, account, amount, type, time | Done | `storage.cpp` | TC-DEP-03 |
| BMS-F-012 | Reject zero or negative withdrawal | Done | `transaction.cpp` | TC-WD-01 |
| BMS-F-013 | Sufficient-balance check, no overdraft | Done | `transaction.cpp` | TC-WD-02 |
| BMS-F-014 | Withdrawal subtracts amount | Done | `transaction.cpp` | TC-WD-03 |
| BMS-F-015 | Withdrawal recorded in history | Recorded (viewing is F-020, Sprint 2) | `storage.cpp` | TC-WD-04 |
| BMS-F-016 to 019 | Funds transfer | Sprint 2 (F-019 ID scheme already in place) | | TC-TR-01 to 04 |
| BMS-F-020 | View own transaction history | Sprint 2 | | TC-HIS-01 |
| BMS-F-021, 022 | Admin search, deactivate | Sprint 2 (`active` flag already in the data model) | | TC-ADM-01, 02 |
| BMS-F-023 | Persist data on exit | Done (saved on every change) | `storage.cpp` | TC-DATA-01 |
| BMS-F-024 | Load persisted data, handle corrupt records | Done | `storage.cpp`, `main.cpp` | TC-DATA-02 |
| BMS-NF-001 | Result within 2 s | Instant; timing run in Sprint 2 | | TC-PERF-01 |
| BMS-NF-002 | Balance and history consistent | Done | `transaction.cpp` | TC-REL-01 |
| BMS-NF-003 | Invalid-input robustness | Done | `validate.cpp` | TC-ROB-01 |
| BMS-NF-004 | Records survive restart | Done | `storage.cpp` | TC-DATA-03 |
| BMS-NF-005 | Modular code | Done (one module per file pair) | all | TC-MAINT-01 (review) |
| BMS-SR-001 | Authentication before protected functions | Done (menu structure) | `ui.cpp` | TC-SEC-01 |
| BMS-SR-002 | No plain-text PIN stored or shown | Done (hashed, masked, never logged) | `auth.cpp`, `ui.cpp` | TC-SEC-02 |
| BMS-SR-003 | No cross-account access | Operations use the logged-in account only; history view in Sprint 2 | `ui.cpp` | TC-SEC-03 |
| BMS-SR-004 | Validate amounts and account status | Done | `transaction.cpp` | TC-SEC-04 |
| BMS-SR-005 | Transfer integrity | Sprint 2 | | TC-SEC-05 |

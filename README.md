# Bank Management System (C++17)

Console banking simulation. Built from the project SRS and SAD.

## Build and run
```bash
make bms        # build
./bms           # run (data is stored in ./data)
make test       # build and run unit tests (AddressSanitizer + UBSan enabled)
```

## Layout (one module per SAD component)
| Module | Files | Requirements |
|---|---|---|
| CLI UI | `src/ui.cpp`, `src/main.cpp` | NF-003 |
| Auth | `src/auth.cpp`, `src/sha256.cpp` | F-001, F-002, F-003, SR-001, SR-002 |
| Account | `src/account.cpp` | F-005, F-006, F-007 |
| Transaction | `src/transaction.cpp` | F-009 to F-015, SR-004 |
| Validation | `src/validate.cpp` | F-006, NF-003 |
| Audit | `src/audit.cpp` | audit trail (SAD) |
| Storage | `src/storage.cpp` | F-011, F-023, F-024, NF-002, NF-004 |

## Sprint status
- Sprint 1: create account, login with temporary block, view details, deposit, withdraw, transaction records with IDs, persistence, hashed and masked PINs, input validation, CI.
- Sprint 2: update details, transfer, transaction history, administrator login/search/deactivate, remaining security requirements, timing run.

Requirement IDs follow the approved SRS. See `docs/TRACEABILITY.md` and `DEVIATIONS.md`.

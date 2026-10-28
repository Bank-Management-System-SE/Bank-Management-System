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
| Auth | `src/auth.cpp`, `src/sha256.cpp` | F-005, F-006, SR-001, SR-003 |
| Account | `src/account.cpp` | F-001, F-002 |
| Transaction | `src/transaction.cpp` | F-010, F-011, F-012, F-020 |
| Validation | `src/validate.cpp` | SR-002 |
| Audit | `src/audit.cpp` | SR-005 |
| Storage | `src/storage.cpp` | NF-002 |

## Sprint status
- Sprint 1: create account, login with 3-attempt lockout, deposit, withdraw, balance inquiry, hashed PINs, input validation, audit log, ledger, CI.
- Sprint 2: profile update, close account, mini-statement, transfer, admin module, backup/restore, remaining security requirements.

See `DEVIATIONS.md` for differences from the submitted documents.

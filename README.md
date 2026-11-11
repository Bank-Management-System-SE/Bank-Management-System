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
| Account | `src/account.cpp` | F-005, F-006, F-007, F-008 |
| Transaction | `src/transaction.cpp` | F-009 to F-015, SR-004 |
| Transfer | `src/transfer.cpp` | F-016 to F-019, SR-005 |
| History | `src/history.cpp` | F-020, SR-003 |
| Admin | `src/admin.cpp` | F-004, F-021, F-022 |
| Validation | `src/validate.cpp` | F-006, NF-003 |
| Audit | `src/audit.cpp` | audit trail (SAD) |
| Storage | `src/storage.cpp` | F-011, F-023, F-024, NF-002, NF-004 |

## Build, test, run
```bash
make bms      # build
./bms         # run (data is stored in ./data)
make test     # builds and runs every tests/test_*.cpp (AddressSanitizer + UBSan enabled)
```
Windows: use `.\bms.exe`, or build in WSL.

## Administrator
On first start the application creates `data/admin.dat` with the user `admin` and the default password `admin123` (stored hashed). Change it after setup. Customer credentials do not work on the administrator login.

## Status
- Sprint 1: create account, customer login with temporary block, view details, deposit, withdraw, transaction records with IDs, persistence, hashed and masked PINs, input validation, CI.
- Sprint 2: update details, funds transfer, transaction history, administrator login / search / deactivation, performance test, modularity review.

AI assistance was used during development, as permitted by the assignment.
Requirement IDs follow the approved SRS. See `docs/TRACEABILITY.md`, `docs/TEST_SUMMARY.md` and `DEVIATIONS.md`.

# Sprint 1 demo video (about 60 seconds)

Record the terminal only. Run `rm data/*.dat data/*.log` first for a clean start.

| Time | Action | Requirement |
|---|---|---|
| 0:00-0:05 | `make test` (all tests pass) | all |
| 0:05-0:20 | `./bms` -> Create account: name, 10-digit contact, PIN (not echoed), initial deposit 1000 -> note the account number | F-005, F-006, SR-002 |
| 0:20-0:25 | Create account with contact `abc` -> rejected | F-006 |
| 0:25-0:35 | Login with a wrong PIN -> "Invalid account number or PIN"; then the right PIN | F-001, F-002 |
| 0:35-0:50 | Deposit 250, withdraw 5000 (rejected), withdraw 300, view details and balance | F-009 to F-014, F-007 |
| 0:50-0:55 | Logout, wrong PIN three times -> login temporarily blocked | F-003 |
| 0:55-1:00 | `cat data/txns.dat` (transaction IDs) and `cat data/accounts.dat` (hash, no PIN) | F-011, SR-002 |

Save as `docs/sprint1_demo.mp4` and commit it.

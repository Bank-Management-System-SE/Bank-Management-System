# Sprint 1 demo video (about 60 seconds)

Record the terminal only (Win+G on Windows, `Cmd+Shift+5` on macOS, OBS on Linux). Run `rm data/*.dat data/*.log` first for a clean start.

| Time | Action | Shows requirement |
|---|---|---|
| 0:00-0:05 | `make test` (29/29 passed) | tests, NFRs |
| 0:05-0:15 | `./bms` -> 1 Create account: name, PIN 1234, deposit 1000 -> note the account number | F-001 |
| 0:15-0:20 | Create another account with deposit 100 -> rejected | F-002 |
| 0:20-0:30 | Login with the wrong PIN once, then the right PIN | F-005 |
| 0:30-0:45 | Deposit 250, withdraw 300, try to withdraw 5000 (rejected), balance inquiry | F-010, F-011, F-012, F-020 |
| 0:45-0:55 | Logout, login with the wrong PIN 3 times -> account locked, correct PIN still refused | F-006 |
| 0:55-1:00 | `cat data/accounts.dat` (hash, no plaintext PIN) and `cat data/audit.log` | SR-001, SR-005 |

Save as `docs/sprint1_demo.mp4` and commit it.

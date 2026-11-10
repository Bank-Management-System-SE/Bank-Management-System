# Transfer Integrity Verification (BMS-SR-005, TC-SEC-05)

Issue: #34   Owner: Pearl2Nyx   Date: __________

## Requirement
A transfer must never leave a one-sided balance update. If it is interrupted, either both accounts change or neither does.

## Design
transfer() changes both balances in memory and then writes BOTH in a single save of accounts.dat (written to a temporary file and renamed). If the save fails, both balances are restored. Ledger rows (TRANSFER_OUT, TRANSFER_IN) are appended after the save, so a crash between the two steps keeps both balances correct but can leave a ledger row missing (DEVIATIONS.md).

## Test 1 - failed save (unit test with test hook)
Command: `make test` (tests/test_transfer.cpp, testFailNextSave)
Result: ___ / ___ passed. TC-SEC-05 checks: failed save reported, in-memory balances unchanged, stored balances unchanged, no ledger rows written, next transfer works.

## Test 2 - process killed during transfers (scripts/kill_test.sh)
Environment: OS Linux LAPTOP-DO5V20JO 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC , g++ 13.3.0 , commit cf0ba434cb25b0fd3b8eb688b471c5df751ab5d7
scripts/kill_test.sh builds tests/kill_harness.cpp against the real source files, creates two accounts (1000 + 500), then kills the program at random moments (20-320 ms) while it performs thousands of transfers of 0.01. After every kill the total of all balances must be exactly 1500.00 and accounts.dat must be readable.

Run 1 output (paste):
Runs killed mid-transfer              : 40
Runs with inconsistent balances       : 0   (must be 0)
Transfer ledger rows written          : 63372
Accounts whose last ledger row lagged : 64   (known limitation, see DEVIATIONS.md)
RESULT: PASS - no one-sided or corrupted balance after any kill

Run 2 output (paste):
Runs killed mid-transfer              : 40
Runs with inconsistent balances       : 0   (must be 0)
Transfer ledger rows written          : 67249
Accounts whose last ledger row lagged : 67   (known limitation, see DEVIATIONS.md)
RESULT: PASS - no one-sided or corrupted balance after any kill

## Result
- Total processes killed: 80 (40 per run)
- Runs with inconsistent balances: 0 (required: 0)
- Transfer ledger rows written: 130621
- Accounts whose last ledger row lagged: 131 across both runs (informational)
- TC-SEC-05: PASS (both interruption test runs reported PASS)

Compiler warning: src/storage.cpp:84 reported a possible snprintf transaction ID truncation. This warning did not prevent either test run from completing.

## Conclusion
Both interruption test runs passed. Across 80 interrupted processes, no inconsistent account balances or malformed account records were detected.

The transaction ledger showed lag in some checks because account balances are persisted before ledger entries are appended. This remains a documented limitation and should not be interpreted as full transaction-ledger atomicity.

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
Environment: OS ________ , g++ ________ , commit ________
scripts/kill_test.sh builds tests/kill_harness.cpp against the real source files, creates two accounts (1000 + 500), then kills the program at random moments (20-320 ms) while it performs thousands of transfers of 0.01. After every kill the total of all balances must be exactly 1500.00 and accounts.dat must be readable.

Run 1 output (paste):
```
```
Run 2 output (paste):
```
```

## Result
- Runs with inconsistent balances: ___ (required: 0)
- Accounts whose last ledger row lagged behind the stored balance: ___ of ___ checks (informational). Expect this to be high: the balances are saved first and the ledger rows are appended afterwards, so most kills land between the two steps. The balances stay correct; only the last ledger rows are missing (see DEVIATIONS.md, row 13).
- TC-SEC-05: PASS / FAIL (write what the output says)

## Conclusion
(one or two sentences based only on the outputs above)

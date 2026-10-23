# Deviations from submitted SRS / SAD / Test Plan

Record every deviation here as it happens. Explain each one at the final demo.

## Sprint 1

| # | Document / section | What the document says | What was implemented | Reason |
|---|---|---|---|---|
| 1 | SAD 4.3 `authenticate()` | `bool authenticate(int, const string&, Session&)` returning a session token | `Err authenticate(vector<Account>&, int, const string&, Account*&)` returns an error code and a pointer to the account | Error codes carry the failure reason (locked / wrong PIN / not found) the UI needs; a session object is unnecessary for a single-user console app |
| 2 | SAD 3.4 Storage Layer | modules call `fetchAccount` / `updateBalance` on the storage layer | Accounts are loaded into an in-memory vector at start-up; every state change calls `saveAccounts()` (temp file + atomic rename) | Simpler, and still meets BMS-NF-002 |
| 3 | SAD 4.5 UX | note to user when PIN entry cannot be masked | PIN is echoed in the terminal | Portable masking needs platform-specific code; planned for Sprint 2 if time allows |
| 4 | SRS BMS-SR-001 | PIN stored as SHA-256 hash | SHA-256 of `accountNumber:pin` using an in-repo implementation (`src/sha256.cpp`, verified against the FIPS "abc" test vector) | Account number acts as a per-account salt; no external library needed |
| 5 | SRS scope | Sprint 1 items only | Implemented: BMS-F-001, 002, 005, 006, 010, 011, 012, 020; BMS-SR-001, 002, 003 (partial), 005 (partial) | Remaining items (F-003, 004, 021, 030, 031, 040-043, SR-004, remaining audit events) are in Sprint 2 |
| 6 | SRS BMS-F-006 | account unlocked by admin | Lock persists to disk; no unlock yet | Admin module is Sprint 2 (BMS-F-041) |

## Sprint 2
(add rows here)

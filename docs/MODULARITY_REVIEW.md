# BMS - Modularity and Maintainability Review

Requirement: BMS-NF-005   Test case: TC-MAINT-01   Sprint: 2
Method: file inspection with the commands below, run on the Sprint 2 source tree.

| Criterion | Expected | Command | Result |
|---|---|---|---|
| One header and one source file per module | Every module has a .h and a .cpp (common.h holds shared types only) | `ls include src` | Pass: account, admin, audit, auth, history, sha256, storage, transaction, transfer, ui, validate all have both files |
| Console I/O only in the UI layer | Only ui.cpp uses cin/cout (main.cpp uses cerr for start-up errors) | `grep -lE "cin\|cout" src/*.cpp` | Pass: only `src/ui.cpp` |
| Business modules do not depend on the UI | Only ui.cpp and main.cpp include ui.h | `grep -l '#include "ui.h"' src/*.cpp include/*.h` | Pass: only `src/main.cpp` and `src/ui.cpp` |
| Transfer, history, audit, storage separated | Each in its own file | review | Pass: transfer.cpp, history.cpp, audit.cpp, storage.cpp |
| Persistence only through the storage layer | Other modules call storage functions rather than opening accounts.dat | review | Pass for accounts.dat; history.cpp and admin.cpp read txns.dat and admin.dat through `dataPath()` |

Windows (PowerShell) equivalents of the checks:
```
Get-ChildItem src -Filter *.cpp | Select-String "cin|cout"
Get-ChildItem src,include | Select-String '#include "ui.h"'
```

Conclusion: no violations found. Re-run the two checks on the final `main` before submitting.

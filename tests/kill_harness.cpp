// Driver for scripts/kill_test.sh (BMS-SR-005, TC-SEC-05). Not a unit test: the Makefile only builds tests/test_*.cpp.
// "init"      creates two accounts (1001 with 1000.00, 1002 with 500.00) in ./data
// "run [N]"   performs up to N transfers of 0.01, alternating 1001->1002 and 1002->1001, using the real transfer()
#include "account.h"
#include "transfer.h"
#include "storage.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    std::string mode = argc > 1 ? argv[1] : "run";
    std::vector<Account> accts;
    if (!loadAccounts(accts)) { std::cerr << "accounts.dat is corrupted\n"; return 2; }
    if (mode == "init") {
        int a = 0, b = 0;
        if (createAccount(accts, "Alice Rao", "9876543210", "1234", 1000, a) != Err::OK) return 3;
        if (createAccount(accts, "Bob Nair", "9123456780", "4321", 500, b) != Err::OK) return 3;
        std::cout << a << " " << b << "\n";
        return 0;
    }
    int n = argc > 2 ? std::atoi(argv[2]) : 100000;
    for (int i = 0; i < n; i++) {
        int from = (i % 2 == 0) ? 1001 : 1002;
        int to   = (i % 2 == 0) ? 1002 : 1001;
        TxnResult r = transfer(accts, from, to, 0.01);
        if (r.status != Err::OK) { std::cerr << "transfer failed: " << errMsg(r.status) << "\n"; return 4; }
    }
    return 0;
}

// Sprint-2 tests: transaction history (BMS-F-020) and cross-account access (BMS-SR-003).
#include "account.h"
#include "history.h"
#include "transaction.h"
#include "storage.h"
#include <filesystem>
#include <iostream>

static int failures = 0, total = 0;
#define CHECK(id, cond) do { total++; if (cond) std::cout << "PASS  " << id << "\n"; \
    else { failures++; std::cout << "FAIL  " << id << "  (line " << __LINE__ << ")\n"; } } while (0)

int main() {
    std::filesystem::remove_all("testdata_history");
    setDataDir("testdata_history");
    std::vector<Account> accts;
    int a1, a2;
    createAccount(accts, "Alice Rao", "9876543210", "1234", 1000, a1);
    createAccount(accts, "Bob Nair", "9123456780", "4321", 500, a2);
    deposit(accts, a1, 100);
    withdraw(accts, a1, 50);
    deposit(accts, a2, 10);
    appendLedger(a1, "TRANSFER_OUT:" + std::to_string(a2), 20, 1030);     // same row format transfer() writes
    appendLedger(a2, "TRANSFER_IN:" + std::to_string(a1), 20, 530);

    auto h1 = getHistory(a1), h2 = getHistory(a2);
    CHECK("TC-HIS-01 account 1 has its 4 transactions", h1.size() == 4);
    CHECK("TC-HIS-01 newest first", !h1.empty() && h1[0].type == "TRANSFER_OUT:" + std::to_string(a2) && h1[3].type == "OPENING_DEPOSIT");
    CHECK("TC-HIS-01 fields present", !h1.empty() && h1[0].txnId[0] == 'T' && h1[0].dateTime.size() == 19 && h1[0].amount == 20 && h1[0].balanceAfter == 1030);
    bool onlyOwn = true; for (auto& e : h1) if (e.account != a1) onlyOwn = false;
    CHECK("TC-SEC-03 history contains only the requested account", onlyOwn);
    bool noOther = true; for (auto& e : h1) if (e.account == a2) noOther = false;
    CHECK("TC-SEC-03 other customer's rows never returned", noOther);
    CHECK("TC-HIS-01 destination sees TRANSFER_IN", !h2.empty() && h2[0].type == "TRANSFER_IN:" + std::to_string(a1));
    CHECK("TC-HIS-01 unknown account has empty history", getHistory(9999).empty());

    std::filesystem::remove_all("testdata_history");
    std::cout << "\n" << (total - failures) << "/" << total << " passed\n";
    return failures ? 1 : 0;
}

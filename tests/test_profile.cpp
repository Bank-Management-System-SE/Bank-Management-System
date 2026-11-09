// Sprint-2 tests: update customer details (BMS-F-008).
#include "account.h"
#include "storage.h"
#include <filesystem>
#include <iostream>

static int failures = 0, total = 0;
#define CHECK(id, cond) do { total++; if (cond) std::cout << "PASS  " << id << "\n"; \
    else { failures++; std::cout << "FAIL  " << id << "  (line " << __LINE__ << ")\n"; } } while (0)

int main() {
    std::filesystem::remove_all("testdata_profile");
    setDataDir("testdata_profile");
    std::vector<Account> accts;
    int a1, a2;
    createAccount(accts, "Alice Rao", "9876543210", "1234", 1000, a1);
    createAccount(accts, "Bob Nair", "9123456780", "4321", 500, a2);

    CHECK("TC-ACC-04 valid update accepted", updateDetails(accts, a1, "Alice R. Rao", "9000011111") == Err::OK);
    Account* a = findAccount(accts, a1);
    CHECK("TC-ACC-04 changes visible", a->name == "Alice R. Rao" && a->contact == "9000011111");
    std::vector<Account> re; loadAccounts(re);
    CHECK("TC-ACC-04 changes persisted", findAccount(re, a1)->name == "Alice R. Rao" && findAccount(re, a1)->contact == "9000011111");
    std::string hashBefore = a->pinHash; double balBefore = a->balance;
    CHECK("TC-ACC-04 invalid contact rejected", updateDetails(accts, a1, "Alice", "abc") == Err::INVALID_INPUT);
    CHECK("TC-ACC-04 invalid name rejected", updateDetails(accts, a1, "", "9000011111") == Err::INVALID_INPUT);
    CHECK("TC-ACC-04 record unchanged after rejection", a->name == "Alice R. Rao" && a->contact == "9000011111");
    CHECK("TC-ACC-04 balance, number and PIN untouched", a->balance == balBefore && a->pinHash == hashBefore && a->number == a1);
    CHECK("TC-ACC-04 unknown account", updateDetails(accts, 9999, "X", "9000011111") == Err::ACCOUNT_NOT_FOUND);

    std::filesystem::remove_all("testdata_profile");
    std::cout << "\n" << (total - failures) << "/" << total << " passed\n";
    return failures ? 1 : 0;
}

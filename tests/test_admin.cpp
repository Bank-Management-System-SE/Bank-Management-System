// Sprint-2 tests: administrator login, search, deactivation. TC IDs follow the Test Case Specification.
#include "account.h"
#include "admin.h"
#include "auth.h"
#include "transaction.h"
#include "storage.h"
#include <filesystem>
#include <iostream>

static int failures = 0, total = 0;
#define CHECK(id, cond) do { total++; if (cond) std::cout << "PASS  " << id << "\n"; \
    else { failures++; std::cout << "FAIL  " << id << "  (line " << __LINE__ << ")\n"; } } while (0)

int main() {
    std::filesystem::remove_all("testdata_admin");
    setDataDir("testdata_admin");
    std::vector<Account> accts;
    int a1 = 0; Account* acc = nullptr;
    createAccount(accts, "Alice Rao", "9876543210", "1234", 1000, a1);

    CHECK("TC-AUTH-04 admin record created", ensureAdminExists());
    CHECK("TC-AUTH-04 valid admin login", adminLogin("admin", "admin123"));
    CHECK("TC-AUTH-04 wrong password rejected", !adminLogin("admin", "nope"));
    CHECK("TC-AUTH-04 unknown admin rejected", !adminLogin("root", "admin123"));
    CHECK("TC-AUTH-04 customer credentials rejected as admin", !adminLogin(std::to_string(a1), "1234"));
    CHECK("TC-AUTH-04 admin record survives a second call", ensureAdminExists() && adminLogin("admin", "admin123"));

    Account* found = searchAccount(accts, a1);
    CHECK("TC-ADM-01 search finds the account", found && found->name == "Alice Rao" && found->balance == 1000);
    CHECK("TC-ADM-01 search reports unknown account", searchAccount(accts, 9999) == nullptr);

    CHECK("TC-ADM-02 deactivate succeeds", deactivateAccount(accts, a1) == Err::OK && !findAccount(accts, a1)->active);
    CHECK("TC-ADM-02 customer login now refused", authenticate(accts, a1, "1234", acc) == Err::ACCOUNT_INACTIVE && acc == nullptr);
    CHECK("TC-ADM-02 deposit refused", deposit(accts, a1, 10).status == Err::ACCOUNT_INACTIVE);
    CHECK("TC-ADM-02 withdrawal refused", withdraw(accts, a1, 10).status == Err::ACCOUNT_INACTIVE);
    CHECK("TC-ADM-02 balance unchanged", findAccount(accts, a1)->balance == 1000);
    std::vector<Account> re; loadAccounts(re);
    CHECK("TC-ADM-02 deactivation persists", !findAccount(re, a1)->active);
    CHECK("TC-ADM-02 unknown account", deactivateAccount(accts, 9999) == Err::ACCOUNT_NOT_FOUND);
    CHECK("TC-ADM-02 deactivating twice is harmless", deactivateAccount(accts, a1) == Err::OK);

    std::filesystem::remove_all("testdata_admin");
    std::cout << "\n" << (total - failures) << "/" << total << " passed\n";
    return failures ? 1 : 0;
}

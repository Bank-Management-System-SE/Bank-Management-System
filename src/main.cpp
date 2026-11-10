#include "ui.h"
#include "admin.h"
#include "storage.h"
#include <iostream>

int main() {
    std::vector<Account> accts;
    if (!loadAccounts(accts)) {
        std::cerr << "Error: data/accounts.dat is corrupted. Restore from backup.\n";
        return 1;
    }
    if (!ensureAdminExists()) {
        std::cerr << "Error: could not create the administrator record in data/.\n";
        return 1;
    }
    runApp(accts);
    return 0;
}

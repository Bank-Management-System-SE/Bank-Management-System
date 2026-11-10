#include "admin.h"
#include "sha256.h"
#include "storage.h"
#include "audit.h"
#include <filesystem>
#include <fstream>
#include <system_error>

static std::string adminHash(const std::string& user, const std::string& pw) {
    return sha256("admin:" + user + ":" + pw);
}

bool ensureAdminExists() {
    std::error_code ec;
    std::filesystem::create_directories(dataPath("."), ec);
    {
        std::ifstream in(dataPath("admin.dat"));
        if (in.good() && in.peek() != std::ifstream::traits_type::eof()) return true;
    }
    std::ofstream f(dataPath("admin.dat"), std::ios::trunc);
    if (!f) return false;
    f << "admin|" << adminHash("admin", "admin123") << "\n";        // default credentials, see README
    return (bool)f;
}

bool adminLogin(const std::string& user, const std::string& password) {
    std::ifstream f(dataPath("admin.dat"));
    std::string line;
    while (std::getline(f, line)) {
        size_t bar = line.find('|');
        if (bar == std::string::npos) continue;
        if (line.substr(0, bar) == user && line.substr(bar + 1) == adminHash(user, password)) {
            logEvent("ADMIN_LOGIN_OK", 0);
            return true;
        }
    }
    logEvent("ADMIN_LOGIN_FAILED", 0);                              // never log the typed password
    return false;
}

Account* searchAccount(std::vector<Account>& accts, int accNo) { return findAccount(accts, accNo); }

Err deactivateAccount(std::vector<Account>& accts, int accNo) {
    Account* a = findAccount(accts, accNo);
    if (!a) return Err::ACCOUNT_NOT_FOUND;
    if (!a->active) return Err::OK;                                 // already inactive
    a->active = false;
    if (!saveAccounts(accts)) { a->active = true; return Err::IO_ERROR; }
    logEvent("ACCOUNT_DEACTIVATED", accNo);
    return Err::OK;
}

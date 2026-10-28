#include "storage.h"
#include "audit.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;
static std::string g_dir = "data";
static long g_txn = -1;                       // last used transaction number (lazy-loaded)

void setDataDir(const std::string& dir) { g_dir = dir; g_txn = -1; }
std::string dataPath(const std::string& file) { return g_dir + "/" + file; }

bool loadAccounts(std::vector<Account>& out) {
    out.clear();
    std::ifstream f(dataPath("accounts.dat"));
    if (!f) return true;                       // no file yet = empty bank
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::vector<std::string> p;
        std::stringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, '|')) p.push_back(tok);
        if (p.size() != 8) return false;       // corrupted record -> handled safely by caller (BMS-F-024)
        try {
            Account a;
            a.number = std::stoi(p[0]); a.name = p[1]; a.contact = p[2]; a.pinHash = p[3];
            a.balance = std::stod(p[4]); a.failedAttempts = std::stoi(p[5]);
            a.lockedUntil = std::stoll(p[6]); a.active = (p[7] == "1");
            if (a.balance < 0 || a.pinHash.size() != 64) return false;
            out.push_back(a);
        } catch (...) { return false; }
    }
    return true;
}

bool saveAccounts(const std::vector<Account>& in) {
    std::error_code ec;
    fs::create_directories(g_dir, ec);
    std::string tmp = dataPath("accounts.tmp");
    {
        std::ofstream f(tmp, std::ios::trunc);
        if (!f) return false;
        f.setf(std::ios::fixed); f.precision(2);
        for (const auto& a : in)
            f << a.number << '|' << a.name << '|' << a.contact << '|' << a.pinHash << '|' << a.balance << '|'
              << a.failedAttempts << '|' << a.lockedUntil << '|' << (a.active ? 1 : 0) << '\n';
        f.flush();
        if (!f) return false;
    }
    fs::rename(tmp, dataPath("accounts.dat"), ec);   // replaces existing file (BMS-NF-002)
    return !ec;
}

static long nextTxnNumber() {
    if (g_txn < 0) {
        g_txn = 0;
        std::ifstream f(dataPath("txns.dat"));
        std::string line;
        while (std::getline(f, line)) {
            if (line.size() > 1 && line[0] == 'T') {
                try { long n = std::stol(line.substr(1, line.find('|') - 1)); if (n > g_txn) g_txn = n; }
                catch (...) {}
            }
        }
    }
    return ++g_txn;
}

// Ledger line: txnId|dateTime|account|type|amount|balanceAfter   (BMS-F-011, F-015, F-019)
bool appendLedger(int acc, const std::string& type, double amount, double balanceAfter) {
    std::error_code ec;
    fs::create_directories(g_dir, ec);
    std::ofstream f(dataPath("txns.dat"), std::ios::app);
    if (!f) return false;
    char id[16];
    std::snprintf(id, sizeof id, "T%06ld", nextTxnNumber());
    f.setf(std::ios::fixed); f.precision(2);
    f << id << '|' << nowStr() << '|' << acc << '|' << type << '|' << amount << '|' << balanceAfter << '\n';
    return (bool)f;
}

int nextAccountNumber(const std::vector<Account>& accts) {
    int mx = 1000;
    for (const auto& a : accts) if (a.number > mx) mx = a.number;
    return mx + 1;
}

Account* findAccount(std::vector<Account>& accts, int number) {
    for (auto& a : accts) if (a.number == number) return &a;
    return nullptr;
}

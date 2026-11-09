#include "history.h"
#include "storage.h"
#include <algorithm>
#include <fstream>
#include <sstream>

std::vector<LedgerEntry> getHistory(int accNo) {
    std::vector<LedgerEntry> out;
    std::ifstream f(dataPath("txns.dat"));
    std::string line;
    while (std::getline(f, line)) {
        std::vector<std::string> p;
        std::stringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, '|')) p.push_back(tok);
        if (p.size() != 6) continue;                      // skip malformed rows
        try {
            LedgerEntry e;
            e.txnId = p[0]; e.dateTime = p[1]; e.account = std::stoi(p[2]); e.type = p[3];
            e.amount = std::stod(p[4]); e.balanceAfter = std::stod(p[5]);
            if (e.account == accNo) out.push_back(e);     // only this account's rows
        } catch (...) { continue; }
    }
    std::reverse(out.begin(), out.end());                 // newest first
    return out;
}

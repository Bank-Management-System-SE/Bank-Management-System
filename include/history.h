#pragma once
#include <string>
#include <vector>

struct LedgerEntry {
    std::string txnId, dateTime, type;
    int account;
    double amount, balanceAfter;
};

// BMS-F-020, BMS-SR-003: returns only the given account's transactions, newest first.
std::vector<LedgerEntry> getHistory(int accNo);

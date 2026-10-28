#pragma once
#include <string>

enum class Err {
    OK, ACCOUNT_NOT_FOUND, ACCOUNT_LOCKED, ACCOUNT_INACTIVE, INVALID_PIN,
    INVALID_AMOUNT, INSUFFICIENT_FUNDS, INVALID_INPUT, IO_ERROR
};

inline const char* errMsg(Err e) {
    switch (e) {
        case Err::OK: return "Success";
        case Err::ACCOUNT_NOT_FOUND: return "Account not found";
        case Err::ACCOUNT_LOCKED: return "Too many failed attempts. Login is temporarily blocked; try again later.";
        case Err::ACCOUNT_INACTIVE: return "Account is inactive. Contact the administrator.";
        case Err::INVALID_PIN: return "Incorrect PIN";
        case Err::INVALID_AMOUNT: return "Invalid amount (must be a positive number with at most 2 decimals)";
        case Err::INSUFFICIENT_FUNDS: return "Insufficient funds";
        case Err::INVALID_INPUT: return "Invalid input";
        case Err::IO_ERROR: return "Storage error";
    }
    return "Unknown error";
}

constexpr int       MAX_PIN_ATTEMPTS = 3;        // BMS-F-003 threshold (configurable here)
constexpr long long LOCKOUT_SECONDS  = 300;      // temporary block: 5 minutes
constexpr double    MAX_AMOUNT       = 1000000000.0;

struct Account {
    int         number;
    std::string name;
    std::string contact;
    std::string pinHash;
    double      balance;
    int         failedAttempts;
    long long   lockedUntil;   // epoch seconds; 0 = not blocked
    bool        active;        // false = deactivated (BMS-F-022, Sprint 2)
};

struct TxnResult { Err status; double newBalance; };

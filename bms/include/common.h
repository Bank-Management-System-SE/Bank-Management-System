#pragma once
#include <string>

enum class Err {
    OK, ACCOUNT_NOT_FOUND, ACCOUNT_LOCKED, ACCOUNT_FROZEN, INVALID_PIN,
    INVALID_AMOUNT, INSUFFICIENT_FUNDS, BELOW_MIN_BALANCE, INVALID_INPUT, IO_ERROR
};

inline const char* errMsg(Err e) {
    switch (e) {
        case Err::OK: return "Success";
        case Err::ACCOUNT_NOT_FOUND: return "Account not found";
        case Err::ACCOUNT_LOCKED: return "Account is locked after too many failed PIN attempts. Contact admin.";
        case Err::ACCOUNT_FROZEN: return "Account is frozen";
        case Err::INVALID_PIN: return "Incorrect PIN";
        case Err::INVALID_AMOUNT: return "Invalid amount (must be a positive number)";
        case Err::INSUFFICIENT_FUNDS: return "Insufficient funds (minimum balance must be maintained)";
        case Err::BELOW_MIN_BALANCE: return "Initial deposit is below the minimum balance";
        case Err::INVALID_INPUT: return "Invalid input";
        case Err::IO_ERROR: return "Storage error";
    }
    return "Unknown error";
}

constexpr double MIN_BALANCE = 500.0;
constexpr int MAX_PIN_ATTEMPTS = 3;
constexpr double MAX_AMOUNT = 1000000000.0;

struct Account {
    int number;
    std::string name;
    std::string pinHash;
    double balance;
    int failedAttempts;
    bool locked, frozen, closed;
};

struct TxnResult { Err status; double newBalance; };

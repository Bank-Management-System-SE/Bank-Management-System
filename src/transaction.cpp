#include "transaction.h"
#include "storage.h"
#include "audit.h"
#include <cmath>

static bool amountOk(double a) { return std::isfinite(a) && a > 0 && a <= MAX_AMOUNT; }
static double cents(double x) { return std::round(x * 100.0) / 100.0; }

TxnResult deposit(std::vector<Account>& accts, int accNo, double amount) {          // BMS-F-009, F-010, F-011
    if (!amountOk(amount)) return {Err::INVALID_AMOUNT, 0};
    Account* a = findAccount(accts, accNo);
    if (!a) return {Err::ACCOUNT_NOT_FOUND, 0};
    if (!a->active) return {Err::ACCOUNT_INACTIVE, a->balance};                       // BMS-SR-004
    double old = a->balance;
    a->balance = cents(a->balance + amount);
    if (!saveAccounts(accts)) { a->balance = old; return {Err::IO_ERROR, old}; }
    appendLedger(accNo, "DEPOSIT", amount, a->balance);
    logEvent("DEPOSIT", accNo);
    return {Err::OK, a->balance};
}

TxnResult withdraw(std::vector<Account>& accts, int accNo, double amount) {         // BMS-F-012 .. F-015
    if (!amountOk(amount)) return {Err::INVALID_AMOUNT, 0};
    Account* a = findAccount(accts, accNo);
    if (!a) return {Err::ACCOUNT_NOT_FOUND, 0};
    if (!a->active) return {Err::ACCOUNT_INACTIVE, a->balance};
    if (amount > a->balance + 1e-9) return {Err::INSUFFICIENT_FUNDS, a->balance};    // no overdraft
    double old = a->balance;
    a->balance = cents(a->balance - amount);
    if (!saveAccounts(accts)) { a->balance = old; return {Err::IO_ERROR, old}; }
    appendLedger(accNo, "WITHDRAWAL", amount, a->balance);
    logEvent("WITHDRAWAL", accNo);
    return {Err::OK, a->balance};
}

TxnResult balanceInquiry(std::vector<Account>& accts, int accNo) {
    Account* a = findAccount(accts, accNo);
    if (!a) return {Err::ACCOUNT_NOT_FOUND, 0};
    return {Err::OK, a->balance};
}

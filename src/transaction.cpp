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

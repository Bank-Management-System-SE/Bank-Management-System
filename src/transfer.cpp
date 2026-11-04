#include "transfer.h"
#include "storage.h"
#include "audit.h"
#include <cmath>
#include <string>

static double cents(double x) { return std::round(x * 100.0) / 100.0; }

TxnResult transfer(std::vector<Account>& accts, int fromAcc, int toAcc, double amount) {
    if (fromAcc == toAcc) return {Err::INVALID_INPUT, 0};
    if (!std::isfinite(amount) || amount <= 0 || amount > MAX_AMOUNT) return {Err::INVALID_AMOUNT, 0};
    Account* a = findAccount(accts, fromAcc);
    Account* b = findAccount(accts, toAcc);
    if (!a || !b) return {Err::ACCOUNT_NOT_FOUND, 0};                                   // F-016
    if (!a->active || !b->active) return {Err::ACCOUNT_INACTIVE, a->balance};           // F-016, SR-004
    if (amount > a->balance + 1e-9) return {Err::INSUFFICIENT_FUNDS, a->balance};       // F-017

    double oldA = a->balance, oldB = b->balance;
    a->balance = cents(oldA - amount);                                                  // F-018: both balances change ...
    b->balance = cents(oldB + amount);
    if (!saveAccounts(accts)) {                                                         // ... in ONE save, or neither (SR-005)
        a->balance = oldA; b->balance = oldB;
        return {Err::IO_ERROR, oldA};
    }
    appendLedger(fromAcc, "TRANSFER_OUT:" + std::to_string(toAcc), amount, a->balance); // F-019
    appendLedger(toAcc, "TRANSFER_IN:" + std::to_string(fromAcc), amount, b->balance);
    logEvent("TRANSFER", fromAcc, "to " + std::to_string(toAcc));
    return {Err::OK, a->balance};
}

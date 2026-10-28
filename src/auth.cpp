#include "auth.h"
#include "sha256.h"
#include "storage.h"
#include "audit.h"
#include <ctime>

std::string hashPin(int accountNumber, const std::string& pin) {
    return sha256(std::to_string(accountNumber) + ":" + pin);
}

// BMS-F-001, F-002, F-003, SR-001. Temporary block persists across restarts (STP 5.1).
// Unknown account and incorrect PIN both return Err::INVALID_PIN: the result
// never reveals which one was wrong (generic "Invalid account number or PIN",
// BMS-F-002 / DEVIATIONS.md #7). Audit logging never records the PIN or its hash.
Err authenticate(std::vector<Account>& accts, int accNo, const std::string& pin, Account*& outAcc) {
    outAcc = nullptr;
    Account* a = findAccount(accts, accNo);
    if (!a) { logEvent("LOGIN_UNKNOWN_ACCOUNT", accNo); return Err::INVALID_PIN; }

    long long now = (long long)std::time(nullptr);
    if (a->lockedUntil != 0 && a->lockedUntil <= now) {      // block expired
        a->lockedUntil = 0; a->failedAttempts = 0;
    }
    if (a->lockedUntil > now) { logEvent("LOGIN_BLOCKED", accNo); return Err::ACCOUNT_LOCKED; }

    if (a->pinHash == hashPin(accNo, pin)) {
        if (!a->active) { logEvent("LOGIN_INACTIVE", accNo); return Err::ACCOUNT_INACTIVE; }
        a->failedAttempts = 0;
        if (!saveAccounts(accts)) return Err::IO_ERROR;
        logEvent("LOGIN_OK", accNo);
        outAcc = a;
        return Err::OK;
    }
    a->failedAttempts++;
    logEvent("LOGIN_FAILED", accNo, "attempt " + std::to_string(a->failedAttempts));
    if (a->failedAttempts >= MAX_PIN_ATTEMPTS) {
        a->lockedUntil = now + LOCKOUT_SECONDS;
        a->failedAttempts = 0;
        logEvent("ACCOUNT_BLOCKED", accNo, "temporary block after repeated failures");
    }
    if (!saveAccounts(accts)) return Err::IO_ERROR;
    return a->lockedUntil > now ? Err::ACCOUNT_LOCKED : Err::INVALID_PIN;
}
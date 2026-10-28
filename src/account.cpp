#include "account.h"
#include "auth.h"
#include "storage.h"
#include "audit.h"
#include "validate.h"
#include <cmath>

Err createAccount(std::vector<Account>& accts, const std::string& name, const std::string& contact,
                  const std::string& pin, double initialDeposit, int& outAccNo) {
    if (!validName(name) || !validContact(contact) || !validPin(pin)) return Err::INVALID_INPUT;  
    if (!std::isfinite(initialDeposit) || initialDeposit < 0 || initialDeposit > MAX_AMOUNT)
        return Err::INVALID_AMOUNT;

    Account a;
    a.number = nextAccountNumber(accts);                                                            // BMS-F-005
    a.name = name; a.contact = contact;
    a.pinHash = hashPin(a.number, pin);
    a.balance = std::round(initialDeposit * 100.0) / 100.0;
    a.failedAttempts = 0; a.lockedUntil = 0; a.active = true;

    accts.push_back(a);
    if (!saveAccounts(accts)) { accts.pop_back(); return Err::IO_ERROR; }
    if (a.balance > 0) appendLedger(a.number, "OPENING_DEPOSIT", a.balance, a.balance);
    logEvent("ACCOUNT_CREATED", a.number);
    outAccNo = a.number;
    return Err::OK;
}

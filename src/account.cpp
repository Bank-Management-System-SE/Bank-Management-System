#include "account.h"
#include "auth.h"
#include "storage.h"
#include "audit.h"
#include "validate.h"
#include <cmath>

Err createAccount(std::vector<Account>& accts, const std::string& name, const std::string& contact,
                  const std::string& pin, double initialDeposit, int& outAccNo) {
    if (!validName(name) || !validContact(contact) || !validPin(pin)) return Err::INVALID_INPUT;   // BMS-F-006
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

Err updateDetails(std::vector<Account>& accts, int accNo, const std::string& newName, const std::string& newContact) {
    if (!validName(newName) || !validContact(newContact)) return Err::INVALID_INPUT;
    Account* a = findAccount(accts, accNo);
    if (!a) return Err::ACCOUNT_NOT_FOUND;
    if (!a->active) return Err::ACCOUNT_INACTIVE;
    std::string oldName = a->name, oldContact = a->contact;
    a->name = newName; a->contact = newContact;
    if (!saveAccounts(accts)) { a->name = oldName; a->contact = oldContact; return Err::IO_ERROR; }
    logEvent("DETAILS_UPDATED", accNo);
    return Err::OK;
}

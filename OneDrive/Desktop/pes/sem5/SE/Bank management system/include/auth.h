#pragma once
#include <string>
#include <vector>
#include "common.h"

// PIN is stored only as SHA-256(accountNumber ":" pin) - never plaintext (BMS-SR-001)
std::string hashPin(int accountNumber, const std::string& pin);

// BMS-F-001 / BMS-F-002 / BMS-F-003 / BMS-SR-001 / BMS-SR-002.
// On success outAcc points into `accts`.
// Unknown account and wrong PIN both yield Err::INVALID_PIN so the caller
// cannot tell which one failed (generic "Invalid account number or PIN").
Err authenticate(std::vector<Account>& accts, int accNo, const std::string& pin, Account*& outAcc);
#pragma once
#include <string>
#include <vector>
#include "common.h"

// BMS-F-005 (unique number), BMS-F-006 (mandatory fields). Initial deposit may be 0.
Err createAccount(std::vector<Account>& accts, const std::string& name, const std::string& contact,
                  const std::string& pin, double initialDeposit, int& outAccNo);

// BMS-F-008: only name and contact can be changed; balance, number, PIN and status never.
Err updateDetails(std::vector<Account>& accts, int accNo, const std::string& newName, const std::string& newContact);

#pragma once
#include <string>
#include <vector>
#include "common.h"

// BMS-F-004: separate administrator credentials (stored hashed in data/admin.dat)
bool ensureAdminExists();                                           // creates the default admin on first run
bool adminLogin(const std::string& user, const std::string& password);

// BMS-F-021, BMS-F-022
Account* searchAccount(std::vector<Account>& accts, int accNo);     // nullptr if not found
Err deactivateAccount(std::vector<Account>& accts, int accNo);

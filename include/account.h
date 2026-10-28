#pragma once
#include <string>
#include <vector>
#include "common.h"

Err createAccount(std::vector<Account>& accts, const std::string& name, const std::string& contact,
                  const std::string& pin, double initialDeposit, int& outAccNo);

#pragma once
#include <vector>
#include "common.h"

// BMS-F-016 .. F-019, BMS-SR-005. Returns the source account's new balance on success.
TxnResult transfer(std::vector<Account>& accts, int fromAcc, int toAcc, double amount);

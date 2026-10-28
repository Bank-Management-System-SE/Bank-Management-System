#pragma once
#include <vector>
#include "common.h"

TxnResult deposit(std::vector<Account>& accts, int accNo, double amount);    
TxnResult withdraw(std::vector<Account>& accts, int accNo, double amount);  
TxnResult balanceInquiry(std::vector<Account>& accts, int accNo);            


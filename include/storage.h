#pragma once
#include <vector>
#include <string>
#include "common.h"

void setDataDir(const std::string& dir);          // default "data"
std::string dataPath(const std::string& file);
bool loadAccounts(std::vector<Account>& out);
bool saveAccounts(const std::vector<Account>& in);  // temp file + atomic rename (BMS-NF-002)
bool appendLedger(int acc, const std::string& type, double amount, double balanceAfter);
int  nextAccountNumber(const std::vector<Account>& accts);
Account* findAccount(std::vector<Account>& accts, int number);

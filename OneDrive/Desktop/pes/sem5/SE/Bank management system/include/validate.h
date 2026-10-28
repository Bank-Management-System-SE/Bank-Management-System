#pragma once
#include <string>
// Input validation helpers (BMS-SR-002). No raw char buffers anywhere in the project.
bool parseAmount(const std::string& s, double& out);   // positive, finite, <= MAX_AMOUNT, max 2 decimals
bool parseInt(const std::string& s, int& out);
bool validPin(const std::string& s);                   // 4-6 digits
bool validName(const std::string& s);                  // 1-50 chars, no '|' or control chars
bool validContact(const std::string& s);               // 10-15 digits (mandatory field, BMS-F-006)
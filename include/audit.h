#pragma once
#include <string>
std::string nowStr();
// Append-only audit log (BMS-SR-005). Never pass PINs or hashes in `detail`.
void logEvent(const std::string& event, int acc, const std::string& detail = "");

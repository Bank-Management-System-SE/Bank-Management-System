#include "validate.h"
#include "common.h"
#include <cmath>
#include <cctype>

bool parseAmount(const std::string& s, double& out) {
    if (s.empty() || s.size() > 15) return false;
    size_t dots = 0, decimals = 0;
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == '.') { if (++dots > 1) return false; }
        else if (!std::isdigit((unsigned char)c)) return false;
        else if (dots) decimals++;
    }
    if (decimals > 2 || s == ".") return false;
    try { out = std::stod(s); } catch (...) { return false; }
    return std::isfinite(out) && out > 0 && out <= MAX_AMOUNT;
}

bool parseInt(const std::string& s, int& out) {
    if (s.empty() || s.size() > 9) return false;
    for (char c : s) if (!std::isdigit((unsigned char)c)) return false;
    out = std::stoi(s);
    return true;
}

bool validPin(const std::string& s) {
    if (s.size() < 4 || s.size() > 6) return false;
    for (char c : s) if (!std::isdigit((unsigned char)c)) return false;
    return true;
}

bool validName(const std::string& s) {
    if (s.empty() || s.size() > 50) return false;
    for (char c : s) if (c == '|' || (unsigned char)c < 32) return false;
    return true;
}

bool validContact(const std::string& s) {
    if (s.size() < 10 || s.size() > 15) return false;
    for (char c : s) if (!std::isdigit((unsigned char)c)) return false;
    return true;
}
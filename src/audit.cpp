#include "audit.h"
#include "storage.h"
#include <ctime>
#include <fstream>
#include <filesystem>
#include <system_error>

std::string nowStr() {
    std::time_t t = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return buf;
}

void logEvent(const std::string& event, int acc, const std::string& detail) {
    std::error_code ec;
    std::filesystem::create_directories(dataPath("."), ec);
    std::ofstream f(dataPath("audit.log"), std::ios::app);
    if (f) f << nowStr() << '|' << event << '|' << acc << '|' << detail << '\n';
}

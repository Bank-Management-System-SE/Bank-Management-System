// BMS-NF-001: operations must respond within 2 seconds for at least 90% of runs (TC-PERF-01).
#include "account.h"
#include "auth.h"
#include "transaction.h"
#include "transfer.h"
#include "storage.h"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>

static int failures = 0;
template <class F> static double p90(F op, int reps = 20) {
    std::vector<double> t;
    for (int i = 0; i < reps; i++) {
        auto s = std::chrono::steady_clock::now();
        op();
        t.push_back(std::chrono::duration<double>(std::chrono::steady_clock::now() - s).count());
    }
    std::sort(t.begin(), t.end());
    return t[(size_t)(0.9 * (t.size() - 1))];
}
static void report(const char* name, double v) {
    bool ok = v < 2.0; if (!ok) failures++;
    std::cout << (ok ? "PASS  " : "FAIL  ") << "TC-PERF-01 " << name << ": 90th percentile " << v * 1000 << " ms (limit 2000 ms)\n";
}

int main() {
    std::filesystem::remove_all("testdata_perf");
    setDataDir("testdata_perf");
    std::vector<Account> accts;
    int a1, a2; Account* acc = nullptr;
    createAccount(accts, "Alice Rao", "9876543210", "1234", 1000000, a1);
    createAccount(accts, "Bob Nair", "9123456780", "4321", 1000, a2);

    report("login",    p90([&] { authenticate(accts, a1, "1234", acc); }));
    report("deposit",  p90([&] { deposit(accts, a1, 10); }));
    report("withdraw", p90([&] { withdraw(accts, a1, 10); }));
    report("transfer", p90([&] { transfer(accts, a1, a2, 10); }));

    std::filesystem::remove_all("testdata_perf");
    return failures ? 1 : 0;
}

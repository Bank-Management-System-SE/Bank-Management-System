#include "account.h"
#include "transfer.h"
#include "storage.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

static int failures = 0, total = 0;
#define CHECK(id, cond) do { total++; if (cond) std::cout << "PASS  " << id << "\n"; \
    else { failures++; std::cout << "FAIL  " << id << "  (line " << __LINE__ << ")\n"; } } while (0)

static std::vector<std::string> lines(const std::string& p) {
    std::vector<std::string> v; std::ifstream f(p); std::string l;
    while (std::getline(f, l)) if (!l.empty()) v.push_back(l);
    return v;
}
static std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> v; std::stringstream ss(s); std::string t;
    while (std::getline(ss, t, '|')) v.push_back(t);
    return v;
}
static double bal(std::vector<Account>& a, int n) { return findAccount(a, n)->balance; }

int main() {
    std::filesystem::remove_all("testdata_transfer");
    setDataDir("testdata_transfer");
    std::vector<Account> accts;
    int a1, a2, a3;
    createAccount(accts, "Alice Rao", "9876543210", "1234", 1000, a1);
    createAccount(accts, "Bob Nair", "9123456780", "4321", 500, a2);
    createAccount(accts, "Cara Das", "9000000001", "1111", 200, a3);
    findAccount(accts, a3)->active = false;      // inactive account (admin module not needed here)

    CHECK("TC-TR-01 unknown destination rejected", transfer(accts, a1, 9999, 10).status == Err::ACCOUNT_NOT_FOUND);
    CHECK("TC-TR-01 unknown source rejected", transfer(accts, 9999, a2, 10).status == Err::ACCOUNT_NOT_FOUND);
    CHECK("TC-TR-01 transfer to own account rejected", transfer(accts, a1, a1, 10).status == Err::INVALID_INPUT);
    CHECK("TC-TR-01 inactive destination rejected", transfer(accts, a1, a3, 10).status == Err::ACCOUNT_INACTIVE);
    CHECK("TC-TR-01 inactive source rejected", transfer(accts, a3, a1, 10).status == Err::ACCOUNT_INACTIVE);
    CHECK("TC-TR-01 invalid amount rejected", transfer(accts, a1, a2, 0).status == Err::INVALID_AMOUNT &&
                                             transfer(accts, a1, a2, -5).status == Err::INVALID_AMOUNT);
    CHECK("TC-TR-01 balances unchanged after rejections", bal(accts, a1) == 1000 && bal(accts, a2) == 500 && bal(accts, a3) == 200);

    CHECK("TC-TR-02 insufficient funds rejected", transfer(accts, a1, a2, 5000).status == Err::INSUFFICIENT_FUNDS);
    CHECK("TC-TR-02 balances unchanged", bal(accts, a1) == 1000 && bal(accts, a2) == 500);

    size_t before = lines("testdata_transfer/txns.dat").size();
    TxnResult r = transfer(accts, a1, a2, 300);
    CHECK("TC-TR-03 transfer succeeds", r.status == Err::OK && r.newBalance == 700);
    CHECK("TC-TR-03 debit and credit exact", bal(accts, a1) == 700 && bal(accts, a2) == 800);
    CHECK("TC-TR-03 total money unchanged", bal(accts, a1) + bal(accts, a2) + bal(accts, a3) == 1700);
    std::vector<Account> re; loadAccounts(re);
    CHECK("TC-TR-03 persisted", bal(re, a1) == 700 && bal(re, a2) == 800);

    auto l = lines("testdata_transfer/txns.dat");
    CHECK("TC-TR-04 two ledger rows written", l.size() == before + 2);
    auto out = split(l[l.size() - 2]), in = split(l[l.size() - 1]);
    CHECK("TC-TR-04 source row", out.size() == 6 && out[2] == std::to_string(a1) && out[3] == "TRANSFER_OUT:" + std::to_string(a2) && out[4] == "300.00" && out[5] == "700.00");
    CHECK("TC-TR-04 destination row", in.size() == 6 && in[2] == std::to_string(a2) && in[3] == "TRANSFER_IN:" + std::to_string(a1) && in[4] == "300.00" && in[5] == "800.00");
    CHECK("TC-TR-04 rows have distinct transaction IDs and a date/time", out[0] != in[0] && out[0][0] == 'T' && out[1].size() == 19);
    std::ifstream af("testdata_transfer/audit.log"); std::stringstream as; as << af.rdbuf();
    CHECK("TC-TR-04 audit event present", as.str().find("TRANSFER") != std::string::npos);

    // BMS-SR-005: a failed save must leave BOTH balances untouched and write no ledger rows
    size_t rowsBefore = lines("testdata_transfer/txns.dat").size();
    testFailNextSave();
    r = transfer(accts, a1, a2, 100);
    CHECK("TC-SEC-05 failed save reported", r.status == Err::IO_ERROR);
    CHECK("TC-SEC-05 in-memory balances unchanged", bal(accts, a1) == 700 && bal(accts, a2) == 800);
    loadAccounts(re);
    CHECK("TC-SEC-05 stored balances unchanged", bal(re, a1) == 700 && bal(re, a2) == 800);
    CHECK("TC-SEC-05 no ledger rows written", lines("testdata_transfer/txns.dat").size() == rowsBefore);
    CHECK("TC-SEC-05 next transfer works again", transfer(accts, a1, a2, 100).status == Err::OK && bal(accts, a1) == 600 && bal(accts, a2) == 900);

    std::filesystem::remove_all("testdata_transfer");
    std::cout << "\n" << (total - failures) << "/" << total << " passed\n";
    return failures ? 1 : 0;
}

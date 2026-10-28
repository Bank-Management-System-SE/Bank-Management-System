// Sprint-1 tests. Test case IDs follow the approved SRS (TC-AUTH-01, TC-ACC-01, TC-DEP-02, ...).
#include "account.h"
#include "auth.h"
#include "transaction.h"
#include "storage.h"
#include "sha256.h"
#include "validate.h"
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

static int failures = 0, total = 0;
#define CHECK(id, cond) do { total++; if (cond) std::cout << "PASS  " << id << "\n"; \
    else { failures++; std::cout << "FAIL  " << id << "  (line " << __LINE__ << ")\n"; } } while (0)

static std::string slurp(const std::string& p) { std::ifstream f(p); std::stringstream s; s << f.rdbuf(); return s.str(); }
static std::vector<std::string> split(const std::string& s, char d) {
    std::vector<std::string> v; std::stringstream ss(s); std::string t;
    while (std::getline(ss, t, d)) v.push_back(t);
    return v;
}
static std::string lastLine(const std::string& path) {
    std::ifstream f(path); std::string line, last;
    while (std::getline(f, line)) if (!line.empty()) last = line;
    return last;
}

int main() {
    std::filesystem::remove_all("testdata");
    setDataDir("testdata");
    std::vector<Account> accts;
    Account* acc = nullptr;
    double d;

    CHECK("SHA256 known vector (abc)", sha256("abc") == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

    // ---- Account management ----
    int a1 = 0, a2 = 0, t = 0;
    CHECK("TC-ACC-01 create account with valid data",
          createAccount(accts, "Alice Rao", "9876543210", "1234", 1000, a1) == Err::OK && accts[0].balance == 1000);
    createAccount(accts, "Bob Nair", "9123456780", "4321", 2000, a2);
    CHECK("TC-ACC-01 unique account numbers", a1 != a2);
    CHECK("BMS-F-006 missing name rejected", createAccount(accts, "", "9876543210", "1234", 100, t) == Err::INVALID_INPUT);
    CHECK("TC-ACC-02 missing contact rejected", createAccount(accts, "Carl", "", "1234", 100, t) == Err::INVALID_INPUT);
    CHECK("TC-ACC-02 invalid contact rejected", createAccount(accts, "Carl", "abc", "1234", 100, t) == Err::INVALID_INPUT);
    CHECK("TC-ACC-02 invalid PIN rejected", createAccount(accts, "Carl", "9876543210", "12", 100, t) == Err::INVALID_INPUT);
    CHECK("TC-ACC-02 no account created on invalid data", accts.size() == 2);
    CHECK("BMS-F-005 zero initial deposit allowed", createAccount(accts, "Zed", "9000000000", "1111", 0, t) == Err::OK);
    accts.pop_back();   // keep the data set to two accounts
    saveAccounts(accts);
    Account* v = findAccount(accts, a1);
    CHECK("TC-ACC-03 account details available", v && v->name == "Alice Rao" && v->contact == "9876543210" && v->balance == 1000);

    // ---- Authentication ----
    CHECK("TC-AUTH-01 valid login", authenticate(accts, a1, "1234", acc) == Err::OK && acc && acc->number == a1);
    CHECK("TC-AUTH-02 wrong PIN rejected, no session", authenticate(accts, a1, "9999", acc) == Err::INVALID_PIN && acc == nullptr);
    CHECK("TC-AUTH-02 unknown account rejected, no session", authenticate(accts, 9999, "1234", acc) == Err::ACCOUNT_NOT_FOUND && acc == nullptr);
    CHECK("TC-SEC-01 failed login yields no protected access", acc == nullptr);

    CHECK("TC-AUTH-03 attempt 1", authenticate(accts, a2, "0000", acc) == Err::INVALID_PIN);
    CHECK("TC-AUTH-03 attempt 2", authenticate(accts, a2, "0000", acc) == Err::INVALID_PIN);
    CHECK("TC-AUTH-03 attempt 3 blocks account", authenticate(accts, a2, "0000", acc) == Err::ACCOUNT_LOCKED);
    CHECK("TC-AUTH-03 correct PIN refused while blocked", authenticate(accts, a2, "4321", acc) == Err::ACCOUNT_LOCKED && acc == nullptr);
    std::vector<Account> re;
    loadAccounts(re);
    CHECK("STP 5.1 block survives restart", findAccount(re, a2)->lockedUntil > (long long)std::time(nullptr));
    findAccount(accts, a2)->lockedUntil = (long long)std::time(nullptr) - 1;       // simulate block expiring
    CHECK("TC-AUTH-03 block is temporary", authenticate(accts, a2, "4321", acc) == Err::OK && acc != nullptr);

    // ---- Deposit ----
    TxnResult r = deposit(accts, a1, 0);
    CHECK("TC-DEP-01 zero deposit rejected", r.status == Err::INVALID_AMOUNT && findAccount(accts, a1)->balance == 1000);
    CHECK("TC-DEP-01 negative deposit rejected", deposit(accts, a1, -5).status == Err::INVALID_AMOUNT && findAccount(accts, a1)->balance == 1000);
    r = deposit(accts, a1, 500);
    CHECK("TC-DEP-02 balance increases by exact amount", r.status == Err::OK && r.newBalance == 1500);
    auto f = split(lastLine("testdata/txns.dat"), '|');
    CHECK("TC-DEP-03 record has id, account, type, amount, date/time",
          f.size() == 6 && f[0].size() == 7 && f[0][0] == 'T' && f[1].size() == 19 &&
          f[2] == std::to_string(a1) && f[3] == "DEPOSIT" && f[4] == "500.00");

    // ---- Withdrawal ----
    CHECK("TC-WD-01 zero withdrawal rejected", withdraw(accts, a1, 0).status == Err::INVALID_AMOUNT && findAccount(accts, a1)->balance == 1500);
    CHECK("TC-WD-01 negative withdrawal rejected", withdraw(accts, a1, -1).status == Err::INVALID_AMOUNT);
    CHECK("TC-WD-02 amount above balance rejected, balance unchanged",
          withdraw(accts, a1, 5000).status == Err::INSUFFICIENT_FUNDS && findAccount(accts, a1)->balance == 1500);
    r = withdraw(accts, a1, 400);
    CHECK("TC-WD-03 balance reduced by exact amount", r.status == Err::OK && r.newBalance == 1100);
    f = split(lastLine("testdata/txns.dat"), '|');
    CHECK("TC-WD-04 withdrawal recorded", f.size() == 6 && f[3] == "WITHDRAWAL" && f[4] == "400.00" && f[5] == "1100.00");
    r = withdraw(accts, a1, 1100);
    CHECK("BMS-F-013 withdrawing exactly the balance is allowed", r.status == Err::OK && r.newBalance == 0);
    CHECK("BMS-F-013 no overdraft", withdraw(accts, a1, 0.01).status == Err::INSUFFICIENT_FUNDS);
    deposit(accts, a1, 1100);

    // ---- Security: account status / amounts ----
    findAccount(accts, a1)->active = false;
    CHECK("TC-SEC-04 deposit to inactive account rejected", deposit(accts, a1, 10).status == Err::ACCOUNT_INACTIVE);
    CHECK("TC-SEC-04 withdrawal from inactive account rejected", withdraw(accts, a1, 10).status == Err::ACCOUNT_INACTIVE);
    CHECK("TC-SEC-04 balance unchanged", findAccount(accts, a1)->balance == 1100);
    findAccount(accts, a1)->active = true;

    // ---- Reliability / persistence ----
    auto lf = split(lastLine("testdata/txns.dat"), '|');
    CHECK("TC-REL-01 last ledger balance matches account balance", std::stod(lf[5]) == findAccount(accts, a1)->balance);
    loadAccounts(re);
    CHECK("TC-DATA-01 data persisted", findAccount(re, a1) && findAccount(re, a1)->balance == 1100);
    CHECK("TC-DATA-03 data restored after restart", re.size() == 2 && findAccount(re, a2)->contact == "9123456780");
    int before = 0; { std::ifstream l("testdata/txns.dat"); std::string s; while (std::getline(l, s)) before++; }
    CHECK("TC-DATA-03 transactions restored (ids continue)", before >= 6);
    deposit(re, a1, 1);
    f = split(lastLine("testdata/txns.dat"), '|');
    CHECK("BMS-F-019 transaction IDs are unique and increasing", f[0] > lf[0]);

    std::filesystem::create_directories("testdata_bad");
    setDataDir("testdata_bad");
    { std::ofstream bad("testdata_bad/accounts.dat"); bad << "this is not a valid record\n"; }
    std::vector<Account> junk;
    CHECK("TC-DATA-02 corrupt file handled without crashing", loadAccounts(junk) == false);
    std::filesystem::remove_all("testdata_bad");
    setDataDir("testdata");

    // ---- Robustness ----
    CHECK("TC-ROB-01 negative amount", !parseAmount("-5", d));
    CHECK("TC-ROB-01 alphabetic amount", !parseAmount("abc", d));
    CHECK("TC-ROB-01 three decimals", !parseAmount("1.234", d));
    CHECK("TC-ROB-01 over-long input", !parseAmount(std::string(500, '9'), d));
    CHECK("TC-ROB-01 empty input", !parseAmount("", d));
    CHECK("TC-ROB-01 valid amount", parseAmount("250.75", d) && d == 250.75);
    CHECK("TC-ROB-01 PIN too short", !validPin("12"));
    CHECK("TC-ROB-01 PIN letters", !validPin("12ab"));
    CHECK("TC-ROB-01 name with delimiter", !validName("a|b"));
    CHECK("TC-ROB-01 over-long name", !validName(std::string(80, 'x')));

    // ---- Security: PIN never stored or logged in plain text ----
    std::string acctFile = slurp("testdata/accounts.dat"), audit = slurp("testdata/audit.log"), ledger = slurp("testdata/txns.dat");
    CHECK("TC-SEC-02 no plain-text PIN in accounts file", acctFile.find("|1234|") == std::string::npos && acctFile.find("|4321|") == std::string::npos);
    CHECK("TC-SEC-02 no PIN or hash in audit log", audit.find(findAccount(accts, a1)->pinHash) == std::string::npos);
    CHECK("TC-SEC-02 no PIN or hash in transaction history", ledger.find(findAccount(accts, a1)->pinHash) == std::string::npos);
    CHECK("Audit log records the block", audit.find("ACCOUNT_BLOCKED") != std::string::npos);

    std::filesystem::remove_all("testdata");
    std::cout << "\n" << (total - failures) << "/" << total << " passed\n";
    return failures ? 1 : 0;
}

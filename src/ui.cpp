#include "ui.h"
#include "account.h"
#include "admin.h"
#include "auth.h"
#include "history.h"
#include "transaction.h"
#include "transfer.h"
#include "validate.h"
#include <iostream>
#include <iomanip>
#include <string>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

static bool readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    if (!std::getline(std::cin, out)) return false;      // EOF -> exit cleanly
    while (!out.empty() && (out.back() == '\r' || out.back() == ' ')) out.pop_back();
    return true;
}

static void showErr(Err e) { std::cout << "  [!] " << errMsg(e) << "\n"; }

// Masked PIN/password entry (BMS-SR-002). Falls back to plain input when stdin is not a terminal.
static bool readPin(const std::string& prompt, std::string& out) {
    std::cout << prompt << std::flush;
#ifdef _WIN32
    out.clear();
    int c;
    while ((c = _getch()) != '\r' && c != '\n') {
        if (c == 3) return false;
        if (c == '\b') { if (!out.empty()) { out.pop_back(); std::cout << "\b \b"; } }
        else if (c >= 32 && c < 127 && out.size() < 64) { out.push_back((char)c); std::cout << '*'; }
    }
    std::cout << "\n";
    return true;
#else
    termios oldt;
    if (!isatty(STDIN_FILENO) || tcgetattr(STDIN_FILENO, &oldt) != 0) {
        if (!std::getline(std::cin, out)) return false;
    } else {
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        bool ok = (bool)std::getline(std::cin, out);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        std::cout << "\n";
        if (!ok) return false;
    }
    while (!out.empty() && (out.back() == '\r' || out.back() == ' ')) out.pop_back();
    return true;
#endif
}

static bool confirm(const std::string& question) {
    std::string a;
    if (!readLine(question + " (y/n): ", a)) return false;
    return a == "y" || a == "Y";
}

static void printAccount(const Account& a) {
    std::cout << "  Account number : " << a.number << "\n  Name           : " << a.name
              << "\n  Contact        : " << a.contact << "\n  Status         : " << (a.active ? "Active" : "Inactive")
              << "\n  Current balance: " << std::fixed << std::setprecision(2) << a.balance << "\n";
}

static void createFlow(std::vector<Account>& accts) {
    std::string name, contact, pin, amt;
    if (!readLine("Full name: ", name)) return;
    if (!validName(name)) { std::cout << "  [!] Name must be 1-50 characters and cannot contain '|'\n"; return; }
    if (!readLine("Contact number (10-15 digits): ", contact)) return;
    if (!validContact(contact)) { std::cout << "  [!] Contact must be 10-15 digits\n"; return; }
    if (!readPin("Choose a PIN (4-6 digits): ", pin)) return;
    if (!validPin(pin)) { std::cout << "  [!] PIN must be 4-6 digits\n"; return; }
    if (!readLine("Initial deposit (0 or more): ", amt)) return;
    double d = 0;
    if (amt != "0" && !parseAmount(amt, d)) { showErr(Err::INVALID_AMOUNT); return; }
    int accNo;
    Err e = createAccount(accts, name, contact, pin, d, accNo);
    if (e != Err::OK) { showErr(e); return; }
    std::cout << "  Account created. Your account number is " << accNo << "\n";
}

static void historyFlow(int accNo) {                                     // BMS-F-020
    std::vector<LedgerEntry> h = getHistory(accNo);
    if (h.empty()) { std::cout << "  No transactions yet.\n"; return; }
    std::cout << "  " << std::left << std::setw(9) << "ID" << std::setw(21) << "Date/Time" << std::setw(22) << "Type"
              << std::right << std::setw(12) << "Amount" << std::setw(14) << "Balance" << "\n";
    for (const auto& e : h)
        std::cout << "  " << std::left << std::setw(9) << e.txnId << std::setw(21) << e.dateTime << std::setw(22) << e.type
                  << std::right << std::fixed << std::setprecision(2) << std::setw(12) << e.amount << std::setw(14) << e.balanceAfter << "\n";
}

static void updateFlow(std::vector<Account>& accts, Account* acc) {      // BMS-F-008
    std::string name, contact;
    std::cout << "  Leave blank to keep the current value.\n";
    if (!readLine("New name [" + acc->name + "]: ", name)) return;
    if (!readLine("New contact [" + acc->contact + "]: ", contact)) return;
    if (name.empty()) name = acc->name;
    if (contact.empty()) contact = acc->contact;
    Err e = updateDetails(accts, acc->number, name, contact);
    if (e != Err::OK) { showErr(e); return; }
    std::cout << "  Details updated.\n";
}

static void transferFlow(std::vector<Account>& accts, int accNo) {       // BMS-F-016 .. F-019
    std::string toStr, amt;
    if (!readLine("Destination account number: ", toStr)) return;
    int to;
    if (!parseInt(toStr, to)) { std::cout << "  [!] Account number must be numeric\n"; return; }
    if (!readLine("Amount: ", amt)) return;
    double d;
    if (!parseAmount(amt, d)) { showErr(Err::INVALID_AMOUNT); return; }
    if (!confirm("Transfer " + amt + " to account " + toStr + "?")) { std::cout << "  Cancelled.\n"; return; }
    TxnResult r = transfer(accts, accNo, to, d);
    if (r.status != Err::OK) { showErr(r.status); return; }
    std::cout << "  Transferred. New balance: " << std::fixed << std::setprecision(2) << r.newBalance << "\n";
}

static void accountMenu(std::vector<Account>& accts, Account* acc) {
    int accNo = acc->number;
    std::cout << "\nWelcome, " << acc->name << "\n";
    std::string choice, amt;
    while (true) {
        std::cout << "\n--- Account " << accNo << " ---\n1. Deposit\n2. Withdraw\n3. View account details and balance\n"
                     "4. Transaction history\n5. Update details\n6. Transfer funds\n7. Logout\n";
        if (!readLine("Choice: ", choice)) return;
        if (choice == "1" || choice == "2") {
            if (!readLine("Amount: ", amt)) return;
            double d;
            if (!parseAmount(amt, d)) { showErr(Err::INVALID_AMOUNT); continue; }
            TxnResult r = (choice == "1") ? deposit(accts, accNo, d) : withdraw(accts, accNo, d);
            if (r.status != Err::OK) showErr(r.status);
            else std::cout << "  " << (choice == "1" ? "Deposited" : "Withdrew") << " " << std::fixed
                           << std::setprecision(2) << d << ". New balance: " << r.newBalance << "\n";
        } else if (choice == "3") {
            TxnResult r = balanceInquiry(accts, accNo);
            if (r.status != Err::OK) showErr(r.status); else printAccount(*acc);
        } else if (choice == "4") historyFlow(accNo);
        else if (choice == "5") updateFlow(accts, acc);
        else if (choice == "6") transferFlow(accts, accNo);
        else if (choice == "7") { std::cout << "  Logged out.\n"; return; }
        else std::cout << "  [!] Enter 1-7\n";
    }
}

static void loginFlow(std::vector<Account>& accts) {
    std::string accStr, pin;
    if (!readLine("Account number: ", accStr)) return;
    int accNo;
    if (!parseInt(accStr, accNo)) { std::cout << "  [!] Account number must be numeric\n"; return; }
    if (!readPin("PIN: ", pin)) return;
    Account* acc = nullptr;
    Err e = authenticate(accts, accNo, pin, acc);
    if (e == Err::ACCOUNT_NOT_FOUND || e == Err::INVALID_PIN) {
        std::cout << "  [!] Invalid account number or PIN\n";            // same message for both (BMS-F-002)
        return;
    }
    if (e != Err::OK) { showErr(e); return; }
    accountMenu(accts, acc);
}

static void adminMenu(std::vector<Account>& accts) {
    std::string choice, numStr;
    while (true) {
        std::cout << "\n--- Administrator ---\n1. Search account\n2. Deactivate account\n3. Logout\n";
        if (!readLine("Choice: ", choice)) return;
        if (choice == "1" || choice == "2") {
            if (!readLine("Account number: ", numStr)) return;
            int n;
            if (!parseInt(numStr, n)) { std::cout << "  [!] Account number must be numeric\n"; continue; }
            if (choice == "1") {                                            // BMS-F-021
                Account* a = searchAccount(accts, n);
                if (!a) showErr(Err::ACCOUNT_NOT_FOUND); else printAccount(*a);
            } else {                                                        // BMS-F-022
                if (!searchAccount(accts, n)) { showErr(Err::ACCOUNT_NOT_FOUND); continue; }
                if (!confirm("Deactivate account " + numStr + "?")) { std::cout << "  Cancelled.\n"; continue; }
                Err e = deactivateAccount(accts, n);
                if (e != Err::OK) showErr(e); else std::cout << "  Account deactivated.\n";
            }
        } else if (choice == "3") { std::cout << "  Logged out.\n"; return; }
        else std::cout << "  [!] Enter 1-3\n";
    }
}

static void adminLoginFlow(std::vector<Account>& accts) {                  // BMS-F-004
    std::string user, pw;
    if (!readLine("Administrator user: ", user)) return;
    if (!readPin("Password: ", pw)) return;
    if (!adminLogin(user, pw)) { std::cout << "  [!] Invalid administrator credentials\n"; return; }
    adminMenu(accts);
}

void runApp(std::vector<Account>& accts) {
    std::string choice;
    while (true) {
        std::cout << "\n=== Bank Management System ===\n1. Create account\n2. Customer login\n3. Administrator login\n4. Exit\n";
        if (!readLine("Choice: ", choice)) return;
        if (choice == "1") createFlow(accts);
        else if (choice == "2") loginFlow(accts);
        else if (choice == "3") adminLoginFlow(accts);
        else if (choice == "4") { std::cout << "Goodbye.\n"; return; }
        else std::cout << "  [!] Enter 1-4\n";
    }
}

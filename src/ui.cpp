#include "ui.h"
#include "account.h"
#include "auth.h"
#include "transaction.h"
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

// Masked PIN entry (test plan 5.1: PIN never echoed where masking is supported).
// Falls back to plain input when stdin is not a terminal (piped/scripted runs).
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

static void accountMenu(std::vector<Account>& accts, Account* acc) {
    int accNo = acc->number;
    std::cout << "\nWelcome, " << acc->name << "\n";
    std::string choice, amt;
    while (true) {
        std::cout << "\n--- Account " << accNo << " ---\n1. Deposit\n2. Withdraw\n3. View account details and balance\n4. Logout\n";
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
            if (r.status != Err::OK) { showErr(r.status); continue; }
            std::cout << "  Account number : " << acc->number << "\n  Name           : " << acc->name
                      << "\n  Contact        : " << acc->contact << "\n  Status         : "
                      << (acc->active ? "Active" : "Inactive") << "\n  Current balance: " << std::fixed
                      << std::setprecision(2) << r.newBalance << "\n";
        } else if (choice == "4") { std::cout << "  Logged out.\n"; return; }
        else std::cout << "  [!] Enter 1-4\n";
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
        std::cout << "  [!] Invalid account number or PIN\n";   // same message for both
        return;
    }
    if (e != Err::OK) { showErr(e); return; }
    accountMenu(accts, acc);
}

void runApp(std::vector<Account>& accts) {
    std::string choice;
    while (true) {
        std::cout << "\n=== Bank Management System ===\n1. Create account\n2. Login\n3. Exit\n";
        if (!readLine("Choice: ", choice)) return;
        if (choice == "1") createFlow(accts);
        else if (choice == "2") loginFlow(accts);
        else if (choice == "3") { std::cout << "Goodbye.\n"; return; }
        else std::cout << "  [!] Enter 1-3\n";
    }
}

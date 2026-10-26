#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;


// ================= BANK ACCOUNT BASE CLASS =================

class BankAccount
{
private:
    long long accountNumber;
    string customerName;
    string pin;
    double balance;
    bool active;

protected:
    string accountType;
    vector<string> transactionHistory;

public:

    BankAccount(long long accNo, string name, string p, string type)
    {
        accountNumber = accNo;
        customerName = name;
        pin = p;
        accountType = type;
        balance = 0.0;
        active = true;
    }

    virtual ~BankAccount()
    {
    }

    long long getAccountNumber()
    {
        return accountNumber;
    }

    string getCustomerName()
    {
        return customerName;
    }

    bool verifyPIN(string enteredPin)
    {
        return pin == enteredPin;
    }

    bool isActive()
    {
        return active;
    }

    // ================= DEPOSIT =================

    void deposit(double amount)
    {
        if (!active)
        {
            cout << "Account is closed.\n";
            return;
        }

        if (amount <= 0)
        {
            cout << "Invalid deposit amount.\n";
            return;
        }

        balance = balance + amount;

        stringstream ss;
        ss << fixed << setprecision(2) << amount;

        transactionHistory.push_back(
            "Deposit : Rs. " + ss.str()
        );

        cout << "Amount deposited successfully.\n";
    }


    // ================= WITHDRAWAL =================

    void withdraw(double amount)
    {
        if (!active)
        {
            cout << "Account is closed.\n";
            return;
        }

        if (amount <= 0)
        {
            cout << "Invalid withdrawal amount.\n";
            return;
        }

        if (amount > balance)
        {
            cout << "Insufficient balance.\n";
            return;
        }

        balance = balance - amount;

        stringstream ss;
        ss << fixed << setprecision(2) << amount;

        transactionHistory.push_back(
            "Withdrawal : Rs. " + ss.str()
        );

        cout << "Amount withdrawn successfully.\n";
    }


    // ================= BALANCE =================

    void showBalance()
    {
        cout << fixed << setprecision(2);
        cout << "Current Balance: Rs. "
             << balance << endl;
    }


    // ================= ACCOUNT DETAILS =================

    virtual void displayDetails()
    {
        cout << "\n----- Account Details -----\n";

        cout << "Account Number : "
             << accountNumber << endl;

        cout << "Customer Name  : "
             << customerName << endl;

        cout << "Account Type   : "
             << accountType << endl;

        cout << fixed << setprecision(2);

        cout << "Balance        : Rs. "
             << balance << endl;

        cout << "Status         : "
             << (active ? "Active" : "Closed")
             << endl;
    }


    // ================= TRANSACTION HISTORY =================

    void showTransactionHistory()
    {
        cout << "\n----- Transaction History -----\n";

        if (transactionHistory.empty())
        {
            cout << "No transactions available.\n";
            return;
        }

        for (int i = 0; i < transactionHistory.size(); i++)
        {
            cout << i + 1 << ". "
                 << transactionHistory[i]
                 << endl;
        }
    }


    // ================= ACCOUNT CLOSURE =================

    void closeAccount()
    {
        if (!active)
        {
            cout << "Account is already closed.\n";
            return;
        }

        active = false;

        transactionHistory.push_back("Account Closed");

        cout << "Account closed successfully.\n";
    }
};


// ================= SAVINGS ACCOUNT =================

class SavingsAccount : public BankAccount
{
public:

    SavingsAccount(long long accNo,
                   string name,
                   string pin)
        : BankAccount(accNo, name, pin, "Savings")
    {
    }

    void displayDetails() override
    {
        BankAccount::displayDetails();
    }
};


// ================= CURRENT ACCOUNT =================

class CurrentAccount : public BankAccount
{
public:

    CurrentAccount(long long accNo,
                   string name,
                   string pin)
        : BankAccount(accNo, name, pin, "Current")
    {
    }

    void displayDetails() override
    {
        BankAccount::displayDetails();
    }
};


// ================= BANK CLASS =================

class Bank
{
private:

    vector<BankAccount*> accounts;


    // ================= CHECK DUPLICATE ACCOUNT =================

    bool accountExists(long long accountNumber)
    {
        for (int i = 0; i < accounts.size(); i++)
        {
            if (accounts[i]->getAccountNumber()
                == accountNumber)
            {
                return true;
            }
        }

        return false;
    }


    // ================= GENERATE ACCOUNT NUMBER =================

    long long generateAccountNumber()
    {
        long long accountNumber;

        do
        {
            // 2026 + random 6 digits
            accountNumber =
                2026000000LL + rand() % 1000000;

        } while (accountExists(accountNumber));

        return accountNumber;
    }


public:

    // ================= DESTRUCTOR =================

    ~Bank()
    {
        for (int i = 0; i < accounts.size(); i++)
        {
            delete accounts[i];
        }
    }


    // ================= ACCOUNT CREATION =================

    void createAccount()
    {
        string name;
        string pin;
        int type;

        cout << "\n----- Account Creation -----\n";

        cout << "Enter Customer Name: ";

        cin.ignore();
        getline(cin, name);


        // ================= PIN =================

        cout << "Create 4-digit PIN: ";
        cin >> pin;

        if (pin.length() != 4)
        {
            cout << "PIN must contain exactly 4 digits.\n";
            return;
        }

        for (int i = 0; i < pin.length(); i++)
        {
            if (pin[i] < '0' || pin[i] > '9')
            {
                cout << "PIN must contain only digits.\n";
                return;
            }
        }


        // ================= ACCOUNT TYPE =================

        cout << "\nSelect Account Type:\n";

        cout << "1. Savings Account\n";
        cout << "2. Current Account\n";

        cout << "Enter choice: ";
        cin >> type;


        // ================= GENERATE ACCOUNT NUMBER =================

        long long accountNumber =
            generateAccountNumber();

        BankAccount* account;


        if (type == 1)
        {
            account = new SavingsAccount(
                accountNumber,
                name,
                pin
            );
        }
        else if (type == 2)
        {
            account = new CurrentAccount(
                accountNumber,
                name,
                pin
            );
        }
        else
        {
            cout << "Invalid account type.\n";
            return;
        }


        accounts.push_back(account);


        cout << "\n=================================\n";
        cout << "   ACCOUNT CREATED SUCCESSFULLY\n";
        cout << "=================================\n";

        cout << "Account Number : "
             << accountNumber << endl;

        cout << "Customer Name  : "
             << name << endl;

        cout << "Please remember your Account Number and PIN.\n";
    }


    // ================= LOGIN / VERIFICATION =================

    BankAccount* login()
    {
        long long accountNumber;
        string pin;

        cout << "\n----- Login / Verification -----\n";

        cout << "Enter Account Number: ";
        cin >> accountNumber;

        cout << "Enter PIN: ";
        cin >> pin;


        for (int i = 0; i < accounts.size(); i++)
        {
            if (accounts[i]->getAccountNumber()
                == accountNumber)
            {
                if (accounts[i]->verifyPIN(pin))
                {
                    if (!accounts[i]->isActive())
                    {
                        cout << "Account is closed.\n";
                        return NULL;
                    }

                    cout << "Login successful.\n";

                    return accounts[i];
                }
                else
                {
                    cout << "Incorrect PIN.\n";
                    return NULL;
                }
            }
        }

        cout << "Account not found.\n";

        return NULL;
    }


    // ================= DEPOSIT =================

    void depositMoney()
    {
        BankAccount* account = login();

        if (account != NULL)
        {
            double amount;

            cout << "Enter Deposit Amount: Rs. ";
            cin >> amount;

            account->deposit(amount);
        }
    }


    // ================= WITHDRAWAL =================

    void withdrawMoney()
    {
        BankAccount* account = login();

        if (account != NULL)
        {
            double amount;

            cout << "Enter Withdrawal Amount: Rs. ";
            cin >> amount;

            account->withdraw(amount);
        }
    }


    // ================= BALANCE ENQUIRY =================

    void balanceEnquiry()
    {
        BankAccount* account = login();

        if (account != NULL)
        {
            account->showBalance();
        }
    }


    // ================= ACCOUNT DETAILS =================

    void accountDetails()
    {
        BankAccount* account = login();

        if (account != NULL)
        {
            account->displayDetails();
        }
    }


    // ================= TRANSACTION HISTORY =================

    void transactionHistory()
    {
        BankAccount* account = login();

        if (account != NULL)
        {
            account->showTransactionHistory();
        }
    }


    // ================= ACCOUNT CLOSURE =================

    void accountClosure()
    {
        BankAccount* account = login();

        if (account != NULL)
        {
            account->closeAccount();
        }
    }
};


// ================= MAIN FUNCTION =================

int main()
{
    // Initialize random number generator
    srand(time(0));

    Bank bank;

    int choice;

    do
    {
        cout << "\n=================================\n";
        cout << "       BANK ACCOUNT SYSTEM\n";
        cout << "=================================\n";

        cout << "1. Account Creation\n";
        cout << "2. Login / Verification\n";
        cout << "3. Deposit\n";
        cout << "4. Withdrawal\n";
        cout << "5. Balance Enquiry\n";
        cout << "6. Account Details\n";
        cout << "7. Transaction History\n";
        cout << "8. Account Closure\n";
        cout << "9. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;


        switch (choice)
        {
        case 1:
            bank.createAccount();
            break;

        case 2:
            bank.login();
            break;

        case 3:
            bank.depositMoney();
            break;

        case 4:
            bank.withdrawMoney();
            break;

        case 5:
            bank.balanceEnquiry();
            break;

        case 6:
            bank.accountDetails();
            break;

        case 7:
            bank.transactionHistory();
            break;

        case 8:
            bank.accountClosure();
            break;

        case 9:
            cout << "\nThank you for using the Bank Account System.\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 9);

    return 0;
}
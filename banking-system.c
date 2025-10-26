#include<stdio.h>
int main()
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 1000
#define MIN_PASSWORD_LEN 8
#define MAX_PASSWORD_LEN 20
#define MIN_PHONE_LEN 10
#define MAX_PHONE_LEN 15
#define MIN_DEPOSIT 100.0
#define MAX_LOAN_AMOUNT 100000.0
#define LOAN_INTEREST_RATE 0.05f
#define MAX_LOGIN_ATTEMPTS 3

// Global interest rate variable (initially set to 3%)
float INTEREST_RATE = 0.03f;

// Structure to hold account details
typedef struct {
    int accountNumber;
    char name[50];
    char address[100];
    char phone[15];
    float balance;
    float loanAmount;
    float interest;
    char password[20];
    time_t creationDate;
    int loginAttempts;
    time_t lastLogin;
} Account;

// Structure to hold transaction details
typedef struct {
    int accountNumber;
    char type[20];
    float amount;
    time_t timestamp;
} Transaction;

// Structure to hold interest rate history
typedef struct {
    float rate;
    time_t timestamp;
} InterestRateHistory;

// Structure to hold login history
typedef struct {
    int accountNumber;
    char status[20]; // "Success" or "Failed"
    time_t timestamp;
} LoginHistory;

Account accounts[MAX_ACCOUNTS];
Transaction transactions[MAX_TRANSACTIONS];
InterestRateHistory rateHistory[100];
LoginHistory loginHistory[1000];
int accountCount = 0;
int transactionCount = 0;
int rateHistoryCount = 0;
int loginHistoryCount = 0;

// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Helper function to validate string input
int isValidString(const char* str, int maxLen) {
    int len = strlen(str);
    if (len == 0 || len >= maxLen) return 0;
    for (int i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

// Function to validate phone number
int isValidPhone(const char* phone) {
    int len = strlen(phone);
    if (len < MIN_PHONE_LEN || len > MAX_PHONE_LEN) {
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i])) {
            return 0;
        }
    }
    return 1;
}

// Function to validate password strength
int isStrongPassword(const char* password) {
    int len = strlen(password);
    if (len < MIN_PASSWORD_LEN || len > MAX_PASSWORD_LEN) {
        return 0;
    }
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    for (int i = 0; i < len; i++) {
        if (isupper(password[i])) hasUpper = 1;
        else if (islower(password[i])) hasLower = 1;
        else if (isdigit(password[i])) hasDigit = 1;
        else hasSpecial = 1;
    }
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

// Function to generate unique account number
int generateAccountNumber() {
    static int base = 1000;
    return base + accountCount;
}

// Function to pause and wait for user input
void waitForEnter() {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
    getchar();
}

// Function to get current timestamp as string
char* getTimeString(time_t timestamp) {
    static char timeStr[26];
    char* result = ctime(&timestamp);
    strncpy(timeStr, result, 26);
    timeStr[24] = '\0'; // Remove newline
    return timeStr;
}

// Custom function to parse date in YYYY-MM-DD format
int parseDate(const char* dateStr, struct tm* tm) {
    int year, month, day;
    if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0; // Invalid format
    }
    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0; // Invalid date
    }
    tm->tm_year = year - 1900;
    tm->tm_mon = month - 1;
    tm->tm_mday = day;
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = -1; // Let mktime determine DST
    return 1;
}

// Function to authenticate user credentials
int authenticate(int accNo, const char* password) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (accounts[i].loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                printf("Account is locked due to too many failed login attempts.\n");
                return -1;
            }
            if (strcmp(accounts[i].password, password) == 0) {
                accounts[i].loginAttempts = 0; // Reset attempts on success
                accounts[i].lastLogin = time(NULL);
                saveAccountsToFile();
                return i;
            } else {
                accounts[i].loginAttempts++;
                saveAccountsToFile();
                return -1;
            }
        }
    }
    return -1;
}

// Function to log login attempts
void logLoginAttempt(int accNo, const char* status) {
    if (loginHistoryCount >= 1000) return;
    LoginHistory lh;
    lh.accountNumber = accNo;
    strncpy(lh.status, status, sizeof(lh.status) - 1);
    lh.status[sizeof(lh.status) - 1] = '\0';
    lh.timestamp = time(NULL);
    loginHistory[loginHistoryCount++] = lh;

    FILE* fp = fopen("login_history.txt", "a");
    if (fp == NULL) return;
    fprintf(fp, "Acc#%d | %s | %s\n", accNo, status, getTimeString(lh.timestamp));
    fclose(fp);
}

// Function to save accounts to file
void saveAccountsToFile() {
    FILE* fp = fopen("accounts.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open accounts file for writing.\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, fp);
    fwrite(accounts, sizeof(Account), accountCount, fp);
    fwrite(&INTEREST_RATE, sizeof(float), 1, fp);
    fwrite(&rateHistoryCount, sizeof(int), 1, fp);
    fwrite(rateHistory, sizeof(InterestRateHistory), rateHistoryCount, fp);
    fclose(fp);
}

// Function to load accounts from file
void loadAccountsFromFile() {
    FILE* fp = fopen("accounts.dat", "rb");
    if (fp != NULL) {
        fread(&accountCount, sizeof(int), 1, fp);
        if (accountCount > MAX_ACCOUNTS) {
            printf("Error: Account count exceeds maximum limit.\n");
            accountCount = 0;
            fclose(fp);
            return;
        }
        fread(accounts, sizeof(Account), accountCount, fp);
        fread(&INTEREST_RATE, sizeof(float), 1, fp);
        fread(&rateHistoryCount, sizeof(int), 1, fp);
        if (rateHistoryCount > 100) rateHistoryCount = 0;
        fread(rateHistory, sizeof(InterestRateHistory), rateHistoryCount, fp);
        fclose(fp);
    }
}

// Function to log a transaction
void logTransaction(int accNo, const char* type, float amount) {
    if (transactionCount >= MAX_TRANSACTIONS) {
        printf("Error: Transaction limit reached.\n");
        return;
    }
    Transaction t;
    t.accountNumber = accNo;
    strncpy(t.type, type, sizeof(t.type) - 1);
    t.type[sizeof(t.type) - 1] = '\0';
    t.amount = amount;
    t.timestamp = time(NULL);
    transactions[transactionCount++] = t;

    FILE* fp = fopen("transactions.txt", "a");
    if (fp == NULL) {
        printf("Error: Cannot open transactions file.\n");
        return;
    }
    fprintf(fp, "Acc#%d | %s | %.2f | %s\n", accNo, type, amount, getTimeString(t.timestamp));
    fclose(fp);
}

// Function to filter transactions by date
void filterTransactionsByDate(int accNo, time_t start, time_t end) {
    FILE* fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("No transaction history found.\n");
        waitForEnter();
        return;
    }
    printf("\n--- Transactions for Acc#%d from %s to %s ---\n", accNo, getTimeString(start), getTimeString(end));
    char line[200];
    char accStr[20];
    sprintf(accStr, "Acc#%d", accNo);
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, accStr)) {
            char* timestampStr = strrchr(line, '|');
            if (timestampStr) {
                timestampStr += 2; // Skip "| "
                // Parse the timestamp manually
                struct tm tm = {0};
                // Expected format: "Wed Apr 12 14:30:00 2023"
                int day, year;
                char monthStr[4];
                int hour, min, sec;
                if (sscanf(timestampStr, "%*s %s %d %d:%d:%d %d", monthStr, &day, &hour, &min, &sec, &year) != 6) {
                    continue; // Skip invalid timestamp
                }
                tm.tm_year = year - 1900;
                tm.tm_mday = day;
                tm.tm_hour = hour;
                tm.tm_min = min;
                tm.tm_sec = sec;
                // Map month string to number
                if (strcmp(monthStr, "Jan") == 0) tm.tm_mon = 0;
                else if (strcmp(monthStr, "Feb") == 0) tm.tm_mon = 1;
                else if (strcmp(monthStr, "Mar") == 0) tm.tm_mon = 2;
                else if (strcmp(monthStr, "Apr") == 0) tm.tm_mon = 3;
                else if (strcmp(monthStr, "May") == 0) tm.tm_mon = 4;
                else if (strcmp(monthStr, "Jun") == 0) tm.tm_mon = 5;
                else if (strcmp(monthStr, "Jul") == 0) tm.tm_mon = 6;
                else if (strcmp(monthStr, "Aug") == 0) tm.tm_mon = 7;
                else if (strcmp(monthStr, "Sep") == 0) tm.tm_mon = 8;
                else if (strcmp(monthStr, "Oct") == 0) tm.tm_mon = 9;
                else if (strcmp(monthStr, "Nov") == 0) tm.tm_mon = 10;
                else if (strcmp(monthStr, "Dec") == 0) tm.tm_mon = 11;
                else continue; // Invalid month
                time_t txTime = mktime(&tm);
                if (txTime >= start && txTime <= end) {
                    printf("%s", line);
                    found = 1;
                }
            }
        }
    }
    if (!found) {
        printf("No transactions found in this date range.\n");
    }
    fclose(fp);
    waitForEnter();
}

// Function to filter transactions by type
void filterTransactionsByType(int accNo, const char* type) {
    FILE* fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("No transaction history found.\n");
        waitForEnter();
        return;
    }
    printf("\n--- %s Transactions for Acc#%d ---\n", type, accNo);
    char line[200];
    char accStr[20];
    sprintf(accStr, "Acc#%d", accNo);
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, accStr) && strstr(line, type)) {
            printf("%s", line);
            found = 1;
        }
    }
    if (!found) {
        printf("No %s transactions found.\n", type);
    }
    fclose(fp);
    waitForEnter();
}

// Function to show transaction summary
void showTransactionSummary(int accNo) {
    FILE* fp = fopen("transactions.txt", "r");
    if (!fp) {
        printf("No transaction history found.\n");
        waitForEnter();
        return;
    }
    float totalDeposits = 0, totalWithdrawals = 0, totalInterest = 0, totalLoans = 0, totalRepayments = 0;
    int depositCount = 0, withdrawalCount = 0, interestCount = 0, loanCount = 0, repaymentCount = 0;
    char line[200];
    char accStr[20];
    sprintf(accStr, "Acc#%d", accNo);

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, accStr)) {
            float amount;
            sscanf(line, "Acc#%*d | %*s | %f |", &amount);
            if (strstr(line, "Deposit")) {
                totalDeposits += amount;
                depositCount++;
            } else if (strstr(line, "Withdraw")) {
                totalWithdrawals += amount;
                withdrawalCount++;
            } else if (strstr(line, "Interest Applied")) {
                totalInterest += amount;
                interestCount++;
            } else if (strstr(line, "Loan Taken")) {
                totalLoans += amount;
                loanCount++;
            } else if (strstr(line, "Loan Repayment")) {
                totalRepayments += amount;
                repaymentCount++;
            }
        }
    }

    printf("\n--- Transaction Summary for Acc#%d ---\n", accNo);
    printf("Total Deposits    : %.2f (%d transactions)\n", totalDeposits, depositCount);
    printf("Total Withdrawals : %.2f (%d transactions)\n", totalWithdrawals, withdrawalCount);
    printf("Total Interest    : %.2f (%d applications)\n", totalInterest, interestCount);
    printf("Total Loans Taken : %.2f (%d loans)\n", totalLoans, loanCount);
    printf("Total Repayments  : %.2f (%d repayments)\n", totalRepayments, repaymentCount);
    fclose(fp);
    waitForEnter();
}


// Function to close an account
void closeAccount(int index) {
    if (accounts[index].loanAmount > 0) {
        printf("Error: Cannot close account with outstanding loan of %.2f.\n", accounts[index].loanAmount);
        waitForEnter();
        return;
    }
    int accNo = accounts[index].accountNumber;
    for (int i = index; i < accountCount - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    accountCount--;
    saveAccountsToFile();
    logTransaction(accNo, "Account Closed", 0.0);
    printf("Account %d has been closed.\n", accNo);
    waitForEnter();
}

// Function to view transaction history
void viewHistory(int index) {
    showTransactionHistory(accounts[index].accountNumber);
}

// Function to display logged-in user menu
void loggedInMenu(int index) {
    int choice;
    do {
        printf("\n=== Welcome %s (Acc# %d) ===\n", accounts[index].name, accounts[index].accountNumber);
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. Check Balance\n");
        printf("4. Update Details\n");
        printf("5. View Transaction History\n");
        printf("6. Loan Management\n");
        printf("7. Close Account\n");
        printf("8. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: depositMoney(index); break;
            case 2: withdrawMoney(index); break;
            case 3: checkBalance(index); break;
            case 4: updateDetails(index); break;
            case 5: viewHistory(index); break;
            case 6: loanSection(index); break;
            case 7: closeAccount(index); return;
            case 8: printf("Logging out...\n"); waitForEnter(); break;
            default: printf("Invalid option.\n"); waitForEnter();
        }
    } while (choice != 8);
}

// Function to handle admin login
int adminLogin() {
    char username[20], password[20];
    printf("\n--- Admin Login ---\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        logLoginAttempt(0, "Admin Success");
        printf("Admin login successful.\n");
        waitForEnter();
        return 1;
    }
    logLoginAttempt(0, "Admin Failed");
    printf("Invalid admin credentials.\n");
    waitForEnter();
    return 0;
}

// Function to show all accounts
void showAllAccounts() {
    printf("\n--- All Bank Accounts ---\n");
    if (accountCount == 0) {
        printf("No accounts available.\n");
    } else {
        for (int i = 0; i < accountCount; i++) {
            Account a = accounts[i];
            printf("\nAccount No: %d\n", a.accountNumber);
            printf("Name      : %s\n", a.name);
            printf("Address   : %s\n", a.address);
            printf("Phone     : %s\n", a.phone);
            printf("Balance   : %.2f\n", a.balance);
            printf("Loan Amt  : %.2f\n", a.loanAmount);
            printf("Created   : %s\n", getTimeString(a.creationDate));
            printf("Last Login: %s\n", a.lastLogin ? getTimeString(a.lastLogin) : "Never");
        }
    }
    waitForEnter();
}

// Function to remove an account by admin
void removeAccountByAdmin() {
    int accNo;
    printf("\nEnter Account Number to Remove: ");
    scanf("%d", &accNo);

    int found = 0;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (accounts[i].loanAmount > 0) {
                printf("Error: Cannot remove account with outstanding loan of %.2f.\n", accounts[i].loanAmount);
            } else {
                for (int j = i; j < accountCount - 1; j++) {
                    accounts[j] = accounts[j + 1];
                }
                accountCount--;
                saveAccountsToFile();
                logTransaction(accNo, "Account Removed by Admin", 0.0);
                printf("Account %d removed successfully.\n", accNo);
            }
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Account not found.\n");
    }
    waitForEnter();
}

// Function to view login history
void viewLoginHistory() {
    FILE* fp = fopen("login_history.txt", "r");
    if (!fp) {
        printf("No login history found.\n");
        waitForEnter();
        return;
    }
    printf("\n--- Login History ---\n");
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
    waitForEnter();
}

// Function to display admin menu
void adminMenu() {
    int choice;
    do {
        printf("\n=== Admin Panel ===\n");
        printf("1. Show All Accounts\n");
        printf("2. Remove an Account\n");
        printf("3. Apply Interest to All Accounts\n");
        printf("4. Set Interest Rate\n");
        printf("5. View Interest Rate History\n");
        printf("6. View Login History\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: showAllAccounts(); break;
            case 2: removeAccountByAdmin(); break;
            case 3: applyInterest(); break;
            case 4: setInterestRate(); break;
            case 5: viewInterestRateHistory(); break;
            case 6: viewLoginHistory(); break;
            case 7: printf("Logging out of admin...\n"); waitForEnter(); break;
            default: printf("Invalid choice.\n"); waitForEnter();
        }
    } while (choice != 7);
}

// Function to display main menu
void mainMenu() {
    int choice;
    do {
        printf("\n=== Bank Account Management System ===\n");
        printf("1. Create New Account\n");
        printf("2. Login to Account\n");
        printf("3. Admin Login\n");
        printf("4. Recover Password\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: {
                int index = login();
                if (index != -1) {
                    loggedInMenu(index);
                }
                break;
            }
            case 3:
                if (adminLogin()) {
                    adminMenu();
                }
                break;
            case 4: recoverPassword(); break;
            case 5: printf("Exiting system...\n"); waitForEnter(); break;
            default: printf("Invalid choice.\n"); waitForEnter();
        }
    } while (choice != 5);
}

// Main function
int main() {
    printf("Initializing Bank Account Management System...\n");
    loadAccountsFromFile();
    mainMenu();
    printf("System shutdown complete.\n");
    return 0;
}



    return 0;
}
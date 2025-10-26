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
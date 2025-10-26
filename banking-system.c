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
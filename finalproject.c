#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct
{
    int month;
    int year;
} Date;

typedef struct
{
    char accnumber[20];
    char name[100];
    char address[100];
    double balance;
    char mobile[15];
    Date dateopen;
    char status[50];
} accInfo;

// Function prototypes
Date getCurrentDate();
char *monthname(int month);
int LOGIN();
void loadAccounts(accInfo bank_accounts[], int *n);
void search(accInfo acc[], int n);
void advanced_search(accInfo acc[], int n);
void recordTransaction(char *accnumber, char *type, double amount, double new_balance);
int isDuplicate(accInfo bank_accounts[], int n, char *account_num);
int askToSave();                                // Added prototype
void SAVE(accInfo accounts[], int numAccounts); // Added prototype
void Add_Acc(accInfo bank_accounts[], int *n);
void Delete_Acc(accInfo bank_accounts[], int *n);
void Modify_Acc(accInfo bank_accounts[], int *n);
void CHANGE_STATUS(accInfo acc[], int n);
int dailyLimit(double withdrawnToday, double amount);
void WITHDRAW(accInfo acc[], int n, double *withdrawnToday);
void deposit(accInfo acc[], int n);
void transfer(accInfo acc[], int n);
void Report(char accnumber[]);
void QUIT();
void initialMenu();
void mainMenu(accInfo accounts[], int *numAccounts, double *withdrawnToday); // Added prototype

// Global variable declaration (needed for functions that reference it)
// Alternatively, we can pass it as parameter - I'll fix by removing references to bank_accounts in functions
// and using the correct parameter names

Date getCurrentDate()
{
    Date current;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    current.year = tm.tm_year + 1900;
    current.month = tm.tm_mon + 1;
    return current;
}

char *monthname(int month)
{
    switch (month)
    {
    case 1:
        return "January";
    case 2:
        return "February";
    case 3:
        return "March";
    case 4:
        return "April";
    case 5:
        return "May";
    case 6:
        return "June";
    case 7:
        return "July";
    case 8:
        return "August";
    case 9:
        return "September";
    case 10:
        return "October";
    case 11:
        return "November";
    case 12:
        return "December";
    default:
        return "unknown";
    }
}

int LOGIN()
{
    char u[50], p[50], username[50], pass[50], *token, buffer[100];
    FILE *ptr = fopen("users.txt", "r");
    if (ptr == NULL)
    {
        printf("Error Cannot open users.txt\n");
        return 0;
    }
    printf("Enter username: ");
    scanf("%49s", username);
    printf("Enter password: ");
    scanf("%49s", pass);
    while (fgets(buffer, sizeof(buffer), ptr) != NULL)
    {
        token = strtok(buffer, " \t\n");
        if (token != NULL)
            strcpy(u, token);

        token = strtok(NULL, " \t\n");
        if (token != NULL)
            strcpy(p, token);
        p[strcspn(p, "\n")] = '\0';
        if (strcmp(u, username) == 0 && strcmp(p, pass) == 0)
        {
            fclose(ptr);
            printf("Login successful\n");
            return 1;
        }
    }
    fclose(ptr);
    printf("Invalid username or password!\n");
    return 0;
}

void LOAD(accInfo bank_accounts[], int *n)
{
    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL)
    {
        printf("Error opening accounts.txt file.\n");
        exit(1);
    }
    char line[256];
    *n = 0;
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strlen(line) <= 1) // skip empty lines
            continue;

        line[strcspn(line, "\n")] = '\0'; // remove newline char

        int result = sscanf(line, "%19[^,],%99[^,],%99[^,],%lf,%14[^,],%d-%d,%49[^\n]",
                            bank_accounts[i].accnumber,
                            bank_accounts[i].name,
                            bank_accounts[i].address,
                            &bank_accounts[i].balance,
                            bank_accounts[i].mobile,
                            &bank_accounts[i].dateopen.month,
                            &bank_accounts[i].dateopen.year,
                            bank_accounts[i].status);
        if (result == 8)
        {
            char *status = bank_accounts[i].status;
            while (*status && (*status == ' ' || *status == '\t'))
            {
                status++; // points to first char instead of space
            }

            // If we found leading spaces, shift the string left
            if (status != bank_accounts[i].status)
            {
                strcpy(bank_accounts[i].status, status);
            }

            int len = strlen(bank_accounts[i].status);
            while (len > 0 && (bank_accounts[i].status[len - 1] == ' ' ||
                               bank_accounts[i].status[len - 1] == '\t'))
            {
                bank_accounts[i].status[len - 1] = '\0';
                len--;
            }

            i++;
        }
        else
        {
            printf("Warning: Skipping malformed line: %s\n", line);
        }
    }
    *n = i;
    fclose(file);
    printf("Successfully loaded %d accounts.\n", *n);
}
void search(accInfo acc[], int n)
{
    char search_number[20];
    printf("enter your account's number: ");
    scanf("%s", search_number);

    int found = 0;
    int i;

    for (i = 0; i < n; i++)
    {
        if (strcmp(search_number, acc[i].accnumber) == 0)
        {
            found = 1;

            printf("Account Number : %s\n", acc[i].accnumber);
            printf("Name: %s\n", acc[i].name);
            printf("Address : %s\n", acc[i].address);
            printf("Balance: %.2lf $\n", acc[i].balance);

            printf("Mobile: %s\n", acc[i].mobile);
            printf("Date Opened: %s %d\n", monthname(acc[i].dateopen.month), acc[i].dateopen.year);

            if (strcmp(acc[i].status, "active") == 0)
                printf("Status: Active\n");
            else
                printf("Status: Inactive\n");

            break;
        }
    }
    if (!found)
        printf("Couldn't find this account number\n");
}

void advanced_search(accInfo acc[], int n)
{
    char keyword[100];
    int found = 0;
    printf("Enter the account's name: ");
    scanf("%s", keyword);
    int i;
    for (i = 0; i < n; i++)
    {
        if (strstr(acc[i].name, keyword) != NULL)
        {
            printf("Account Number : %s\n", acc[i].accnumber);
            printf("Name: %s\n", acc[i].name);
            printf("Address : %s\n", acc[i].address);
            printf("Balance: %lf $\n", acc[i].balance);
            printf("Mobile: %s\n", acc[i].mobile);
            printf("Date Opened:%s %d\n", monthname(acc[i].dateopen.month), acc[i].dateopen.year);

            if (strcmp(acc[i].status, "active") == 0)
                printf("Status: active\n");
            else if (strcmp(acc[i].status, "inactive") == 0)
                printf("Status: Inactive\n");

            found = 1;
        }
    }
    if (!found)
        printf("Couldn't find this account name");
}

void recordTransaction(char *accnumber, char *type, double amount, double new_balance)
{
    char filename[50];
    sprintf(filename, "%s.txt", accnumber);
    FILE *file = fopen(filename, "a");
    if (file != NULL)
    {
        Date current_date = getCurrentDate();

        fprintf(file, "%04d-%02d %s %.2f Balance: %.2f\n",
                current_date.year,
                current_date.month,
                type,
                amount,
                new_balance);

        fclose(file);
    }
}

int isDuplicate(accInfo bank_accounts[], int n, char *account_num)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (strcmp(bank_accounts[i].accnumber, account_num) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Moved askToSave function before Add_Acc to fix implicit declaration
int askToSave()
{
    char choice;
    printf("\nDo you want to save changes? (y/n): ");
    scanf(" %c", &choice);
    return (choice == 'y' || choice == 'Y');
}

void SAVE(accInfo accounts[], int numAccounts)
{
    FILE *file = fopen("accounts.txt", "w");
    if (file == NULL)
    {
        printf("Error saving to accounts.txt file.\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++)
    {
        fprintf(file, "%s,%s,%s,%.2lf,%s,%d-%d,%s\n",
                accounts[i].accnumber,
                accounts[i].name,
                accounts[i].address,
                accounts[i].balance,
                accounts[i].mobile,
                accounts[i].dateopen.month,
                accounts[i].dateopen.year,
                accounts[i].status);
    }

    fclose(file);
    printf("All accounts saved successfully to accounts.txt\n");
}

void Add_Acc(accInfo bank_accounts[], int *n)
{
    accInfo new_account;
    printf("Please enter the account number: ");
    scanf("%s", new_account.accnumber);

    if (isDuplicate(bank_accounts, *n, new_account.accnumber))
    {
        printf("This account already exists\n");
        return;
    }

    getchar();
    printf("Please enter name: ");
    fgets(new_account.name, sizeof(new_account.name), stdin);
    new_account.name[strcspn(new_account.name, "\n")] = '\0';

    printf("Please enter address: ");
    fgets(new_account.address, sizeof(new_account.address), stdin);
    new_account.address[strcspn(new_account.address, "\n")] = '\0';

    printf("Please enter balance: ");
    scanf("%lf", &new_account.balance);

    printf("Please enter mobile number: ");
    scanf("%s", new_account.mobile);

    new_account.dateopen = getCurrentDate();
    strcpy(new_account.status, "active");

    bank_accounts[*n] = new_account;
    (*n)++;

    FILE *file = fopen("accounts.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%s,%s,%s,%.2lf,%s,%d-%d,%s\n",
                new_account.accnumber,
                new_account.name,
                new_account.address,
                new_account.balance,
                new_account.mobile,
                new_account.dateopen.month,
                new_account.dateopen.year,
                new_account.status);
        fclose(file);
        printf("Account added successfully!\n");
        if (askToSave())
        {
            SAVE(bank_accounts, *n);
        }
    }
    else
    {
        printf("Error opening accounts.txt\n");
    }
}

void Delete_Acc(accInfo bank_accounts[], int *n)
{
    char account_num[20];
    printf("Please enter the account number to delete: ");
    scanf("%s", account_num);

    int found = 0;

    for (int i = 0; i < *n; i++)
    {
        if (strcmp(bank_accounts[i].accnumber, account_num) == 0)
        {
            found = 1;

            if (bank_accounts[i].balance > 0)
            {
                printf("Cannot delete account - balance must be zero (Current balance: %.2lf)\n",
                       bank_accounts[i].balance);
                return;
            }

            for (int j = i; j < *n - 1; j++)
            {
                bank_accounts[j] = bank_accounts[j + 1];
            }

            (*n)--;

            if (askToSave())
            {
                SAVE(bank_accounts, *n);
            }
            else
            {
                // Just update the file anyway since we modified the array
                FILE *file = fopen("accounts.txt", "w");
                if (file != NULL)
                {
                    for (int k = 0; k < *n; k++)
                    {
                        fprintf(file, "%s,%s,%s,%.2lf,%s,%d-%d,%s\n",
                                bank_accounts[k].accnumber,
                                bank_accounts[k].name,
                                bank_accounts[k].address,
                                bank_accounts[k].balance,
                                bank_accounts[k].mobile,
                                bank_accounts[k].dateopen.month,
                                bank_accounts[k].dateopen.year,
                                bank_accounts[k].status);
                    }
                    fclose(file);
                    printf("Account deleted successfully!\n");
                }
                else
                {
                    printf("Error opening accounts.txt\n");
                }
            }
            return;
        }
    }

    if (!found)
    {
        printf("Account not found: %s\n", account_num);
    }
}

void Modify_Acc(accInfo bank_accounts[], int *n)
{
    char accnum[20];
    int i, found = 0;

    printf("Enter account number: ");
    scanf("%s", accnum);
    getchar();

    for (i = 0; i < *n; i++)
    {
        if (strcmp(bank_accounts[i].accnumber, accnum) == 0)
        {
            found = 1;
            printf("Current name: %s\n", bank_accounts[i].name);
            printf("Enter new name: ");
            fgets(bank_accounts[i].name, sizeof(bank_accounts[i].name), stdin);
            bank_accounts[i].name[strcspn(bank_accounts[i].name, "\n")] = '\0';

            printf("Current mobile: %s\n", bank_accounts[i].mobile);
            printf("Enter new mobile: ");
            fgets(bank_accounts[i].mobile, sizeof(bank_accounts[i].mobile), stdin);
            bank_accounts[i].mobile[strcspn(bank_accounts[i].mobile, "\n")] = '\0';

            printf("Current address: %s\n", bank_accounts[i].address);
            printf("Enter new address: ");
            fgets(bank_accounts[i].address, sizeof(bank_accounts[i].address), stdin);
            bank_accounts[i].address[strcspn(bank_accounts[i].address, "\n")] = '\0';

            break;
        }
    }

    if (!found)
    {
        printf("Account not found\n");
        return;
    }

    if (askToSave())
    {
        SAVE(bank_accounts, *n);
    }
    else
    {
        FILE *file = fopen("accounts.txt", "w");
        if (file == NULL)
        {
            printf("Error opening accounts.txt\n");
            return;
        }

        for (int h = 0; h < *n; h++)
        {
            fprintf(file, "%s,%s,%s,%.2lf,%s,%d-%d,%s\n",
                    bank_accounts[h].accnumber,
                    bank_accounts[h].name,
                    bank_accounts[h].address,
                    bank_accounts[h].balance,
                    bank_accounts[h].mobile,
                    bank_accounts[h].dateopen.month,
                    bank_accounts[h].dateopen.year,
                    bank_accounts[h].status);
        }

        fclose(file);
        printf("Account modified successfully\n");
    }
}

void CHANGE_STATUS(accInfo acc[], int n)
{
    char search_number[20], desired_status[50];
    printf("Enter your account's number: ");
    scanf("%s", search_number);
    printf("Enter the desired account status (active or inactive): ");
    scanf("%s", desired_status);

    int found = 0, i, index;

    for (i = 0; i < n; i++)
    {
        if (strcmp(acc[i].accnumber, search_number) == 0)
        {
            found = 1;
            index = i;
            break;
        }
    }

    if (!found)
    {
        printf("Couldn't find this account number!!\n");
        return;
    }

    // Simple comparison - no need for lowercase conversion
    if (strcmp(acc[index].status, desired_status) == 0)
    {
        printf("The account already has this status\n");
    }
    else
    {
        strcpy(acc[index].status, desired_status);

        if (askToSave())
        {
            SAVE(acc, n);
        }
        else
        {
            FILE *file = fopen("accounts.txt", "w");
            if (file != NULL)
            {
                for (int j = 0; j < n; j++)
                {
                    fprintf(file, "%s,%s,%s,%.2lf,%s,%d-%d,%s\n",
                            acc[j].accnumber, acc[j].name, acc[j].address,
                            acc[j].balance, acc[j].mobile, acc[j].dateopen.month,
                            acc[j].dateopen.year, acc[j].status);
                }
                fclose(file);
                printf("The status has been successfully changed to %s\n", desired_status);
            }
            else
            {
                printf("Error updating accounts file\n");
            }
        }
    }
}
int dailyLimit(double withdrawnToday, double amount)
{
    if (withdrawnToday + amount > 50000)
        return 0;
    return 1;
}

void WITHDRAW(accInfo acc[], int n, double *withdrawnToday)
{
    char accnumber[20];
    double amount;

    printf("\n Withdrawal Process \n");
    printf("Enter active account number: ");
    scanf("%s", accnumber);

    int found = 0, index = -1;

    for (int i = 0; i < n; i++)
    {
        if (strcmp(acc[i].accnumber, accnumber) == 0)
        {
            found = 1;
            index = i;
            break;
        }
    }

    if (!found)
    {
        printf("Error: Account not found\n");
        return;
    }

    if (strcmp(acc[index].status, "active") != 0)
    {
        printf("Warning: Account is inactive. Process cannot be completed\n");
        return;
    }

    printf("Enter withdrawal amount: ");
    if (scanf("%lf", &amount) != 1)
    {
        printf("Error: Invalid amount entered.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    if (amount <= 0)
    {
        printf("Error: Withdrawal amount must be positive\n");
        return;
    }

    if (amount > 10000)
    {
        printf("Error: Maximum withdrawal per transaction is 10,000$\n");
        return;
    }

    if (!dailyLimit(*withdrawnToday, amount))
    {
        printf("Error: Daily withdrawal limit exceeded (50,000$)\n");
        return;
    }

    if (amount > acc[index].balance)
    {
        printf("Error: Insufficient balance.\n");
        printf("Current balance: %.2f$\n", acc[index].balance);
        printf("Requested amount: %.2f$\n", amount);
        return;
    }

    acc[index].balance -= amount;
    *withdrawnToday += amount;

    recordTransaction(acc[index].accnumber, "withdraw", amount, acc[index].balance);

    printf("TRANSACTION SUCCESSFUL!\n");
    printf("Account: %s\n", acc[index].accnumber);
    printf("Withdrawn: %.2f$\n", amount);
    printf("New Balance: %.2f$\n", acc[index].balance);
    printf("Total withdrawn today: %.2f$\n", *withdrawnToday);

    if (askToSave())
    {
        SAVE(acc, n);
    }
}

void deposit(accInfo acc[], int n)
{
    char acc_num[20];
    double deposit_amount;
    printf("Enter your account number: ");
    scanf("%s", acc_num);
    printf("Enter the amount you want to deposit: ");
    scanf("%lf", &deposit_amount);
    if (deposit_amount > 10000)
    {
        printf("Deposit failed: Maximum deposit limit per transaction is 10,000$\n");
        return;
    }

    int i, found = 0, index;
    for (i = 0; i < n; i++)
    {
        if (strcmp(acc[i].accnumber, acc_num) == 0)
        {
            found = 1;
            index = i;
            break;
        }
    }

    if (found)
    {
        if (strcmp(acc[index].status, "active") != 0)
        {
            printf("Warning: This account is inactive. The process cannot be completed\n");
            return;
        }
        acc[index].balance += deposit_amount;
        printf("Deposit completed successfully\n");

        recordTransaction(acc[index].accnumber, "deposit", deposit_amount, acc[index].balance);
        if (askToSave())
        {
            SAVE(acc, n);
        }
    }
    else
    {
        printf("Error: Account not found\n");
    }
}

void transfer(accInfo acc[], int n)
{
    char sender_acc[20], receiver_acc[20];
    double amount;
    int sender_index = -1, receiver_index = -1;
    printf("Enter sender account number: ");
    scanf("%s", sender_acc);
    printf("Enter receiver account number: ");
    scanf("%s", receiver_acc);
    printf("Enter transfer amount: ");
    scanf("%lf", &amount);

    if (strcmp(sender_acc, receiver_acc) == 0)
    {
        printf("Error: Cannot transfer to the same account.\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (strcmp(acc[i].accnumber, sender_acc) == 0)
            sender_index = i;
        if (strcmp(acc[i].accnumber, receiver_acc) == 0)
            receiver_index = i;
    }

    if (sender_index == -1)
    {
        printf("Error: Sender account not found.\n");
        return;
    }
    if (strcmp(acc[sender_index].status, "active") != 0)
    {
        printf("Error: Sender account is inactive.\n");
        return;
    }

    if (receiver_index == -1)
    {
        printf("Error: Receiver account not found.\n");
        return;
    }
    if (strcmp(acc[receiver_index].status, "active") != 0)
    {
        printf("Error: Receiver account is inactive.\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Error: Amount must be positive.\n");
        return;
    }
    if (acc[sender_index].balance < amount)
    {
        printf("Error: Insufficient balance.\n");
        return;
    }

    acc[sender_index].balance -= amount;
    acc[receiver_index].balance += amount;

    recordTransaction(sender_acc, "transfer-out", amount, acc[sender_index].balance);
    recordTransaction(receiver_acc, "transfer-in", amount, acc[receiver_index].balance);

    printf("Transfer successful. New balance: $%.2f\n", acc[sender_index].balance);

    if (askToSave())
    {
        SAVE(acc, n);
    }
}

void Report(char accnumber[])
{
    char lines[100][100];
    char filename[20];
    int count = 0;
    int start, i;
    sprintf(filename, "%s.txt", accnumber);

    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("No transactions found for this account\n");
        return;
    }

    while (fgets(lines[count], sizeof(lines[count]), f))
    {
        count++;
    }

    fclose(f);

    if (count > 5)
    {
        start = count - 5;
    }
    else
    {
        start = 0;
    }

    printf("Last 5 transactions are:\n");
    for (i = start; i < count; i++)
    {
        printf("%s", lines[i]);
    }
}

void QUIT()
{
    printf("thankyou for using bankmanagment system\n");
    printf("GOODBYE!!");
    exit(0);
}

void initialMenu()
{
    accInfo accounts[100];
    int numAccounts = 0;
    double withdrawnToday = 0.0;
    int choice;

    do
    {
        printf("\n   BANK MANAGEMENT SYSTEM   \n");
        printf("1. LOGIN\n");
        printf("2. QUIT\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (LOGIN())
            {
                printf("Login successful!\n");
                LOAD(accounts, &numAccounts);
                mainMenu(accounts, &numAccounts, &withdrawnToday);
            }
            else
            {
                int retryChoice;
                printf("\nLogin failed. What would you like to do?\n");
                printf("1. Go back to main menu\n");
                printf("2. Exit program\n");
                printf("Enter your choice: ");
                scanf("%d", &retryChoice);

                if (retryChoice == 2)
                {
                    printf("Exiting program. Goodbye!\n");
                    exit(0);
                }
            }
            break;

        case 2:
            printf("Thank you for using Bank Management System. Goodbye!\n");
            exit(0);

        default:
            printf("Invalid choice! Please try again.\n");
        }

    } while (1);
}

void mainMenu(accInfo accounts[], int *numAccounts, double *withdrawnToday)
{
    int mainChoice;

    do
    {
        printf("\n=== MAIN MENU ===\n");
        printf("1. ADD Account\n");
        printf("2. DELETE Account\n");
        printf("3. MODIFY Account\n");
        printf("4. SEARCH Account\n");
        printf("5. ADVANCED SEARCH\n");
        printf("6. CHANGE_STATUS\n");
        printf("7. WITHDRAW\n");
        printf("8. DEPOSIT\n");
        printf("9. TRANSFER\n");
        printf("10. REPORT\n");
        printf("11. SAVE\n");
        printf("12. PRINT(SORT)\n");
        printf("13. QUIT (Logout)\n");
        printf("Enter your choice: ");
        scanf("%d", &mainChoice);

        char accnumber[20]; // For Report function

        switch (mainChoice)
        {
        case 1:
            Add_Acc(accounts, numAccounts);
            break;
        case 2:
            Delete_Acc(accounts, numAccounts);
            break;
        case 3:
            Modify_Acc(accounts, numAccounts);
            break;
        case 4:
            search(accounts, *numAccounts);
            break;
        case 5:
            advanced_search(accounts, *numAccounts);
            break;
        case 6:
            CHANGE_STATUS(accounts, *numAccounts);
            break;
        case 7:
            WITHDRAW(accounts, *numAccounts, withdrawnToday);
            break;
        case 8:
            deposit(accounts, *numAccounts);
            break;
        case 9:
            transfer(accounts, *numAccounts);
            break;
        case 10:
            printf("Enter account number for report: ");
            scanf("%s", accnumber);
            Report(accnumber);
            break;
        case 11:
            SAVE(accounts, *numAccounts);
            break;
        case 12: // New option
            print(accounts, *numAccounts);
            break;
        case 13:
            printf("Logging out... Returning to initial menu.\n");
            return;
        default:
            printf("Invalid choice! Please try again.\n");
        }

    } while (1);
}
void print(accInfo accounts[], int n)
{
    int choice;

    printf("\n   SORT AND PRINT ACCOUNTS \n");
    printf("Sort by:\n");
    printf("1. Name\n");
    printf("2. Balance\n");
    printf("3. Date opened\n");
    printf("4. Status\n");
    printf("5. Cancel\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 5)
    {
        printf("Operation cancelled.\n");
        return;
    }

    // Make a copy of accounts array to avoid modifying original
    accInfo temp[n];
    for (int i = 0; i < n; i++)
    {
        temp[i] = accounts[i];
    }

    switch (choice)
    {
    case 1:
        sortByName(temp, n);
        printf("\nAccounts sorted by NAME:\n");
        break;
    case 2:
        SortByBalance(temp, n);
        printf("\nAccounts sorted by BALANCE:\n");
        break;
    case 3:
        SortByDate(temp, n);
        printf("\nAccounts sorted by DATE OPENED:\n");
        break;
    case 4:
        sortByStatus(temp, n);
        printf("\nAccounts sorted by STATUS:\n");
        break;
    default:
        printf("Invalid choice!\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        printf("\nAccount Number : %s\n", temp[i].accnumber);
        printf("Name: %s\n", temp[i].name);
        printf("Address : %s\n", temp[i].address);
        printf("Balance: %.2lf $\n", temp[i].balance);
        printf("Mobile: %s\n", temp[i].mobile);
        printf("Date Opened: %s %d\n", monthname(temp[i].dateopen.month),
               temp[i].dateopen.year);
        printf("Status: %s\n", temp[i].status);
    }
    printf("Total accounts: %d\n", n);
}
// bubble sort
void sortByName(accInfo accounts[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        { // n-i-1 --> no of comparisons
            if (strcasecmp(accounts[j].name, accounts[j + 1].name) > 0)
            { // strcasecmp --> for lower and uppercases
                // swap them acc to alph. order
                accInfo temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}
void SortByDate(accInfo accounts[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            // Compare year first, then month
            if (accounts[j].dateopen.year > accounts[j + 1].dateopen.year ||
                (accounts[j].dateopen.year == accounts[j + 1].dateopen.year &&
                 accounts[j].dateopen.month > accounts[j + 1].dateopen.month))
            {
                accInfo temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}
void SortByBalance(accInfo accounts[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (accounts[j].balance > accounts[j + 1].balance)
            {
                accInfo temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}
void sortByStatus(accInfo accounts[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (strcmp(accounts[j].status, "active") != 0 &&
                strcmp(accounts[j + 1].status, "active") == 0)
            {
                accInfo temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}
int main()
{
    initialMenu();
    return 0;
}

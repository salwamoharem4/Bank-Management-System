#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "bank_type.h"
#include "bank_function.h"
#include "colors.h"

// Color functions - ADD THESE
void printError(const char *msg)
{
    printf(RED "%s" RESET, msg);
}

void printSuccess(const char *msg)
{
    printf(GREEN "%s" RESET, msg);
}

void printWarning(const char *msg)
{
    printf(YELLOW "%s" RESET, msg);
}

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

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

int is_valid_email(const char *email)
{
    if (!email)
        return 0;
    if (strchr(email, ','))
        return 0;
    char *at = strchr(email, '@');
    if (!at || at == email)
        return 0;

    char *dot = strchr(at + 1, '.');
    if (!dot || dot[1] == '\0')
        return 0;

    return 1;
}

int isValidnumber(const char *str)
{
    if (!str || !*str)
        return 0;

    int dots = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            continue;
        }
        else if (str[i] == '.')
        {
            dots++;
            if (dots > 1)
                return 0;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

_Bool is_valid_name(const char *name)
{
    for (int i = 0; name[i]; i++)
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    return name[0] != '\0';
}

int LOGIN()
{
    char u[50], p[50], username[50], pass[50], *token, buffer[100];
    FILE *ptr = fopen("users.txt", "r");
    if (ptr == NULL)
    {
        printError("Error Cannot open users.txt\n");
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
            return 1;
        }
    }
    fclose(ptr);
    printError("Invalid username or password!\n");
    return 0;
}

void LOAD(accInfo bank_accounts[], int *n)
{
    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL)
    {
        printError("Error opening accounts.txt file.\n");
        return;
    }

    char line[5000];
    *n = 0;
    int i = 0;

    while (fgets(line, sizeof(line), file) != NULL && i < 100)
    {
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) <= 1)
            continue;

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
            i++;
        }
        else
        {
            printWarning("Warning: Skipping malformed line\n");
        }
    }

    *n = i;
    fclose(file);
    if (*n > 0)
    {
        printSuccess("Successfully loaded %d accounts.\n");
    }
    else
    {
        printWarning("No accounts loaded from file.\n");
    }
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
        printError("Couldn't find this account number\n");
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
        printError("Couldn't find this account name\n");
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
        printError("Error saving to accounts.txt file.\n");
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
    printSuccess("All accounts saved successfully to accounts.txt\n");
}

void Add_Acc(accInfo bank_accounts[], int *n)
{
    accInfo new_account = {0};
    char temp_buffer[1000];

    while (1)
    {
        printf("Please enter account number: ");
        scanf("%s", temp_buffer);
        clearInputBuffer();

        if (isValidnumber(temp_buffer))
        {
            strncpy(new_account.accnumber, temp_buffer, sizeof(new_account.accnumber) - 1);
            new_account.accnumber[sizeof(new_account.accnumber) - 1] = '\0';
            break;
        }
        else
        {
            printError("Invalid account number. Please enter numbers only\n");
        }
    }

    if (isDuplicate(bank_accounts, *n, new_account.accnumber))
    {
        printWarning("This account already exists\n");
        return;
    }

    while (1)
    {
        printf("Please enter name (letters and spaces only): ");
        fgets(new_account.name, sizeof(new_account.name), stdin);
        new_account.name[strcspn(new_account.name, "\n")] = '\0';

        if (strlen(new_account.name) == sizeof(new_account.name) - 1)
        {
            clearInputBuffer();
        }

        if (is_valid_name(new_account.name))
        {
            break;
        }
        else
        {
            printError("Invalid name! Use only letters and spaces.\n");
        }
    }

    int email_valid = 0;
    while (!email_valid)
    {
        printf("Please enter email address: ");
        fgets(new_account.address, sizeof(new_account.address), stdin);
        new_account.address[strcspn(new_account.address, "\n")] = '\0';

        if (is_valid_email(new_account.address))
        {
            email_valid = 1;
        }
        else
        {
            printError("Invalid email! Must contain '@' and '.' \n");
        }
    }

    while (1)
    {
        printf("Please enter balance: ");
        fgets(temp_buffer, sizeof(temp_buffer), stdin);
        temp_buffer[strcspn(temp_buffer, "\n")] = '\0';

        if (strlen(temp_buffer) == sizeof(temp_buffer) - 1)
        {
            clearInputBuffer();
        }

        if (isValidnumber(temp_buffer))
        {
            sscanf(temp_buffer, "%lf", &new_account.balance);
            break;
        }
        else
        {
            printError("Invalid amount. Enter again\n");
        }
    }

    while (1)
    {
        printf("Please enter mobile number (digits only): ");
        fgets(temp_buffer, sizeof(temp_buffer), stdin);
        temp_buffer[strcspn(temp_buffer, "\n")] = '\0';

        if (strlen(temp_buffer) == sizeof(temp_buffer) - 1)
        {
            clearInputBuffer();
        }

        if (isValidnumber(temp_buffer))
        {
            strncpy(new_account.mobile, temp_buffer, sizeof(new_account.mobile) - 1);
            new_account.mobile[sizeof(new_account.mobile) - 1] = '\0';
            break;
        }
        else
        {
            printError("Invalid mobile number! Please enter digits only.\n");
        }
    }

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
        printSuccess("Account added successfully!\n");
        if (askToSave())
        {
            SAVE(bank_accounts, *n);
        }
    }
    else
    {
        printError("Error opening accounts.txt\n");
    }
}

void Delete_Acc(accInfo bank_accounts[], int *n)
{
    char account_num[20];
    printf("Please enter the account number to delete: ");
    scanf("%s", account_num);
    clearInputBuffer();
    int found = 0;

    for (int i = 0; i < *n; i++)
    {
        if (strcmp(bank_accounts[i].accnumber, account_num) == 0)
        {
            found = 1;

            if (bank_accounts[i].balance > 0)
            {
                printError("Cannot delete account - balance must be zero\n");
                printf("Current balance: %.2lf\n", bank_accounts[i].balance);
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
                    printSuccess("Account deleted successfully!\n");
                }
                else
                {
                    printError("Error opening accounts.txt\n");
                }
            }
            return;
        }
    }

    if (!found)
    {
        printError("Account not found\n");
    }
}

void Modify_Acc(accInfo bank_accounts[], int *n)
{
    char accnum[20];
    int i, found = 0;
    char temp_buffer[1000];

    printf("Enter account number: ");
    scanf("%s", accnum);
    clearInputBuffer();

    for (i = 0; i < *n; i++)
    {
        if (strcmp(bank_accounts[i].accnumber, accnum) == 0)
        {
            found = 1;
            printf("Current name: %s\n", bank_accounts[i].name);

            while (1)
            {
                printf("Enter new name (letters and spaces only): ");
                fgets(temp_buffer, sizeof(temp_buffer), stdin);
                temp_buffer[strcspn(temp_buffer, "\n")] = '\0';

                if (strlen(temp_buffer) == sizeof(temp_buffer) - 1)
                {
                    clearInputBuffer();
                }

                if (is_valid_name(temp_buffer))
                {
                    strncpy(bank_accounts[i].name, temp_buffer, sizeof(bank_accounts[i].name) - 1);
                    bank_accounts[i].name[sizeof(bank_accounts[i].name) - 1] = '\0';
                    break;
                }
                else
                {
                    printError("Invalid name! Use only letters and spaces.\n");
                }
            }

            printf("Current mobile: %s\n", bank_accounts[i].mobile);

            while (1)
            {
                printf("Enter new mobile number (digits only): ");
                fgets(temp_buffer, sizeof(temp_buffer), stdin);
                temp_buffer[strcspn(temp_buffer, "\n")] = '\0';

                if (strlen(temp_buffer) == sizeof(temp_buffer) - 1)
                {
                    clearInputBuffer();
                }

                if (isValidnumber(temp_buffer))
                {
                    strncpy(bank_accounts[i].mobile, temp_buffer, sizeof(bank_accounts[i].mobile) - 1);
                    bank_accounts[i].mobile[sizeof(bank_accounts[i].mobile) - 1] = '\0';
                    break;
                }
                else
                {
                    printError("Invalid mobile number! Use digits only.\n");
                }
            }

            printf("Current email address: %s\n", bank_accounts[i].address);

            int email_valid = 0;
            while (!email_valid)
            {
                printf("Enter new email address: ");
                fgets(temp_buffer, sizeof(temp_buffer), stdin);
                temp_buffer[strcspn(temp_buffer, "\n")] = '\0';

                if (strlen(temp_buffer) == sizeof(temp_buffer) - 1)
                {
                    clearInputBuffer();
                }

                if (is_valid_email(temp_buffer))
                {
                    strncpy(bank_accounts[i].address, temp_buffer, sizeof(bank_accounts[i].address) - 1);
                    bank_accounts[i].address[sizeof(bank_accounts[i].address) - 1] = '\0';
                    email_valid = 1;
                }
                else
                {
                    printError("Invalid email! Must contain '@' and '.' and cannot contain commas.\n");
                }
            }

            break;
        }
    }

    if (!found)
    {
        printError("Account not found\n");
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
            printError("Error opening accounts.txt\n");
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
        printSuccess("Account modified successfully\n");
    }
}

void CHANGE_STATUS(accInfo acc[], int n)
{
    char search_number[20], desired_status[50];
    int valid_status = 0;

    printf("Enter your account's number: ");
    scanf("%s", search_number);
    clearInputBuffer();

    while (!valid_status)
    {
        printf("Enter the desired account status (active or inactive): ");
        scanf("%s", desired_status);
        clearInputBuffer();

        if (strcmp(desired_status, "active") == 0 || strcmp(desired_status, "inactive") == 0)
        {
            valid_status = 1;
        }
        else
        {
            printError("Invalid status! Please enter only 'active' or 'inactive'.\n");
        }
    }

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
        printError("Couldn't find this account number!!\n");
        return;
    }

    if (strcmp(acc[index].status, desired_status) == 0)
    {
        printWarning("The account already has this status\n");
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
                printSuccess("The status has been successfully changed to %s\n");
            }
            else
            {
                printError("Error updating accounts file\n");
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
    clearInputBuffer();

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
        printError("Error: Account not found\n");
        return;
    }

    if (strcmp(acc[index].status, "active") != 0)
    {
        printWarning("Warning: Account is inactive. Process cannot be completed\n");
        return;
    }

    printf("Enter withdrawal amount: ");
    if (scanf("%lf", &amount) != 1)
    {
        printError("Error: Invalid amount entered.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    if (amount <= 0)
    {
        printError("Error: Withdrawal amount must be positive\n");
        return;
    }

    if (amount > 10000)
    {
        printError("Error: Maximum withdrawal per transaction is 10,000$\n");
        return;
    }

    if (!dailyLimit(*withdrawnToday, amount))
    {
        printError("Error: Daily withdrawal limit exceeded (50,000$)\n");
        return;
    }

    if (amount > acc[index].balance)
    {
        printError("Error: Insufficient balance.\n");
        printf("Current balance: %.2f$\n", acc[index].balance);
        printf("Requested amount: %.2f$\n", amount);
        return;
    }

    acc[index].balance -= amount;
    *withdrawnToday += amount;

    recordTransaction(acc[index].accnumber, "withdraw", amount, acc[index].balance);

    printSuccess("TRANSACTION SUCCESSFUL!\n");
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
    char temp_buffer[1000];
    printf("Enter your account number: ");
    scanf("%s", acc_num);
    clearInputBuffer();
    while (1)
    {
        printf("Enter the amount you want to deposit: ");
        scanf("%s", temp_buffer);
        clearInputBuffer();
        if (isValidnumber(temp_buffer))
        {
            sscanf(temp_buffer, "%lf", &deposit_amount);

            if (deposit_amount > 0)
            {
                break;
            }
            else
            {
                printError("Deposit amount must be positive.\n");
            }
        }
        else
        {
            printError("Invalid amount! Please enter numbers only.\n");
        }
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
            printWarning("Warning: This account is inactive. The process cannot be completed\n");
            return;
        }
        acc[index].balance += deposit_amount;
        printSuccess("Deposit completed successfully\n");

        recordTransaction(acc[index].accnumber, "deposit", deposit_amount, acc[index].balance);
        if (askToSave())
        {
            SAVE(acc, n);
        }
    }
    else
    {
        printError("Error: Account not found\n");
    }
}

void transfer(accInfo acc[], int n)
{
    char sender_acc[20], receiver_acc[20];
    double amount;
    int sender_index = -1, receiver_index = -1;
    printf("Enter sender account number: ");
    scanf("%s", sender_acc);
    clearInputBuffer();
    printf("Enter receiver account number: ");
    scanf("%s", receiver_acc);
    clearInputBuffer();
    printf("Enter transfer amount: ");
    scanf("%lf", &amount);
    clearInputBuffer();
    if (strcmp(sender_acc, receiver_acc) == 0)
    {
        printError("Error: Cannot transfer to the same account.\n");
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
        printError("Error: Sender account not found.\n");
        return;
    }
    if (strcmp(acc[sender_index].status, "active") != 0)
    {
        printError("Error: Sender account is inactive.\n");
        return;
    }

    if (receiver_index == -1)
    {
        printError("Error: Receiver account not found.\n");
        return;
    }
    if (strcmp(acc[receiver_index].status, "active") != 0)
    {
        printError("Error: Receiver account is inactive.\n");
        return;
    }

    if (amount <= 0)
    {
        printError("Error: Amount must be positive.\n");
        return;
    }
    if (acc[sender_index].balance < amount)
    {
        printError("Error: Insufficient balance.\n");
        return;
    }

    acc[sender_index].balance -= amount;
    acc[receiver_index].balance += amount;

    recordTransaction(sender_acc, "transfer-out", amount, acc[sender_index].balance);
    recordTransaction(receiver_acc, "transfer-in", amount, acc[receiver_index].balance);

    printSuccess("Transfer successful. New balance: $%.2f\n");

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
        printWarning("No transactions found for this account\n");
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
// ========== MENU FUNCTIONS ==========

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
        printWarning("Invalid choice!\n");
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
        {
            if (strcasecmp(accounts[j].name, accounts[j + 1].name) > 0)
            {
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
                printSuccess("Login successful!\n");
                LOAD(accounts, &numAccounts);
                mainMenu(accounts, &numAccounts, &withdrawnToday);
            }
            else
            {
                int retryChoice;
                printWarning("\nLogin failed. What would you like to do?\n");
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
            printWarning("Invalid choice! Please try again.\n");
        }

    } while (1);
}
// ========== BONUS: DELETE MULTIPLE ACCOUNTS ==========

void bonusMenu(accInfo accounts[], int *numAccounts)
{
    int option;
    printf("\n=== DELETE MULTIPLE ACCOUNTS ===\n");
    printf("1. Delete by Date\n");
    printf("2. Delete Inactive Accounts (>90 days & balance zero)\n");
    printf("Enter your choice: ");
    scanf("%d", &option);

    if (option == 1)
    {
        Date date;
        printf("Please enter date (MM-YYYY): ");
        if (scanf("%d-%d", &date.month, &date.year) != 2) // return number of successful operation
        // mafrod yb2o etnen -->month,year
        {
            printWarning("Invalid format!\n");
            clearInputBuffer();
            return;
        }
        if (date.month < 1 || date.month > 12)
        {
            printWarning("Invalid month!\n");
            return;
        }
        deleteMult(accounts, numAccounts, date);
    }
    else if (option == 2)
    {
        Date date;
        date.month = 0;
        date.year = 0;
        deleteMult(accounts, numAccounts, date);
    }
    else
    {
        printWarning("Invalid choice!\n");
    }
}

time_t convertToTimeT(Date date)
{
    struct tm tm_time = {0};
    tm_time.tm_year = date.year - 1900; // tm_year counts years since 1900
    tm_time.tm_mon = date.month - 1;    // tm_mon is 0-based
    tm_time.tm_mday = 1;
    // bbd2 mn el midnight
    tm_time.tm_hour = 0;
    tm_time.tm_min = 0;
    tm_time.tm_sec = 0;

    return mktime(&tm_time); // store seconds
}

int daysBetween(Date d1, Date d2)
{
    time_t t1 = convertToTimeT(d1);
    time_t t2 = convertToTimeT(d2);

    double diffSeconds = difftime(t2, t1);
    int diffDays = (int)(diffSeconds / (60 * 60 * 24)); // nthwlo le ayam alshan a3ml compare
    return diffDays;
}

int isInactiveMoreThan90Days(accInfo account, Date currentDate)
{
    // Create a last activity date (assuming last activity was when status changed)
    Date lastActivityDate;
    lastActivityDate.month = account.dateopen.month;
    lastActivityDate.year = account.dateopen.year;

    int diff = daysBetween(lastActivityDate, currentDate);
    return (diff > 90); // return true law el far2 akbr mn 90 yom
}

void deleteMult(accInfo acc[], int *n, Date date)
{
    int deletedCount = 0;

    if (date.month == 0) // Option 2 ana amla intialize lel month yb2a be 2 lma ykhtar option2
        Date currentDate = getCurrentDate();

    for (int i = 0; i < *n; i++)
    {
        if (strcmp(acc[i].status, "inactive") == 0 &&
            acc[i].balance == 0.0 &&
            isInactiveMoreThan90Days(acc[i], currentDate))
        {
            // Delete account by shifting remaining accounts left
            // bfdl a3ml shift shmal fa bshel el account el mesh mhtgah
            for (int j = i; j < *n - 1; j++)
            {
                acc[j] = acc[j + 1];
            }
            (*n)--; // b3d ma 3mlna delete number of account decreased by one
            i--;    // Stay at same index to check shifted element
            // b3d ma hasl shift baat btshawr aala acc tane mhtag a3mlo check
            deletedCount++;
        }
    }

    if (deletedCount == 0)
    {
        printWarning("No inactive accounts found with balance zero and inactive >90 days.\n");
    }
    else
    {
        printSuccess("%d inactive accounts deleted successfully.\n");
        printf("Accounts deleted: %d\n", deletedCount);
    }
}
else // Option 1: Delete by date
{
    for (int i = 0; i < *n; i++)
    {
        if (acc[i].dateopen.year == date.year &&
            acc[i].dateopen.month == date.month)
        {
            // Delete account by shifting remaining accounts left
            for (int j = i; j < *n - 1; j++)
            {
                acc[j] = acc[j + 1];
            }
            (*n)--;
            i--; // Stay at same index to check shifted element
            deletedCount++;
        }
    }

    if (deletedCount == 0)
    {
        char warningMsg[100];
        sprintf(warningMsg, "No accounts found created on %s %d\n", monthname(date.month), date.year);
        printWarning(warningMsg);
    }
    else
    {
        char successMsg[100];
        sprintf(successMsg, "%d inactive accounts deleted successfully.\n", deletedCount);
        printSuccess(successMsg);
        printf("Accounts created in %s %d deleted: %d\n", monthname(date.month), date.year, deletedCount);
    }
}

if (deletedCount > 0)
{
    if (askToSave())
    {
        SAVE(acc, *n);
    }
    else
    {
        // Update file anyway
        FILE *file = fopen("accounts.txt", "w");
        if (file != NULL)
        {
            for (int k = 0; k < *n; k++)
            {
                fprintf(file, "%s,%s,%s,%.2lf,%s,%d-%d,%s\n",
                        acc[k].accnumber,
                        acc[k].name,
                        acc[k].address,
                        acc[k].balance,
                        acc[k].mobile,
                        acc[k].dateopen.month,
                        acc[k].dateopen.year,
                        acc[k].status);
            }
            fclose(file);
        }
        else
        {
            printError("Error opening accounts.txt\n");
        }
    }
}
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
        printf("13. DELETE MULTIPLE Accounts\n");
        printf("14. QUIT \n");
        printf("Enter your choice: ");
        scanf("%d", &mainChoice);

        char accnumber[20];

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
        case 12:
            print(accounts, *numAccounts);
            break;
        case 13:
            bonusMenu(accounts, numAccounts);
            break;
        case 14:
            printf("Logging out... Returning to initial menu.\n");
            return;
        default:
            printWarning("Invalid choice! Please try again.\n");
        }

    } while (1);
}
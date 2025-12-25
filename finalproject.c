#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
            strcpy(u, token); //username

        token = strtok(NULL, " \t\n");
        if (token != NULL)
            strcpy(p, token);//pasword
        // replace \n by \0
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
void loadAccounts(accInfo bank_accounts[], int *n)
{
    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL)
    {
        printf("Error opening accounts.txt file.\n");
        exit(1);
    }
    char line[256];
    *n = 0;
    // Read and parse accounts 
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Skip empty lines
        if (strlen(line) <= 1)
            continue;

        // Replace \n by \0
        line[strcspn(line, "\n")] = '\0';

        // Parse the line
        int result = sscanf(line, "%19[^,],%99[^,],%99[^,],%lf,%14[^,],%d-%d,%49[^\n]",
                            bank_accounts[i].accnumber,
                            bank_accounts[i].name,
                            bank_accounts[i].address,
                            &bank_accounts[i].balance,
                            bank_accounts[i].mobile,
                            &bank_accounts[i].dateopen.month,
                            &bank_accounts[i].dateopen.year,
                            bank_accounts[i].status);
        if (result == 8) // All 8 fields successfully read
        {
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
            printf("E-mail : %s\n", acc[i].address);
            printf("Balance: %.2lf $\n", acc[i].balance); 

            printf("Mobile: %s\n", acc[i].mobile);
            printf("Date Opened: %s %d\n",monthname(acc[i].dateopen.month),acc[i].dateopen.year);

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
            printf("E-mail : %s\n", acc[i].address);
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
// it will be used by ALL transaction functions
void recordTransaction(char *accnumber, char *type, double amount, double new_balance)
{
    char filename[50];
    sprintf(filename, "%s.txt", accnumber);//create file per account
    FILE *file = fopen(filename, "a");

    if (file != NULL)
    {
        
        Date current_date = getCurrentDate();

        fprintf(file, "%04d-%02d %s %.2f Balance: %.2f\n",
                current_date.year,
                current_date.month,
                type,   //withdraw,deposit
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

    getchar(); // Clear the newline from scanf
    printf("Please enter name: ");
    fgets(new_account.name, sizeof(new_account.name), stdin);
    new_account.name[strcspn(new_account.name, "\n")] = '\0'; //law la2t new line bt replace by \0

    printf("Please enter email address: ");
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
        fprintf(file, "%s,%s,%s,%.2lf,%s,%d,%d,%s\n",
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

    int found = 0; // Flag to check if account exists

    for (int i = 0; i < *n; i++)
    {
        if (strcmp(bank_accounts[i].accnumber, account_num) == 0) 
        {
            found = 1; // Account found

            // Check if balance is zero
            if (bank_accounts[i].balance > 0)
            {
                printf("Cannot delete account - balance must be zero (Current balance: %.2lf)\n",
                       bank_accounts[i].balance);
                return;
            }

            // Delete the account by shifting elements
            for (int j = i; j < *n - 1; j++)
            {
                bank_accounts[j] = bank_accounts[j + 1];
            }

            (*n)--; 

            // Update the file
            FILE *file = fopen("accounts.txt", "w");
            if (file != NULL)
            {
                for (int k = 0; k < *n; k++)
                {
                    fprintf(file, "%s,%s,%s,%.2lf,%s,%d,%d,%s\n",
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

            printf("Current email: %s\n", bank_accounts[i].email);
            printf("Enter new email: ");
            fgets(bank_accounts[i].email, sizeof(bank_accounts[i].email), stdin);
            bank_accounts[i].email[strcspn(bank_accounts[i].email, "\n")] = '\0';

            printf("Account modified successfully\n");
            return;
        }
    }

    if (!found)
    {
        printf("Account not found\n");
    }
    FILE *file = fopen("accounts.txt", "w");
    if (file == NULL)
    {
        printf("Error opening accounts.txt\n");
        return;
    }

    for (int h = 0; h < *n; h++)
    {
        fprintf(file, "%s,%s,%s,%lf,%s,%d,%d,%s\n",
                bank_accounts[h].accnumber,
                bank_accounts[h].name,
                bank_accounts[h].email,
                bank_accounts[h].balance,
                bank_accounts[h].mobile,
                bank_accounts[h].dateopen.month,
                bank_accounts[h].dateopen.year,
                bank_accounts[h].status);
    }

    fclose(file);
    printf("Account modified successfully\n");
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

    // Convert desired_status to lowercase for comparison
    char desired_lower[50];
    strcpy(desired_lower, desired_status);
    for (int j = 0; desired_lower[j]; j++)
        desired_lower[j] = tolower(desired_lower[j]);

    if (strcmp(acc[index].status, desired_lower) == 0)
    {
        printf("The account already has this status\n");
    }
    else
    {
       
        strcpy(acc[index].status, desired_lower); 

        // Update the accounts.txt file
        FILE *file = fopen("accounts.txt", "w");
        if (file != NULL)
        {
            for (int j = 0; j < n; j++)
            {
                fprintf(file, "%s,%s,%s,%.2lf,%s,%d,%d,%s\n",acc[j].accnumber,acc[j].name,acc[j].address,acc[j].balance,acc[j].mobile,acc[j].dateopen.month,acc[j].dateopen.year,acc[j].status);
            }
            fclose(file);
            printf("The status has been successfully changed to %s\n", desired_lower);
        }
        else
        {
            printf("Error updating accounts file\n");
        }
    }
}
    int dailyLimit(double withdrawnToday, double amount)
    {
        // Check if adding this amount exceeds daily limit
        if (withdrawnToday + amount > 50000)
            return 0; // exceeded

        return 1; // allowed
    }

void WITHDRAW(accInfo *acc, double *withdrawnToday)
{
    double amount;
    char input[100]; //store account number entered by user
    printf("\n Withdrawal Process \n");
    printf("Enter active account number: ");
    scanf("%s", input);
    // Verify account number and see if it matches
    if (strcmp(acc->accnumber, input) != 0)
    {
        printf("Error Account number does not match\n");
        return;
    }

    // Check account status 
    if (strcmp(acc->status, "active") != 0)
    {
        printf("Warning Account is inactive. Process cannot be completed\n");
        return;
    }

    // Get withdrawal amount 
    printf("Enter withdrawal amount: ");
    if (scanf("%lf", &amount) != 1) //to make sure it is valid input
    {
        printf("Error: Invalid amount entered.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    //check amount of money enterd
    if (amount <= 0)
    {
        printf("Error: Withdrawal amount must be positive\n");
        return;
    }

    //  limit 
    if (amount > 10000)
    {
        printf("Error: Maximum withdrawal per transaction is 10,000$\n");
        return;
    }

    //Daily limit check 
    if (!dailyLimit(*withdrawnToday, amount)) 
    {
        printf("Error: Daily withdrawal limit exceeded (50,000$)\n");
        return;
    }

    //check if the user has enough money for withdrawal
    if (amount > acc->balance)
    {
        printf("Error: Insufficient balance.\n");
        printf("Current balance: %.2f$\n", acc->balance);
        printf("Requested amount: %.2f$\n", amount);
        return;
    }

    //Process the withdrawal 
    acc->balance -= amount;
    *withdrawnToday += amount;
    recordTransaction(accnumber, "WITHDRAW", amount, bank_accounts[i].balance); 
    printf("TRANSACTION SUCCESSFUL!\n");
    printf("Account: %s\n", acc->accnumber);
    printf("Withdrawn: %.2f$\n", amount);
    printf("New Balance: %.2f$\n", acc->balance);
    printf("Total withdrawn today: %.2f$\n", *withdrawnToday);
}
void Report(char accnumber[]) 
{
    char lines[100][100]; // Array to store transaction lines from the file
    char filename[20];    // To store the file name (accountNumber.txt)
    int count = 0;        // Number of transactions in the file
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

    // If there are more than 5 transactions, start from the last 5
    if (count > 5)
    {
        start = count - 5;
    }
    else
    {
        start = 0; // Otherwise, print all transactions
    }

    // Print the last 5 transactions
    printf("Last 5 transactions are:\n");
    for (i = start; i < count; i++)
    {
        printf("%s", lines[i]);
    }
}
int main()
{
   
    return 0;
}
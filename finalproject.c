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

    printf("TRANSACTION SUCCESSFUL!\n");
    printf("Account: %s\n", acc->accnumber);
    printf("Withdrawn: %.2f$\n", amount);
    printf("New Balance: %.2f$\n", acc->balance);
    printf("Total withdrawn today: %.2f$\n", *withdrawnToday);
}
int main()
{
   
    return 0;
}
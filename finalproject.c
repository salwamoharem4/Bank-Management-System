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

int main()
{
   
    return 0;
}
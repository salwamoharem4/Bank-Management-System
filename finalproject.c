#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct
{
    int month;
    int year;
} date;
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
    printf("Enter username:");
    scanf("%s", username);
    printf("Enter password:");
    scanf("%s", pass);
    if (ptr == NULL)
    {
        printf("Error");
        exit(-1);
    }
    while (fgets(buffer, sizeof(buffer), ptr) != NULL)
    {
        token = strtok(buffer, " "); //split it to username and pass
        if (token != NULL)
            strcpy(u, token); //for username
        token = strtok(NULL, " ");
        if (token != NULL)
            strcpy(p, token); //for password
        int i = 0;
        while (p[i] != '\0')
        {
            if (p[i] == '\n')
            {
                p[i] = '\0'; //replace newline by null terminator
                break;
            }
            i++;
        }
        if (strcmp(u, username) == 0 && strcmp(p, pass) == 0) // check login credentials
            return 1;
    }
    fclose(ptr);
    return 0;
}
void loadAccounts(accInfo bank_accounts[], int *n)
{
    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL)
    {
        printf("Error opening accounts.txt file.\n");
        Quit();
    }

    char line[256];
    *n = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        (*n)++;
    }
    rewind(file); // resets the file position indicator to the beginning of the file.

    // Read each account
    int i = 0;
    for (i = 0; i < (*n); i++)
    {
        fscanf(file, "%19[^,],%99[^,],%99[^,],%lf,%11[^,],%d-%d,%49[^\n]%*c",
               bank_accounts[i].accnumber,
               bank_accounts[i].name,
               bank_accounts[i].address, 
               &bank_accounts[i].balance,
               bank_accounts[i].mobile,
               &bank_accounts[i].dateopen.month,
               &bank_accounts[i].dateopen.year,
               bank_accounts[i].status); 
    }
    fclose(file);
}
int main()
{

    return 0;
}

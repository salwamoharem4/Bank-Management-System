#ifndef BANK_FUNCTIONS_H
#define BANK_FUNCTIONS_H

#include "bank_type.h"

// Utility functions
Date getCurrentDate();
char *monthname(int month);
void clearInputBuffer();
int isValidnumber(const char *str);
_Bool is_valid_name(const char *name);
int is_valid_email(const char *email);
void bonusMenu(accInfo accounts[], int *numAccounts);
void deleteMult(accInfo acc[], int *n, Date date);
int isInactiveMoreThan90Days(accInfo account, Date currentDate);
int daysBetween(Date d1, Date d2);
time_t convertToTimeT(Date date);
// File operations
int LOGIN();
void LOAD(accInfo bank_accounts[], int *n);
void SAVE(accInfo accounts[], int numAccounts);
int askToSave();
void recordTransaction(char *accnumber, char *type, double amount, double new_balance);

// Account operations
void Add_Acc(accInfo bank_accounts[], int *n);
void Delete_Acc(accInfo bank_accounts[], int *n);
void Modify_Acc(accInfo bank_accounts[], int *n);
void search(accInfo acc[], int n);
void advanced_search(accInfo acc[], int n);
void CHANGE_STATUS(accInfo acc[], int n);
void WITHDRAW(accInfo acc[], int n, double *withdrawnToday);
void deposit(accInfo acc[], int n);
void transfer(accInfo acc[], int n);
void Report(char accnumber[]);

// Sorting functions
void sortByName(accInfo accounts[], int n);
void SortByBalance(accInfo accounts[], int n);
void SortByDate(accInfo accounts[], int n);
void sortByStatus(accInfo accounts[], int n);

// Menu functions
void initialMenu();
void mainMenu(accInfo accounts[], int *numAccounts, double *withdrawnToday);
void print(accInfo accounts[], int n);

#endif
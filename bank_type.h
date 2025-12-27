#ifndef BANK_TYPE_H
#define BANK_TYPE_H

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

#endif
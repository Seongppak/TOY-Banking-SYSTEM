#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../니마이 완성을 위한 프로젝트/banking.h"

static int fault = 0, rename_count = 0;
static int test_rename(const char *from, const char *to)
{
    rename_count++;
    if ((fault == 1 && rename_count == 1) ||
        (fault == 2 && rename_count == 2) ||
        (fault == 3 && rename_count >= 2))
    {
        errno = EACCES;
        return -1;
    }
    return rename(from, to);
}
static int test_remove(const char *path)
{
    if (fault == 4 && strstr(path, ".bak") != NULL) { errno = EACCES; return -1; }
    return remove(path);
}
static int test_fflush(FILE *file)
{
    if (fault == 5) { errno = ENOSPC; return EOF; }
    return fflush(file);
}
#define rename test_rename
#define remove test_remove
#define fflush test_fflush
#include "../니마이 완성을 위한 프로젝트/bandking.c"
#undef rename
#undef remove
#undef fflush

int main(int argc, char *argv[])
{
    struct human accounts[MAX_ACCOUNTS], original[MAX_ACCOUNTS];
    int count, i, result;
    memset(accounts, 0x5A, sizeof(accounts));
    memcpy(original, accounts, sizeof(accounts));
    count = read_file("customer_info.txt", accounts, MAX_ACCOUNTS);
    printf("COUNT=%d UNCHANGED=%d\n", count, memcmp(accounts, original, sizeof(accounts)) == 0);
    if (count < 0) return 1;
    for (i = 0; i < count; i++)
        printf("ACCOUNT=%lld %hd %s %lld\n", accounts[i].ac_num, accounts[i].pw, accounts[i].name, accounts[i].balance);
    if (argc > 1 && count > 0)
    {
        fault = atoi(argv[1]);
        deposit(&accounts[0].balance, 5000);
        result = save_file("customer_info.txt", accounts, count);
        printf("SAVE=%d\n", result);
    }
    return 0;
}

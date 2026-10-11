#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "banking.h"

static void fixture(const char *text)
{
	FILE *file = fopen("fixture.txt", "w");
	assert(file != NULL);
	assert(fputs(text, file) >= 0);
	assert(fclose(file) == 0);
}

int main(void)
{
	struct human accounts[MAX_ACCOUNTS] = { 0 };
	FILE *file;
	long long number;
	int i;
	assert(read_file("missing.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == 0);
	fixture("\xEF\xBB\xBF" "1 1234 김백제 10000\n2 1275 이온조 25000\n3 1685 최쿠농 100000\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == 3);
	assert(accounts[0].balance == 10000 && accounts[1].balance == 25000 && accounts[2].balance == 100000);
	assert(strcmp(accounts[0].name, "김백제") == 0);
	assert(read_file("fixture.txt", accounts, 2) == -1);
	fixture("1 1234 김백제 10000\nwrong\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	assert(accounts[1].balance == 25000);
	fixture("1 1234 김백제 10000 extra\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("1 40000 김백제 10000\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("1 1234 abcdefghijklmnopqrst 10000\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("1 1234 김백제 -1\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("1 1234 김백제 10000\n1 1234 김백제 10000\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	file = fopen("fixture.txt", "w");
	assert(file != NULL);
	for (i = 1; i <= MAX_ACCOUNTS + 1; i++) fprintf(file, "%d 1234 name 0\n", i);
	assert(fclose(file) == 0);
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	assert(save_file("missing_directory/file.txt", accounts, 3) == -1);
	assert(save_file("saved.txt", accounts, 3) == 0);
	assert(read_file("saved.txt", accounts, MAX_ACCOUNTS) == 3);
	assert(accounts[1].balance == 25000);
	assert(transfer_between_accounts("saved.txt", accounts, 3, -1, 1, 500) == TRANSFER_INVALID);
	assert(transfer_between_accounts("saved.txt", accounts, 3, 0, 3, 500) == TRANSFER_INVALID);
	assert(transfer_between_accounts("saved.txt", accounts, 3, 0, 0, 500) == TRANSFER_INVALID);
	assert(accounts[0].balance == 10000 && accounts[1].balance == 25000);
	assert(transfer_between_accounts("missing_directory/file.txt", accounts, 3, 0, 1, 500) == TRANSFER_SAVE_ERROR);
	assert(accounts[0].balance == 10000 && accounts[1].balance == 25000);
	assert(transfer_between_accounts("saved.txt", accounts, 3, 0, 1, 500) == TRANSFER_SUCCESS);
	assert(accounts[0].balance == 9500 && accounts[1].balance == 25500);
	assert(read_file("saved.txt", accounts, MAX_ACCOUNTS) == 3);
	assert(accounts[0].balance == 9500 && accounts[1].balance == 25500);
	assert(!parse_number("9223372036854775808", &number));
	assert(!parse_number("12abc", &number));
	assert(parse_number("9223372036854775807", &number) && number == LLONG_MAX);
	puts("file_checks: PASS");
	return 0;
}

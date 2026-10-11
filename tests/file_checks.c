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
	fixture("\xEF\xBB\xBF" "1 1234 ±ËπÈ¡¶ 10000\n2 1275 ¿Ãø¬¡∂ 25000\n3 1685 √÷ƒÌ≥Û 100000\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == 3);
	assert(accounts[0].balance == 10000 && accounts[1].balance == 25000 && accounts[2].balance == 100000);
	assert(strcmp(accounts[0].name, "±ËπÈ¡¶") == 0);
	assert(read_file("fixture.txt", accounts, 2) == -1);
	fixture("1 1234 ±ËπÈ¡¶ 10000\nwrong\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	assert(accounts[1].balance == 25000);
	fixture("1 1234 ±ËπÈ¡¶ 10000 extra\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("1 40000 ±ËπÈ¡¶ 10000\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("1 1234 abcdefghijklmnopqrst 10000\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("1 1234 ±ËπÈ¡¶ -1\n");
	assert(read_file("fixture.txt", accounts, MAX_ACCOUNTS) == -1);
	fixture("1 1234 ±ËπÈ¡¶ 10000\n1 1234 ±ËπÈ¡¶ 10000\n");
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
	assert(!parse_number("9223372036854775808", &number));
	assert(!parse_number("12abc", &number));
	assert(parse_number("9223372036854775807", &number) && number == LLONG_MAX);
	puts("file_checks: PASS");
	return 0;
}

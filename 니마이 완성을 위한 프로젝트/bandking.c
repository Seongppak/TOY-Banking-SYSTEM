#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include "banking.h"

// 계좌번호 검색
int find_account(struct human human_list[], int count, long long input_ac_num)
{
	int i;

	for (i = 0; i < count; i++)
	{
		if (input_ac_num == human_list[i].ac_num)
		{
			return i;
		}
	}

	return -1;
}

// 비밀번호 확인
int check_password(struct human human_list[], int account_index, short input_pw)
{
	if (input_pw == human_list[account_index].pw)
	{
		return 1;
	}

	return 0;
}

// 입금
void deposit(long long* balance, long long in_money)
{
	*balance = *balance + in_money;
}

// 출금
int withdraw(long long* balance, long long out_money)
{
	if (out_money > *balance)
		return 0;

	else if (out_money <= 0)
		return 0;

	else
	{
		*balance = *balance - out_money;
		return 1;
	}
}

// 이체
int transfer(long long* balance, long long out_money)
{
	if (out_money > *balance)
		return 0;

	else if (out_money <= 0)
		return 0;

	else
	{
		*balance = *balance - out_money;
		return 1;
	}
}

// 잔액조회
void show_balance(long long balance)
{
	printf("현재 귀하의 잔액은 %lld원 입니다.\n", balance);
}

// 이름 확인
int check_name(struct human human_list[], int account_index, char input_name[])
{
	if (strcmp(human_list[account_index].name, input_name) == 0)
	{
		return 1;
	}

	return 0;
}

// 로그 저장
void save_log(
	struct log log_list[],
	int* log_count,
	char work[],
	long long money,
	long long balance
)
{
	if (*log_count < N)
	{
		strcpy(log_list[*log_count].work, work);

		log_list[*log_count].money = money;
		log_list[*log_count].balance = balance;

		(*log_count)++;
	}
}

// 로그 출력
void show_log(struct log log_list[], int log_count)
{
	int i;

	if (log_count == 0)
	{
		printf("확인할 작업 내역이 없습니다.\n");
		return;
	}

	printf("\n------ 작업 내역 ------\n");

	for (i = 0; i < log_count; i++)
	{
		if (strcmp(log_list[i].work, "잔액조회") == 0)
		{
			printf("%d. %s / 현재 잔액 %lld원\n",
				i + 1,
				log_list[i].work,
				log_list[i].balance);
		}

		else
		{
			printf("%d. %s / %lld원 / 거래 후 잔액 %lld원\n",
				i + 1,
				log_list[i].work,
				log_list[i].money,
				log_list[i].balance);
		}
	}
}
// 한 줄 단위로 읽어 긴 입력의 나머지가 다음 입력에 섞이지 않도록 처리
static int read_line(FILE *file, char text[], int size)
{
	int ch, length = 0, invalid = 0;
	while ((ch = fgetc(file)) != '\n' && ch != EOF)
	{
		if (ch == '\0') invalid = 1;
		if (length < size - 1) text[length++] = (char)ch;
		else invalid = 1;
	}
	text[length] = '\0';
	if (ferror(file)) return -1;
	if (ch == EOF && length == 0 && !invalid) return -1;
	if (invalid) return 0;
	if (length > 0 && text[length - 1] == '\r') text[length - 1] = '\0';
	return 1;
}

int read_text(char text[], int size)
{
	char line[256];
	char *start, *end;
	int result = read_line(stdin, line, sizeof(line));
	if (result != 1)
		return result;
	start = line;
	while (isspace((unsigned char)*start)) start++;
	end = start + strlen(start);
	while (end > start && isspace((unsigned char)end[-1])) end--;
	*end = '\0';
	if (*start == '\0' || strlen(start) >= (size_t)size)
		return 0;
	for (end = start; *end; end++)
		if (isspace((unsigned char)*end)) return 0;
	strcpy(text, start);
	return 1;
}

int parse_number(const char text[], long long *number)
{
	char *end;
	long long value;
	errno = 0;
	value = strtoll(text, &end, 10);
	if (errno == ERANGE || end == text || *end != '\0')
		return 0;
	*number = value;
	return 1;
}

int read_number(long long *number)
{
	char text[64];
	int result = read_text(text, sizeof(text));
	return result == 1 ? parse_number(text, number) : result;
}

int read_password(short *password)
{
	long long number;
	int result = read_number(&number);
	if (result != 1) return result;
	if (number < 0 || number > SHRT_MAX) return 0;
	*password = (short)number;
	return 1;
}

int read_file(const char *file_name, struct human human_list[], int max_accounts)
{
	FILE *file;
	struct human loaded[MAX_ACCOUNTS];
	char line[256], ac_num[64], pw[64], name[256], balance[64], extra;
	long long password;
	int count = 0, result, i;
	if (file_name == NULL || human_list == NULL || max_accounts <= 0 || max_accounts > MAX_ACCOUNTS)
		return -1;
	file = fopen(file_name, "rb");
	if (file == NULL) return -1;
	while ((result = read_line(file, line, sizeof(line))) != -1)
	{
		if (result != 1 || count >= max_accounts ||
			sscanf(line, "%63s %63s %255s %63s %c", ac_num, pw, name, balance, &extra) != 4 ||
			strlen(name) >= N || !parse_number(ac_num, &loaded[count].ac_num) ||
			!parse_number(pw, &password) || !parse_number(balance, &loaded[count].balance) ||
			loaded[count].ac_num <= 0 || password < 0 || password > SHRT_MAX || loaded[count].balance < 0)
			goto read_error;
		for (i = 0; i < count; i++)
			if (loaded[i].ac_num == loaded[count].ac_num) goto read_error;
		loaded[count].pw = (short)password;
		strcpy(loaded[count].name, name);
		count++;
	}
	if (ferror(file)) goto read_error;
	if (fclose(file) != 0) return -1;
	memcpy(human_list, loaded, count * sizeof(loaded[0]));
	return count;

read_error:
	fclose(file);
	return -1;
}

int save_file(const char *file_name, const struct human human_list[], int account_count)
{
	FILE *file;
	char temporary[1024], backup[1024];
	int i, failed = 0;
	if (file_name == NULL || human_list == NULL || account_count < 0 || account_count > MAX_ACCOUNTS ||
		strlen(file_name) + 5 > sizeof(temporary)) return -1;
	sprintf(temporary, "%s.tmp", file_name);
	sprintf(backup, "%s.bak", file_name);

	file = fopen(file_name, "r");
	if (file == NULL) return -1;
	if (fclose(file) != 0) return -1;
	file = fopen(backup, "r");
	if (file != NULL)
	{
		fclose(file);
		printf("백업 파일이 남아 있습니다. %s를 확인하십시오.\n", backup);
		return -1;
	}
	if (errno != ENOENT) return -1;
	file = fopen(temporary, "r");
	if (file != NULL)
	{
		fclose(file);
		printf("임시 파일이 남아 있습니다. %s를 확인하십시오.\n", temporary);
		return -1;
	}
	if (errno != ENOENT) return -1;
	file = fopen(temporary, "w");
	if (file == NULL) return -1;
	for (i = 0; i < account_count; i++)
	{
		if (fprintf(file, "%lld %hd %s %lld\n", human_list[i].ac_num,
			human_list[i].pw, human_list[i].name, human_list[i].balance) < 0)
		{
			failed = 1;
			break;
		}
	}
	if (fflush(file) != 0) failed = 1;
	if (fclose(file) != 0) failed = 1;
	if (failed || rename(file_name, backup) != 0)
	{
		remove(temporary);
		return -1;
	}
	if (rename(temporary, file_name) != 0)
	{
		if (rename(backup, file_name) != 0)
			printf("기존 파일 복구에 실패했습니다. %s와 %s를 보존하고 확인하십시오.\n", backup, temporary);
		else remove(temporary);
		return -1;
	}
	if (remove(backup) != 0)
		printf("저장은 완료되었으나 백업 파일을 지우지 못했습니다. %s를 확인하십시오.\n", backup);
	return 0;
}

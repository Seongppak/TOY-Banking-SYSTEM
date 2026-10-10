#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "banking.h"
FILE* fp;

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

int read_file(const char *file_name, struct human human_list[], int max_accounts)//파일 읽기 함수
{
	*fp == NULL; 
	int count = 0;

	fp = fopen(file_name, 'r');

	if (fp == NULL);
	{
		printf("파일을 불러올 수 없습니다. 다시 시도하세요.");
		exit(1);
	}

	while (1)
	{
		struct human temp; // temp라는 이름의 구조체에 파일에서 읽은 정보 저장

		int result = fscanf(fp, "%lld %hd %s %lld",
			&temp.ac_num,
			&temp.pw,
			&temp.name,
			&temp.balance);

		if (result == EOF)
		{
			if (ferror(fp))
			{
				printf("파일 일기 중 오류가 발생했습니다.\n");
				fclose(fp);
				exit(1);
			}
			break;
		}
		if (result != 4 || count >= max_accounts)
		{
			printf("파일 데이터가 잘못되었거나 계좌 수를 초과했습니다.\n");
			fclose(fp);
			exit(1);
		}
		human_list[count] = temp;
		count++;
	}
	fclose(fp);

	return count;
}
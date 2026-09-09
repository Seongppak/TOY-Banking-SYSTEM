#include "banking.h"
#include <stdio.h>
#include <string.h>
int find_account(struct human human_list[], int count, long long input_ac_num) // 계좌 조회 
{
	int i;
	for (i = 0; i < count; i++)
	{
		if (input_ac_num == human_list[i].ac_num) // 입력된 계좌와 등록된 계좌가 동일 할 때 값을 1로 반환한다.
		{
			return 1;
		}


	}
	return -1; // if 문에 잡히지 않음 즉, 등록된 계좌와 입력된 계좌가 같은게 없을 때는 -1로 반환한다.
}

int check_password(struct human human_list[], int account_index, short input_pw) // 비밀번호 검증
{
	if (input_pw == human_list[account_index].pw)
	{
		return 1;
	}
	return 0;
}

//입금은 출금, 이체 등과 같이 성공(1)과 실패(0)을 반환해줄 필요가 없기 때문에 int가 아니라 void를 자료형으로 사용한다.
void deposit(long long* balance, long long in_money) // 입금
{
	*balance = in_money + *balance;
}

int withdraw(long long* balance, long long out_money) // 출금
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

int transfer(long long* balance, long long out_money) // 이체
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

void show_balance(long long balance) // 잔액 조회
{
	printf("귀하의 잔액은 %d원 입니다.", balance);
}

int check_name(struct human human_list[], int account_index, char input_name[]) // 이름 확인
{
	if(strcmp(human_list[account_index].name, input_name==0))

		return 1;

	else

		return 0;

}

void save_log(struct log log_list[], int* log_count, char work[], long long money, long long balance) // 로그 저장
{
	if (*log_count < N)
	{
		strcpy(log_list[*log_count].work, work);
		log_list[*log_count].money = money;
		log_list[*log_count].balance = balance;

		(*log_count)++;
	}
}

void show_log(struct log log_list[], int log_count) // 로그 출력
{
	int i;

	if (log_count == 0)
	{
		printf("확인할 작업 내역이 없습니다.\n");
		return;
	}

	printf("\n===== 작업 내역 =====\n");

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
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "banking.h"



int main(void)
{
	struct human human_list[MAX_ACCOUNTS] ={0};

	struct log log_list[N];

	int log_count = 0;
	int account_count = 0;
	int account_index;
	int check = 0;

	long long original_money = 10000;
	long long input_ac_num;
	long long in_money;
	long long out_money;

	char want_work[N];

	printf("등록하신 계좌번호를 입력하십시오.:");
	scanf("%lld", &input_ac_num);

	account_index = find_account(human_list, account_count, input_ac_num);

	if (account_index == -1)
	{
		printf("입력하신 계좌번호를 찾을 수 없습니다.\n");
		return 0;
	}

	while (1)
	{
		printf("원하시는 작업을 입력해주세요( 입금, 출금, 이체, 잔액조회, 종료 ):");
		scanf("%s", want_work);

		// 입금
		if (strcmp(want_work, "입금") == 0)
		{
			printf("입금할 금액을 입력하여 주십시오.:");
			scanf("%lld", &in_money);

			if (in_money <= 0)
			{
				printf("입금 금액이 올바르지 않습니다.\n");
			}

			else
			{
				deposit(&original_money, in_money);

				printf("입금이 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n",
					original_money);

				save_log(
					log_list,
					&log_count,
					"입금",
					in_money,
					original_money
				);

				while (1)
				{
					char in_want_work[N];

					printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
					scanf("%s", in_want_work);

					if (strcmp(in_want_work, "네") == 0)
					{
						check = 1;
						break;
					}

					else if (strcmp(in_want_work, "아니오") == 0)
					{
						break;
					}

					else
					{
						printf("입력값이 잘못 되었습니다.\n");
					}
				}

				if (check == 1)
					break;
			}
		}

		// 출금
		else if (strcmp(want_work, "출금") == 0)
		{
			short input_pw;

			while (1)
			{
				printf("등록된 계좌의 비밀번호를 입력하십시오.:");
				scanf("%hd", &input_pw);

				if (check_password(human_list, account_index, input_pw) == 1)
				{
					printf("비밀번호가 올바르게 입력되었습니다.\n");
					break;
				}

				else
				{
					printf("비밀번호가 틀립니다.\n");
				}
			}

			printf("출금할 금액을 입력하여 주십시오.:");
			scanf("%lld", &out_money);

			if (withdraw(&original_money, out_money) == 1)
			{
				printf("출금이 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n",
					original_money);

				save_log(
					log_list,
					&log_count,
					"출금",
					out_money,
					original_money
				);

				while (1)
				{
					char in_want_work[N];

					printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
					scanf("%s", in_want_work);

					if (strcmp(in_want_work, "네") == 0)
					{
						check = 1;
						break;
					}

					else if (strcmp(in_want_work, "아니오") == 0)
					{
						break;
					}

					else
					{
						printf("입력값이 잘못 되었습니다.\n");
					}
				}

				if (check == 1)
					break;
			}

			else
			{
				printf("출금 액수가 올바르지 않거나 잔액보다 큽니다.\n");
			}
		}

		// 이체
		else if (strcmp(want_work, "이체") == 0)
		{
			short input_pw;

			while (1)
			{
				printf("등록된 계좌의 비밀번호를 입력하십시오.:");
				scanf("%hd", &input_pw);

				if (check_password(human_list, account_index, input_pw) == 1)
				{
					printf("비밀번호가 올바르게 입력되었습니다.\n");
					break;
				}

				else
				{
					printf("비밀번호가 틀립니다.\n");
				}
			}

			printf("이체할 금액을 입력하여 주십시오.:");
			scanf("%lld", &out_money);

			if (transfer(&original_money, out_money) == 1)
			{
				printf("이체가 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n",
					original_money);

				save_log(
					log_list,
					&log_count,
					"이체",
					out_money,
					original_money
				);

				while (1)
				{
					char in_want_work[N];

					printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
					scanf("%s", in_want_work);

					if (strcmp(in_want_work, "네") == 0)
					{
						check = 1;
						break;
					}

					else if (strcmp(in_want_work, "아니오") == 0)
					{
						break;
					}

					else
					{
						printf("입력값이 잘못 되었습니다.\n");
					}
				}

				if (check == 1)
					break;
			}

			else
			{
				printf("이체 액수가 올바르지 않거나 잔액보다 큽니다.\n");
			}
		}

		// 잔액조회
		else if (strcmp(want_work, "잔액조회") == 0)
		{
			short input_pw;

			while (1)
			{
				printf("등록된 계좌의 비밀번호를 입력하십시오.:");
				scanf("%hd", &input_pw);

				if (check_password(human_list, account_index, input_pw) == 1)
				{
					printf("비밀번호가 올바르게 입력되었습니다.\n");
					break;
				}

				else
				{
					printf("비밀번호가 틀립니다.\n");
				}
			}

			show_balance(original_money);

			save_log(
				log_list,
				&log_count,
				"잔액조회",
				0,
				original_money
			);

			while (1)
			{
				char in_want_work[N];

				printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
				scanf("%s", in_want_work);

				if (strcmp(in_want_work, "네") == 0)
				{
					check = 1;
					break;
				}

				else if (strcmp(in_want_work, "아니오") == 0)
				{
					break;
				}

				else
				{
					printf("입력값이 잘못 되었습니다.\n");
				}
			}

			if (check == 1)
				break;
		}

		// 종료
		else if (strcmp(want_work, "종료") == 0)
		{
			printf("프로그램이 종료 됩니다.\n");
			return 0;
		}

		else
		{
			printf("입력값이 잘못 되었습니다.\n");
		}
	}

	// 작업 내역 확인
	if (check == 1)
	{
		char input_name[N];

		while (1)
		{
			printf("계좌 소유주의 이름을 입력하십시오.:");
			scanf("%s", input_name);

			if (check_name(
				human_list,
				account_index,
				input_name
			) == 1)
			{
				printf("실명 확인 되었습니다.\n");
				break;
			}

			else
			{
				printf("귀하의 성명이 확인되지 않습니다.\n");
			}
		}

		show_log(log_list, log_count);
	}

	return 0;
}
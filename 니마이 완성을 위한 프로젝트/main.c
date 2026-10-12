#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "banking.h"

int main(void)
{
	struct human human_list[MAX_ACCOUNTS];

	struct log log_list[N];

	int log_count = 0;
	int account_count;
	int input_result;
	long long menu;
	const char *file_name = "customer_info.txt";
	int account_index;
	int check = 0;

	long long previous_balance;
	long long input_ac_num;
	long long in_money;
	long long out_money;

	char want_work[N];

	account_count = read_file(file_name, human_list, MAX_ACCOUNTS);
	if (account_count < 0)
	{
		printf("계좌 파일을 읽을 수 없거나 데이터 형식이 잘못되었습니다.\n");
		return 1;
	}
	if (account_count == 0) printf("등록된 계좌가 없습니다. 신규 사용자를 등록하십시오.\n");

start_menu:
	check = 0;
	log_count = 0;
	account_index = -1;
	while (1)
	{
		printf("========== BANKING SYSTEM ==========\n\n");
		printf("1. 기존 계좌 로그인\n2. 신규 사용자 등록\n3. 프로그램 종료\n\n");
		printf("원하시는 작업을 선택하십시오.:");
		input_result = read_number(&menu);
		if (input_result == -1) goto finish;
		if (input_result != 1)
		{
			printf("입력값이 잘못 되었습니다.\n");
			continue;
		}
		if (menu == 3) goto finish;
		if (menu == 2)
		{
			struct human new_user;
			if (account_count >= MAX_ACCOUNTS)
			{
				printf("최대 계좌 개수를 초과하여 등록할 수 없습니다.\n메인 메뉴로 돌아갑니다.\n");
				continue;
			}
			printf("신규 사용자 이름을 입력하십시오.:");
			input_result = read_text(new_user.name, N);
			if (input_result == -1) goto finish;
			if (input_result != 1)
			{
				printf("이름이 잘못되었습니다. 사용자 등록이 취소되었습니다.\n");
				continue;
			}
			printf("등록할 계좌번호를 입력하십시오.:");
			input_result = read_number(&new_user.ac_num);
			if (input_result == -1) goto finish;
			if (input_result != 1 || new_user.ac_num <= 0)
			{
				printf("계좌번호가 잘못되었습니다. 사용자 등록이 취소되었습니다.\n");
				continue;
			}
			if (find_account(human_list, account_count, new_user.ac_num) != -1)
			{
				printf("이미 등록된 계좌번호입니다.\n사용자 등록이 취소되었습니다.\n\n메인 메뉴로 돌아갑니다.\n");
				continue;
			}
			printf("등록할 비밀번호를 입력하십시오.:");
			input_result = read_password(&new_user.pw);
			if (input_result == -1) goto finish;
			if (input_result != 1)
			{
				printf("비밀번호가 잘못되었습니다. 사용자 등록이 취소되었습니다.\n");
				continue;
			}
			new_user.balance = 0;
			human_list[account_count] = new_user;
			if (save_file(file_name, human_list, account_count + 1) != 0)
			{
				printf("파일 저장에 실패하여 사용자 등록이 취소되었습니다.\n메인 메뉴로 돌아갑니다.\n");
				continue;
			}
			account_count++;
			printf("신규 사용자 등록이 완료되었습니다.\n\n계좌번호: %lld\n이름: %s\n초기 잔액: 0원\n\n메인 메뉴로 돌아갑니다.\n", new_user.ac_num, new_user.name);
			continue;
		}
		if (menu != 1)
		{
			printf("입력값이 잘못 되었습니다.\n");
			continue;
		}
		printf("등록하신 계좌번호를 입력하십시오.:");
		input_result = read_number(&input_ac_num);
		if (input_result == -1) goto finish;
		if (input_result != 1)
		{
			printf("입력값이 잘못 되었습니다.\n");
			continue;
		}
		account_index = find_account(human_list, account_count, input_ac_num);
		if (account_index == -1)
		{
			printf("입력하신 계좌번호를 찾을 수 없습니다.\n");
			continue;
		}
		{
			short input_pw;
			printf("등록된 계좌의 비밀번호를 입력하십시오.:");
			input_result = read_password(&input_pw);
			if (input_result == -1) goto finish;
			if (input_result != 1 || !check_password(human_list, account_index, input_pw))
			{
				printf("비밀번호가 틀립니다.\n");
				continue;
			}
		}
		break;
	}

	while (1)
	{
		printf("원하시는 작업을 입력해주세요( 입금, 출금, 이체, 잔액조회, 메인메뉴 ):");
		input_result = read_text(want_work, N);
		if (input_result == -1) goto finish;
		if (input_result != 1)
		{
			printf("입력값이 잘못 되었습니다.\n");
			continue;
		}

		// 입금
		if (strcmp(want_work, "입금") == 0)
		{
			printf("입금할 금액을 입력하여 주십시오.:");
			input_result = read_number(&in_money);
			if (input_result == -1) goto finish;
			if (input_result != 1)
			{
				printf("입력값이 잘못 되었습니다.\n");
				continue;
			}

			if (in_money <= 0 || in_money > LLONG_MAX - human_list[account_index].balance)
			{
				printf("입금 금액이 올바르지 않습니다.\n");
			}

			else
			{
				previous_balance = human_list[account_index].balance;
				deposit(&human_list[account_index].balance, in_money);
				if (save_file(file_name, human_list, account_count) != 0)
				{
					human_list[account_index].balance = previous_balance;
					printf("파일 저장에 실패하여 거래가 취소되었습니다.\n");
					continue;
				}

				printf("입금이 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n",
					human_list[account_index].balance);

				save_log(
					log_list,
					&log_count,
					"입금",
					in_money,
					human_list[account_index].balance
				);

				while (1)
				{
					char in_want_work[N];

					printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
					input_result = read_text(in_want_work, N);
					if (input_result == -1) goto finish;
					if (input_result != 1)
					{
						printf("입력값이 잘못 되었습니다.\n");
						continue;
					}

					if (strcmp(in_want_work, "네") == 0)
					{
						check = 1;
						break;
					}

					else if (strcmp(in_want_work, "아니오") == 0)
					{
						check = 2;
						break;
					}

					else
					{
						printf("입력값이 잘못 되었습니다.\n");
					}
				}

				if (check != 0)
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
				input_result = read_password(&input_pw);
				if (input_result == -1) goto finish;
				if (input_result != 1)
				{
					printf("입력값이 잘못 되었습니다.\n");
					continue;
				}

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
			input_result = read_number(&out_money);
			if (input_result == -1) goto finish;
			if (input_result != 1)
			{
				printf("입력값이 잘못 되었습니다.\n");
				continue;
			}

			previous_balance = human_list[account_index].balance;
			if (withdraw(&human_list[account_index].balance, out_money) == 1)
			{
				if (save_file(file_name, human_list, account_count) != 0)
				{
					human_list[account_index].balance = previous_balance;
					printf("파일 저장에 실패하여 거래가 취소되었습니다.\n");
					continue;
				}
				printf("출금이 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n",
					human_list[account_index].balance);

				save_log(
					log_list,
					&log_count,
					"출금",
					out_money,
					human_list[account_index].balance
				);

				while (1)
				{
					char in_want_work[N];

					printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
					input_result = read_text(in_want_work, N);
					if (input_result == -1) goto finish;
					if (input_result != 1)
					{
						printf("입력값이 잘못 되었습니다.\n");
						continue;
					}

					if (strcmp(in_want_work, "네") == 0)
					{
						check = 1;
						break;
					}

					else if (strcmp(in_want_work, "아니오") == 0)
					{
						check = 2;
						break;
					}

					else
					{
						printf("입력값이 잘못 되었습니다.\n");
					}
				}

				if (check != 0)
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
				input_result = read_password(&input_pw);
				if (input_result == -1) goto finish;
				if (input_result != 1)
				{
					printf("입력값이 잘못 되었습니다.\n");
					continue;
				}

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

			printf("현재 이체는 송금 계좌에서 차감만 하며 수취 계좌에 입금하지 않습니다.\n");
			printf("이체할 금액을 입력하여 주십시오.:");
			input_result = read_number(&out_money);
			if (input_result == -1) goto finish;
			if (input_result != 1)
			{
				printf("입력값이 잘못 되었습니다.\n");
				continue;
			}

			previous_balance = human_list[account_index].balance;
			if (transfer(&human_list[account_index].balance, out_money) == 1)
			{
				if (save_file(file_name, human_list, account_count) != 0)
				{
					human_list[account_index].balance = previous_balance;
					printf("파일 저장에 실패하여 거래가 취소되었습니다.\n");
					continue;
				}
				printf("이체 금액 차감이 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n",
					human_list[account_index].balance);

				save_log(
					log_list,
					&log_count,
					"이체",
					out_money,
					human_list[account_index].balance
				);

				while (1)
				{
					char in_want_work[N];

					printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
					input_result = read_text(in_want_work, N);
					if (input_result == -1) goto finish;
					if (input_result != 1)
					{
						printf("입력값이 잘못 되었습니다.\n");
						continue;
					}

					if (strcmp(in_want_work, "네") == 0)
					{
						check = 1;
						break;
					}

					else if (strcmp(in_want_work, "아니오") == 0)
					{
						check = 2;
						break;
					}

					else
					{
						printf("입력값이 잘못 되었습니다.\n");
					}
				}

				if (check != 0)
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
				input_result = read_password(&input_pw);
				if (input_result == -1) goto finish;
				if (input_result != 1)
				{
					printf("입력값이 잘못 되었습니다.\n");
					continue;
				}

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

			show_balance(human_list[account_index].balance);

			save_log(
				log_list,
				&log_count,
				"잔액조회",
				0,
				human_list[account_index].balance
			);

			while (1)
			{
				char in_want_work[N];

				printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
				input_result = read_text(in_want_work, N);
				if (input_result == -1) goto finish;
				if (input_result != 1)
				{
					printf("입력값이 잘못 되었습니다.\n");
					continue;
				}

				if (strcmp(in_want_work, "네") == 0)
				{
					check = 1;
					break;
				}

				else if (strcmp(in_want_work, "아니오") == 0)
				{
					check = 2;
					break;
				}

				else
				{
					printf("입력값이 잘못 되었습니다.\n");
				}
			}

			if (check != 0)
				break;
		}

		// 종료
		else if (strcmp(want_work, "메인메뉴") == 0 || strcmp(want_work, "종료") == 0)
		{
			printf("시작 메뉴로 돌아갑니다.\n\n");
			goto start_menu;
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
			input_result = read_text(input_name, N);
			if (input_result == -1) goto finish;
			if (input_result != 1)
			{
				printf("입력값이 잘못 되었습니다.\n");
				continue;
			}

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
	printf("시작 메뉴로 돌아갑니다.\n\n");
	goto start_menu;

finish:
	if (save_file(file_name, human_list, account_count) != 0)
	{
		printf("계좌 파일 저장에 실패했습니다.\n");
		return 1;
	}
	return 0;
}

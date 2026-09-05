#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 20
#include <string.h>
struct human
{
	long long ac_num;// 등록되어 있는 계좌번호
	short pw; // 등록되어 있는 비밀번호
};
int main(void)
{
	struct human human_list[N] = {{100125652351, 1001}};
	long long input_ac_num; // 계좌 번호 입력
	printf("등록하신 계좌번호를 입력하십시오.:");
	scanf("%lld", &input_ac_num);

	int i, found = 0;
	for (i = 0; i < N; i++) { // 등록된 계좌번호가 나올 때 까지 반복
		if (input_ac_num == human_list[i].ac_num) // 입력된 계좌번호와 등록된 계좌번호가 같지 않을 때
		{
			found = 1;
			break;

		}

	}
	if (found == 0)
		printf("입력하신 계좌번호를 찾을 수 없습니다.\n");
	else // 계좌번호가 확인되면 메인 메뉴로 진입
	{
		long long original_money = 10000, out_money, in_money; //계좌 잔액 10,000\으로 설정, 변경 후 잔액 0으로 초기화
		char want_work[N];
		while (1) // 프로그램이 종료되면 잔액이 초기화 되어서 반복문으로 막음
		{
			printf("원하시는 작업을 입력해주세요( 입금, 출금, 이체, 잔액조회, 종료 ):");
			scanf("%s", want_work);

			if (strcmp(want_work, "입금") == 0) //입금 구현 
			{
				printf("입금할 금액을 입력하여 주십시오.:");
				scanf("%lld", &in_money);
				original_money = original_money + in_money;
				printf("입금이 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n", original_money);
			}
			else if (strcmp(want_work, "출금") == 0) //출금 구현, 비밀번호 필요 
			{
				short input_pw;
				int pw_found = 0;
				while (1) {
					printf("등록된 계좌의 비밀번호를 입력하십시오.:");
					scanf("%hd", &input_pw);
					int i;
					for (i = 0; i < N; i++)
					{
						if (input_pw == human_list[i].pw) // 비밀번호가 올바르게 입력 되었을 때
						{
							printf("비밀번호가 올바르게 입력되었습니다.");
							pw_found = 1;
							break;
						}

					}
					if (pw_found == 0)
						printf("비밀번호가 틀립니다.\n"); // 비밀번호가 올바르게 입력되지 않았을 때
					else
						break; //올바르게 비밀번호 입력하면 무한루프 탈출
				}
				if (pw_found == 1) {
					printf("출금할 금액을 입력하여 주십시오.:");
					scanf("%lld", &out_money);
					if (out_money > original_money)
						printf("출금 액수가 잔액보다 큽니다.\n");
					else
					{
						original_money = original_money - out_money;
						printf("출금이 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n", original_money);
					}
				}
			}
			else if (strcmp(want_work, "이체") == 0) //이체 구현, 비밀번호 필요 
			{
				short input_pw;
				int pw_found = 0;
				while (1) {
					printf("등록된 계좌의 비밀번호를 입력하십시오.:");
					scanf("%hd", &input_pw);
					int i;
					for (i = 0; i < N; i++)
					{
						if (input_pw == human_list[i].pw) // 비밀번호가 올바르게 입력 되었을 때
						{
							printf("비밀번호가 올바르게 입력되었습니다.");
							pw_found = 1;
							break;
						}

					}
					if (pw_found == 1)
						break;
				}
					if (pw_found == 0)
						printf("비밀번호가 틀립니다.\n"); // 비밀번호가 올바르게 입력되지 않았을 때
					else
						break; //올바르게 비밀번호 입력하면 무한루프 탈출
					if (pw_found == 1) {
						printf("이체할 금액을 입력하여 주십시오.:");
						scanf("%lld", &out_money);
						if (out_money > original_money)
							printf("이체 액수가 잔액보다 큽니다.\n");
						else
						{
							original_money = original_money - out_money;
							printf("이체가 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n", original_money);
						}
					}
			}
			else if (strcmp(want_work, "잔액조회") == 0) // 잔액조회 구현, 비밀번호 필요
			{
				short input_pw;
				int pw_found = 0;
				while (1) 
				{
					printf("등록된 계좌의 비밀번호를 입력하십시오.:");
					scanf("%hd", &input_pw);
					int i;
					for (i = 0; i < N; i++)
					{
						if (input_pw == human_list[i].pw) // 비밀번호가 올바르게 입력 되었을 때
						{
							printf("비밀번호가 올바르게 입력되었습니다.");
							pw_found = 1;
							break;
						}

					}
					if (pw_found == 0)
						printf("비밀번호가 틀립니다.\n"); // 비밀번호가 올바르게 입력되지 않았을 때
					else
						break; //올바르게 비밀번호 입력하면 무한루프 탈출
				}
				printf("현재 귀하의 잔액은 %lld원 입니다.\n", original_money); // 잘못된 값 입력 시 나타나는 화면
			}
			else if (strcmp(want_work, "종료") == 0) // 프로그램 종료 구현
			{
				printf("프로그램이 종료 됩니다.\n");
				break;
			}
			else
				printf("입력값이 잘못 되었습니다.\n");// 메뉴값 외 다른 값 입력시 출력
		}
	}
	return 0;
}
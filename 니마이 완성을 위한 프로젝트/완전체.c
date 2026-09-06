#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 20
#include <string.h>

struct human
{
	long long ac_num;// 등록되어 있는 계좌번호
	short pw; // 등록되어 있는 비밀번호
	char name[N]; //등록되어 있는 이름
};

struct log
{
	char work[N]; // 작업 종류
	long long money; // 거래 금액
	long long balance; // 거래 후 잔액
};

int main(void)
{
	int check = 0; // 마지막 추가 작업 선택에서 사용됨

	struct log log_list[N]; // 작업 내역 저장
	int log_count = 0; // 저장된 작업 내역 개수

	struct human human_list[N] = {
		{100152351124, 1524, "김백제"},
		{100144523657, 1275, "이온조"},
		{100142566328, 1685, "최쿠농"}
	}; // 전산에 등록되어 있는 정보

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
		long long original_money = 10000, out_money, in_money; //계좌 잔액 10,000\으로 설정
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

				// 입금 작업 로그 저장
				if (log_count < N)
				{
					strcpy(log_list[log_count].work, "입금");
					log_list[log_count].money = in_money;
					log_list[log_count].balance = original_money;
					log_count++;
				}

				while (1)
				{
					char in_want_work[N];

					printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
					scanf("%s", in_want_work);

					if (strcmp(in_want_work, "아니오") == 0)
					{
						printf("프로그램이 종료됩니다.\n");
						break;
					}
					else if (strcmp(in_want_work, "네") == 0)
					{
						check = 1;
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

			else if (strcmp(want_work, "출금") == 0) //출금 구현, 비밀번호 필요 
			{
				short input_pw;
				int pw_found = 0;

				while (1)
				{
					printf("등록된 계좌의 비밀번호를 입력하십시오.:");
					scanf("%hd", &input_pw);

					if (input_pw == human_list[i].pw) // 비밀번호가 올바르게 입력 되었을 때
					{
						printf("비밀번호가 올바르게 입력되었습니다.\n");
						pw_found = 1;
						break;
					}

					if (pw_found == 0)
						printf("비밀번호가 틀립니다.\n"); // 비밀번호가 올바르게 입력되지 않았을 때
					else
						break; //올바르게 비밀번호 입력하면 무한루프 탈출
				}

				if (pw_found == 1)
				{
					printf("출금할 금액을 입력하여 주십시오.:");
					scanf("%lld", &out_money);

					if (out_money > original_money)
						printf("출금 액수가 잔액보다 큽니다.\n");

					else
					{
						original_money = original_money - out_money;

						printf("출금이 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n", original_money);

						// 출금 작업 로그 저장
						if (log_count < N)
						{
							strcpy(log_list[log_count].work, "출금");
							log_list[log_count].money = out_money;
							log_list[log_count].balance = original_money;
							log_count++;
						}

						while (1)
						{
							char in_want_work[N];

							printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
							scanf("%s", in_want_work);

							if (strcmp(in_want_work, "아니오") == 0)
							{
								printf("프로그램이 종료됩니다.\n");
								break;
							}
							else if (strcmp(in_want_work, "네") == 0)
							{
								check = 1;
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
			}

			else if (strcmp(want_work, "이체") == 0) //이체 구현, 비밀번호 필요 
			{
				short input_pw;
				int pw_found = 0;

				while (1)
				{
					printf("등록된 계좌의 비밀번호를 입력하십시오.:");
					scanf("%hd", &input_pw);

					if (input_pw == human_list[i].pw) // 비밀번호가 올바르게 입력 되었을 때
					{
						printf("비밀번호가 올바르게 입력되었습니다.\n");
						pw_found = 1;
						break;
					}
				}

				if (pw_found == 0)
					printf("비밀번호가 틀립니다.\n"); // 비밀번호가 올바르게 입력되지 않았을 때

				if (pw_found == 1)
				{
					printf("이체할 금액을 입력하여 주십시오.:");
					scanf("%lld", &out_money);

					if (out_money > original_money)
						printf("이체 액수가 잔액보다 큽니다.\n");

					else
					{
						original_money = original_money - out_money;

						printf("이체가 완료 되었습니다. 현재 잔액은 %lld원 입니다.\n", original_money);

						// 이체 작업 로그 저장
						if (log_count < N)
						{
							strcpy(log_list[log_count].work, "이체");
							log_list[log_count].money = out_money;
							log_list[log_count].balance = original_money;
							log_count++;
						}

						while (1)
						{
							char in_want_work[N];

							printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
							scanf("%s", in_want_work);

							if (strcmp(in_want_work, "아니오") == 0)
							{
								printf("프로그램이 종료됩니다.\n");
								break;
							}
							else if (strcmp(in_want_work, "네") == 0)
							{
								check = 1;
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
			}

			else if (strcmp(want_work, "잔액조회") == 0) // 잔액조회 구현, 비밀번호 필요
			{
				short input_pw;
				int pw_found = 0;

				while (1)
				{
					printf("등록된 계좌의 비밀번호를 입력하십시오.:");
					scanf("%hd", &input_pw);

					if (input_pw == human_list[i].pw) // 비밀번호가 올바르게 입력 되었을 때
					{
						printf("비밀번호가 올바르게 입력되었습니다.\n");
						pw_found = 1;
						break;
					}

					if (pw_found == 0)
						printf("비밀번호가 틀립니다.\n"); // 비밀번호가 올바르게 입력되지 않았을 때
					else
						break; //올바르게 비밀번호 입력하면 무한루프 탈출
				}

				printf("현재 귀하의 잔액은 %lld원 입니다.\n", original_money); // 잘못된 값 입력 시 나타나는 화면

				// 잔액조회 작업 로그 저장
				if (log_count < N)
				{
					strcpy(log_list[log_count].work, "잔액조회");
					log_list[log_count].money = 0;
					log_list[log_count].balance = original_money;
					log_count++;
				}

				while (1)
				{
					char in_want_work[N];

					printf("작업내용을 확인 하시겠습니까?(네 또는 아니오):");
					scanf("%s", in_want_work);

					if (strcmp(in_want_work, "아니오") == 0)
					{
						printf("프로그램이 종료됩니다.\n");
						break;
					}
					else if (strcmp(in_want_work, "네") == 0)
					{
						check = 1;
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

			else if (strcmp(want_work, "종료") == 0) // 프로그램 종료 구현
			{
				printf("프로그램이 종료 됩니다.\n");
				break;
			}

			else
				printf("입력값이 잘못 되었습니다.\n");// 메뉴값 외 다른 값 입력시 출력
		}
	}

	if (check == 1)// 추가항목 "네"선택시 실행
	{
		char input_name[N]; // 기계에 입력되는 이름
		int name_found = 0;

		while (1)
		{
			printf("계좌 소유주의 이름을 입력하십시오.:"); // 확인을 위한 이름 입력받음
			scanf("%s", input_name);

			if (strcmp(human_list[i].name, input_name) == 0) // 입력받은거랑 기계에 있는거랑 같으면 반복 탈출
			{
				printf("실명 확인 되었습니다.\n");
				name_found = 1;
				break;
			}
			else
				name_found = 0;

			if (name_found == 1)
				break;

			if (name_found == 0)
				printf("귀하의 성명이 확인되지 않습니다.\n"); // 이름이 확인되지 않으면 실명 다시 입력할 수 있도록 무한루프
		}

		if (name_found == 1) //로그 확인 명령어
		{
			if (log_count == 0)
			{
				printf("확인할 작업 내역이 없습니다.\n");
			}
			else
			{
				int j;

				printf("\n===== 작업 내역 =====\n");

				for (j = 0; j < log_count; j++)
				{
					if (strcmp(log_list[j].work, "잔액조회") == 0)
					{
						printf("%d. %s / 현재 잔액 %lld원\n",
							j + 1,
							log_list[j].work,
							log_list[j].balance);
					}
					else
					{
						printf("%d. %s / %lld원 / 거래 후 잔액 %lld원\n",
							j + 1,
							log_list[j].work,
							log_list[j].money,
							log_list[j].balance);
					}
				}
			}
		}
	}

	return 0;
}
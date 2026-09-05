/*#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 40
#include <string.h> // 문자열 비교 함수(strcmp()) 사용하기 위하여 호출
int main()
{
	long long original_money = 10000,out_money, in_money; //계좌 잔액 10,000\으로 설정, 변경 후 잔액 0으로 초기화
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
		else if (strcmp(want_work, "이체") == 0) //출금 구현, 비밀번호 필요 
		{
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
		else if (strcmp(want_work, "잔액조회") == 0) // 잔액조회 구현, 비밀번호 필요
		{
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
	return 0;
} */
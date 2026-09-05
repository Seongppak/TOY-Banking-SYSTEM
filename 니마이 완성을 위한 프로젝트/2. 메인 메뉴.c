#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 20
int main()
{
	long long original_money = 10000, in_money, after_money = 0;
	char want_work[N];
	printf("원하시는 작업을 입력해주세요( 입금, 출금, 이체, 잔액조회 ):");
	scanf("%s", want_work);
	if (want_work[N] == "입금")
	{
		printf("입금할 금액을 입력하여 주십시오.:");
		scanf("%lld", &in_money);
		after_money = original_money + in_money;
		printf("입금이 완료 되었습니다. 현재 잔액은 %lld원 입니다.", after_money);
		

	}
		
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 20
struct human
{
	long long ac_num[N]; // 등록되어 있는 계좌번호
};
int main(void)
{
	struct human human_list = { { 100125652351 } };
	long long input_ac_num; // 계좌 번호 입력
	printf("등록하신 계좌번호를 입력하십시오.:");
	scanf("%lld", &input_ac_num);
	
	int i, found = 0; 
	for (i = 0; i < N; i++) { // 등록된 계좌번호가 나올 때 까지 반복
		if (input_ac_num == human_list.ac_num[i]) // 입력된 계좌번호와 등록된 계좌번호가 같지 않을 때
		{
			found = 1;
			break;

		}
		
	}
	if (found == 0)
		printf("입력하신 계좌번호를 찾을 수 없습니다.\n");
	return 0;
} 
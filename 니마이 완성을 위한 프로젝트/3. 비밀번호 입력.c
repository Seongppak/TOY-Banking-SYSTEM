#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 20
struct human { short pw[N]; };
int main()

{
	struct human human_list = { 1001 };
	short input_pw;
	int found = 0;
	while (1) {
		printf("등록된 계좌의 비밀번호를 입력하십시오.:");
		scanf("%hd", &input_pw);
		int i;
		for (i = 0; i < N; i++)
		{
			if (input_pw == human_list.pw[i]) // 비밀번호가 올바르게 입력 되었을 때
			{
				printf("비밀번호가 올바르게 입력되었습니다.");
				found = 1;
				break;
			}

		}
		if (found == 0)
			printf("비밀번호가 틀립니다.\n"); // 비밀번호가 올바르게 입력되지 않았을 때
		else
			break; //올바르게 비밀번호 입력하면 무한루프 탈출
	}
	return 0;
}


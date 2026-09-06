#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 20
#include <string.h>
int main(void)
{

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
			printf("작업 조회가 시작됩니다.\n");
			break;
		}
		else
		{
			printf("입력값이 잘못 되었습니다.\n");
		}
	}

	return 0;
}
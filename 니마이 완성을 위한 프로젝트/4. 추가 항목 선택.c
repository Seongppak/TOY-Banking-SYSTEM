/*#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 20
#include <string.h>
struct human { char name[N]; };
int main() {
	struct human human_list[N] = { {"김백제"},{"이공주"},{"최쿠농"} }; // 전산에 등록된 인물들
		int check = 0;
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
				check = 1;
				break;

			}
			else
			{
				printf("입력값이 잘못 되었습니다.\n");
			}
		}
		if (check == 1) // 추가 내용 네 누르면 실행
		{
			char input_name[N]; // 기계에 입력되는 이름
			int name_found = 0;
			while (1) {

				printf("계좌 소유주의 이름을 입력하십시오.:"); // 확인을 위한 이름 입력받음
				scanf("%s", input_name);
				int i;
				for (i = 0; i < N; i++)
				{
					if (strcmp(human_list[i].name, input_name) == 0) // 입력받은거랑 기계에 있는거랑 같으면 반복 탈출
					{
						printf("실명 확인 되었습니다.\n");
						name_found = 1;
						break;
					}
					else
						name_found = 0;


				}
				if (name_found == 1)
					break;
				if (name_found == 0)
					printf("귀하의 성명이 확인되지 않습니다.\n"); // 이름이 확인되지 않으면 실명 다시 입력할 수 있도록 무한루프
			}
		}

	return 0;
} */
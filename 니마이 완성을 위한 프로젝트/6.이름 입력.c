#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 20
#include <string.h>
struct human { char name[N]; };
int main(){
	struct human human_list[N] = { {"김백제"},{"이공주"},{"최쿠농"} }; // 전산에 등록된 인물들
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
	
	
		return 0;
}
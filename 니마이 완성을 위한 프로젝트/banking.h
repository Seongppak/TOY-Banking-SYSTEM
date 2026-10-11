#pragma once

#include <stdio.h>

#define N 20
#define MAX_ACCOUNTS 100

struct human
{
	long long ac_num; // 등록되어 있는 계좌번호
	short pw; // 등록되어 있는 비밀번호
	char name[N]; // 등록되어 있는 이름
	long long balance;
};

struct log
{
	char work[N]; // 작업 종류
	long long money; // 거래 금액
	long long balance; // 거래 후 잔액
};

// 계좌 검색
int find_account(struct human human_list[], int count, long long input_ac_num);

// 비밀번호 확인
int check_password(struct human human_list[], int account_index, short input_pw);

// 입금
void deposit(long long* balance, long long in_money);

// 출금
int withdraw(long long* balance, long long out_money);

// 이체
int transfer(long long* balance, long long out_money);

// 계좌 간 이체 결과
enum transfer_result
{
	TRANSFER_INVALID = 0,
	TRANSFER_SUCCESS = 1,
	TRANSFER_SAVE_ERROR = -1,
	TRANSFER_BALANCE_OVERFLOW = -2
};

// 계좌 간 이체 및 저장 (실패 시 양쪽 잔액 복구)
int transfer_between_accounts(
	const char *file_name,
	struct human human_list[],
	int account_count,
	int sender_index,
	int receiver_index,
	long long out_money
);

// 잔액조회
void show_balance(long long balance);

// 이름 확인
int check_name(struct human human_list[], int account_index, char input_name[]);

// 로그 저장
void save_log(
	struct log log_list[],
	int* log_count,
	char work[],
	long long money,
	long long balance
);

// 로그 출력
void show_log(struct log log_list[], int log_count);
// 계좌 파일 읽기 및 저장 (실패: -1, 빈 파일: 0)
int read_file(const char *file_name, struct human human_list[], int max_accounts);
int save_file(const char *file_name, const struct human human_list[], int account_count);

// 범위를 검사하는 입력 (성공: 1, 잘못된 입력: 0, 입력 종료: -1)
int read_line(FILE *file, char text[], int size);
int read_text(char text[], int size);
int parse_number(const char text[], long long *number);
int read_number(long long *number);
int read_password(short *password);

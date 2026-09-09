#pragma once // #pragma 는 전처리기에 명령을 내릴 때 사용된다. #pragma once 는 한번만 읽으라는 의미 즉, 중복 읽기를 방지하는 명령어이다.
#define N 100
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

/* --- 함수 원형 선언 ---*/ // 헤더파일에 선언을 함으로써, main.c, banking.c에서 선언을 각각 따로 하지 않고 include 를 호출해서 printf()를 사용하듯 하기 위함.
// 계좌번호 검색
int find_account(struct human human_list[], int count, long long input_ac_num);

// 비밀번호 확인
int check_password(struct human human_list[], int count_index, short input_pw);

// 입금
void deposit(long long* balance, long long in_money);

//출금
int withdraw(long long* balance, long long in_money);

//이체 

int transfer(long long* balance, long long out_money);

// 잔액 조회
void show_balance(long long balance);

//이름 확인 
int check_name(struct human human_list[], int account_index, char input_name[]);

//로그 저장
void save_log(struct log log_list[], int* log_count, char work[], long long money, long long blance);

// 로그 출력 
void show_log(struct log log_list[], int log_count);
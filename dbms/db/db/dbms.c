#include <stdio.h>


// 회원 구조체 
struct user
{
	char id[100];
	char password[100];
	struct user* before;
	struct user* after;
	struct db* firsDB;
	struct db* currentDB;
	struct db* nextDB;
};


//디비 구조체 
struct db {
	char name[100];
	struct db* before;
	struct db* after;
	struct table* firstTable;
	struct table* currentTable;
	struct table* nextTable;
};

//테이블 구조체
struct talbe {
	char name[100];
	struct field* field;
	struct table* before;
	struct table* after;
	struct tuple* firstData;
	struct tuple* currentData;
	struct tuple* nextData;

};

// 튜플 구조체 
struct tuple {
	//  char[100] name;
	//  char[20] dataType;
	struct field* field;
	struct data* data;
	struct field* before;
	struct field* after;
	//   struct data *firstData
	//  struct data *currentData;
	//  struct data *nextData;
};

// 필드 구조체 
struct field {
	char name[100];
	char dataType[20];
	struct field* before;
	struct field* after;
};

//데이터 구조체 
struct data {
	void* data;
	struct data* before;
	struct data* after;
};

// 디비 명령어 공백으로 분리한 값을 넣는 구조체 
struct dividedMsg {
	char msg[100];
	struct dividedMsg* after;
};

int userCount;

// 사용자 전역변수 
struct user* tmpUser;  // 임시로 user를 다룰 때 사용하는 임시 user변수
struct user* firstUser;  // 첫번쨰 user를 가리키는 user포인터
struct user* lastUserr;   // 현재 user를 가리키는 user포인터(가장 마지막에 생성된 user)
struct user* selectedUser;   // 현재 접속중인 user를 가리키는 user 포인터 

// 디비 전역변수 
struct db* tmpDb; // 임시로 db를 다룰 때 사용하는 임시 db포인터변수
struct db* firstDb;  // 현재 유저가 가지고 있는 첫번쨰 db  로그인중이 아니라면 null   user의 firstDb가 이것이 된다
struct db* lastDb; // 현재 유저가 가지고 있는 마지막 db 로그인이 아니면 null user의 firstDb의 before가 있으면 그것이 되고    없다면 fristDb도 없으면 null
struct db* selectedDb;   //현재 유저가 선택한 db

//테이블 전역변수 
struct table* tmpTable; // 임시로 테이블을 다룰 때 사용하는 테이블포인터변수  
struct table* firstTable; //  DB를 선택했을 때 DB의 첫번째 테이블 
struct table* lastTable;  // DB를 선택했을 때 DB의 마지막 테이블 
struct talbe* selectedTable;   //  현재 선택중인 테이블 


//튜플 전역변수 
struct tuple* tmpTuple;
struct tuple* firstTuple;
struct tuple* lastTuple;


//필드 전역변수 
struct field* tmpFieldd;  // 임시로 필드를 다룰 때 사용하는 필드
struct field* firstField;  // 선택한 테이블의 첫번째 필드
struct field* lastField;  // 선택한 테이블의 마지막 필드 
struct field* selectedField; // 현재 선택중인 필드 (사용할지는 모르겠음)



//데이터 전역변수 
struct data* tmpData;  // 임시로 데이터를 다룰 때 사용하는 데이터
struct data* firstData; // 현재 튜플의 첫 데이터
struct data* lastData; // 현재 튜플의 마지막 데이터



struct dividedMsg* divide(char msg[]);//문자열 공백으로 나누는 함수 
int addUser(char id[], char password[]);    //아이디와 패스워드를 받고 계정을 생성하는 함수
int deleteUser(struct user* deleteUser); // 사용자 삭제 함수 
int addTable(char tableName[]);   // 테이블 추가 함수 
int deleteTable(struct table* deleteTable);  // 테이블 삭제 함수 
//int addTuple()  // 튜플 추가 함수 
int deleteTuple(struct tuple* deleteTuple); // 튜플 삭제 함수 
int login(char id[], char password[]);   // 로그인하는 함수 
int logout(void);  // 로그아웃하는 함수 
int userOrder(char order[]);   // 로그인 상태에서 데이터베이스 명령어 사용 함수   CREATE , DROP   ->database 
int createDb(char order[]);  // 디비 생성 함수
int dropDb(char order[]);   // 디비 삭제함수
int useDb(char order[]);  // 디비 사용 함수 
int nuseDb();  // 디비 사용 종료 함수 
int dbOrder(char order[]); // 디비 명령어 사용 함수
int createTable(struct dividedMsg*);  //CREATE TABLE 테이블이름 (필드이름1 타입이름, 필드이릅2 타입이름2 .....);
int dropTable(struct dividedMsg*);  //DROP TABLE 테이블이름;
int insertTable(struct dividedMsg*);// INSERT INTO 테이블이름(필드이름1, 필드이름2, 필드이름3 ...)VALUES(데이터값1,데이터값2,데이터값3....)  OR  INSERT INTO 테이블이름VALUES(데이터값1, 데이터값2,데이터값3.....)
int updateTable(struct divideMsg*); //UPDATE 테이블이름 SET 필드이름1=데이터값1, 필드이름2=데이터값2..... where 조건
int deleteTable(struct divideMsg*); //DELETE FROM 테이블이름 where 조건
int selectTable(struct divideMsg*); //SELECT 필드이름1 , 필드이름2 .... from 테이블이름 where 조건  group by 필드
void showMain();



int main(int argc, char* argv[])
{
	int i = 0;
	char id[100];
	char passwd[100];
	char passwdRe[100];
	int monitor = 0;
	char order[1000];

	while (1) {
		// monitor가 0이면 메인화면 1이면 로그인화면 2면 디비화면을 출력한다 
		switch (monitor) {

			//메인화면 
			case 0 :
				showMain();
			break;
			case 1 :
			break;
			case 2 :
			break;


		}



	}



	return 0;

}


void showMain() {

	printf("■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□■□\n");
	printf("□                                                            ■\n");




}
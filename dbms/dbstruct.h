







// 회원 구조체 
struct user
{
	char id[100];
	char password[100];
	struct user* before;
	struct user* after;
	struct db* firstDB;
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
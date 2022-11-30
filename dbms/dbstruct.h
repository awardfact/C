







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
struct table {
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
	struct field* firstField;
	struct field* currentField;
	struct data* firstData;
	struct data* currentData;
	struct tuple* before;
	struct tuple* after;
	//   struct data *firstData
	//  struct data *currentData;
	//  struct data *nextData;
};

// 필드 구조체 
struct field {
	char name[100];
	char dataType[20];
	int length;
	int notNull;
	struct field* before;
	struct field* after;
};

//데이터 구조체 
struct data {
	void* data;
	char cdata[100];
	float fdata;
	int idata;
	struct data* before;
	struct data* after;
};

// 디비 명령어 공백으로 분리한 값을 넣는 구조체 
struct dividedMsg {
	char msg[100];
	struct dividedMsg* after;
	struct dividedMsg* before;
};


//where에 오는 조건 구조체   타입 1 -> =   2 -> !=  3 -> <  4 -> <=  5 ->  >  6-> >=      ,,   after타입   1 -> or  2 -> and 
struct whereCondition {
	int type;
	char field[100];
	char value[100];
	int afterType;
	struct whereCondition* after;

};
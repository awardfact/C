







// ȸ�� ����ü 
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


//��� ����ü 
struct db {
	char name[100];
	struct db* before;
	struct db* after;
	struct table* firstTable;
	struct table* currentTable;
	struct table* nextTable;
};

//���̺� ����ü
struct talbe {
	char name[100];
	struct field* field;
	struct table* before;
	struct table* after;
	struct tuple* firstData;
	struct tuple* currentData;
	struct tuple* nextData;

};

// Ʃ�� ����ü 
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

// �ʵ� ����ü 
struct field {
	char name[100];
	char dataType[20];
	struct field* before;
	struct field* after;
};

//������ ����ü 
struct data {
	void* data;
	struct data* before;
	struct data* after;
};

// ��� ��ɾ� �������� �и��� ���� �ִ� ����ü 
struct dividedMsg {
	char msg[100];
	struct dividedMsg* after;
};
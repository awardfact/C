







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
struct table {
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

// �ʵ� ����ü 
struct field {
	char name[100];
	char dataType[20];
	int length;
	int notNull;
	struct field* before;
	struct field* after;
};

//������ ����ü 
struct data {
	void* data;
	char cdata[100];
	float fdata;
	int idata;
	struct data* before;
	struct data* after;
};

// ��� ��ɾ� �������� �и��� ���� �ִ� ����ü 
struct dividedMsg {
	char msg[100];
	struct dividedMsg* after;
	struct dividedMsg* before;
};


//where�� ���� ���� ����ü   Ÿ�� 1 -> =   2 -> !=  3 -> <  4 -> <=  5 ->  >  6-> >=      ,,   afterŸ��   1 -> or  2 -> and 
struct whereCondition {
	int type;
	char field[100];
	char value[100];
	int afterType;
	struct whereCondition* after;

};
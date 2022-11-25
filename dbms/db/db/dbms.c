#include <stdio.h>


// ȸ�� ����ü 
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

int userCount;

// ����� �������� 
struct user* tmpUser;  // �ӽ÷� user�� �ٷ� �� ����ϴ� �ӽ� user����
struct user* firstUser;  // ù���� user�� ����Ű�� user������
struct user* lastUserr;   // ���� user�� ����Ű�� user������(���� �������� ������ user)
struct user* selectedUser;   // ���� �������� user�� ����Ű�� user ������ 

// ��� �������� 
struct db* tmpDb; // �ӽ÷� db�� �ٷ� �� ����ϴ� �ӽ� db�����ͺ���
struct db* firstDb;  // ���� ������ ������ �ִ� ù���� db  �α������� �ƴ϶�� null   user�� firstDb�� �̰��� �ȴ�
struct db* lastDb; // ���� ������ ������ �ִ� ������ db �α����� �ƴϸ� null user�� firstDb�� before�� ������ �װ��� �ǰ�    ���ٸ� fristDb�� ������ null
struct db* selectedDb;   //���� ������ ������ db

//���̺� �������� 
struct table* tmpTable; // �ӽ÷� ���̺��� �ٷ� �� ����ϴ� ���̺������ͺ���  
struct table* firstTable; //  DB�� �������� �� DB�� ù��° ���̺� 
struct table* lastTable;  // DB�� �������� �� DB�� ������ ���̺� 
struct talbe* selectedTable;   //  ���� �������� ���̺� 


//Ʃ�� �������� 
struct tuple* tmpTuple;
struct tuple* firstTuple;
struct tuple* lastTuple;


//�ʵ� �������� 
struct field* tmpFieldd;  // �ӽ÷� �ʵ带 �ٷ� �� ����ϴ� �ʵ�
struct field* firstField;  // ������ ���̺��� ù��° �ʵ�
struct field* lastField;  // ������ ���̺��� ������ �ʵ� 
struct field* selectedField; // ���� �������� �ʵ� (��������� �𸣰���)



//������ �������� 
struct data* tmpData;  // �ӽ÷� �����͸� �ٷ� �� ����ϴ� ������
struct data* firstData; // ���� Ʃ���� ù ������
struct data* lastData; // ���� Ʃ���� ������ ������



struct dividedMsg* divide(char msg[]);//���ڿ� �������� ������ �Լ� 
int addUser(char id[], char password[]);    //���̵�� �н����带 �ް� ������ �����ϴ� �Լ�
int deleteUser(struct user* deleteUser); // ����� ���� �Լ� 
int addTable(char tableName[]);   // ���̺� �߰� �Լ� 
int deleteTable(struct table* deleteTable);  // ���̺� ���� �Լ� 
//int addTuple()  // Ʃ�� �߰� �Լ� 
int deleteTuple(struct tuple* deleteTuple); // Ʃ�� ���� �Լ� 
int login(char id[], char password[]);   // �α����ϴ� �Լ� 
int logout(void);  // �α׾ƿ��ϴ� �Լ� 
int userOrder(char order[]);   // �α��� ���¿��� �����ͺ��̽� ��ɾ� ��� �Լ�   CREATE , DROP   ->database 
int createDb(char order[]);  // ��� ���� �Լ�
int dropDb(char order[]);   // ��� �����Լ�
int useDb(char order[]);  // ��� ��� �Լ� 
int nuseDb();  // ��� ��� ���� �Լ� 
int dbOrder(char order[]); // ��� ��ɾ� ��� �Լ�
int createTable(struct dividedMsg*);  //CREATE TABLE ���̺��̸� (�ʵ��̸�1 Ÿ���̸�, �ʵ��̸�2 Ÿ���̸�2 .....);
int dropTable(struct dividedMsg*);  //DROP TABLE ���̺��̸�;
int insertTable(struct dividedMsg*);// INSERT INTO ���̺��̸�(�ʵ��̸�1, �ʵ��̸�2, �ʵ��̸�3 ...)VALUES(�����Ͱ�1,�����Ͱ�2,�����Ͱ�3....)  OR  INSERT INTO ���̺��̸�VALUES(�����Ͱ�1, �����Ͱ�2,�����Ͱ�3.....)
int updateTable(struct divideMsg*); //UPDATE ���̺��̸� SET �ʵ��̸�1=�����Ͱ�1, �ʵ��̸�2=�����Ͱ�2..... where ����
int deleteTable(struct divideMsg*); //DELETE FROM ���̺��̸� where ����
int selectTable(struct divideMsg*); //SELECT �ʵ��̸�1 , �ʵ��̸�2 .... from ���̺��̸� where ����  group by �ʵ�
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
		// monitor�� 0�̸� ����ȭ�� 1�̸� �α���ȭ�� 2�� ���ȭ���� ����Ѵ� 
		switch (monitor) {

			//����ȭ�� 
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

	printf("���������������������������������\n");
	printf("��                                                            ��\n");




}
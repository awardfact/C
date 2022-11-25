#include <stdio.h>
#include <termios.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "dbstruct.h"
#include "dbfunction.h"
#include "dbfile.h"
#include "dbshow.h"
#include "dbuser.h"
#include "dborder.h"
#include "dbdb.h"
#include "dbtable.h"
#include "dbfree.h"

int userCount;

// ����� �������� 
struct user* tmpUser = NULL;  // �ӽ÷� user�� �ٷ� �� ����ϴ� �ӽ� user����
struct user* firstUser = NULL;  // ù���� user�� ����Ű�� user������
struct user* lastUser = NULL;   // ���� user�� ����Ű�� user������(���� �������� ������ user)
struct user* selectedUser = NULL;   // ���� �������� user�� ����Ű�� user ������ 

// ��� �������� 
struct db* tmpDb = NULL; // �ӽ÷� db�� �ٷ� �� ����ϴ� �ӽ� db�����ͺ���
struct db* firstDb = NULL;  // ���� ������ ������ �ִ� ù���� db  �α������� �ƴ϶�� null   user�� firstDb�� �̰��� �ȴ�
struct db* lastDb = NULL; // ���� ������ ������ �ִ� ������ db �α����� �ƴϸ� null user�� firstDb�� before�� ������ �װ��� �ǰ�    ���ٸ� fristDb�� ������ null
struct db* selectedDb = NULL;   //���� ������ ������ db

//���̺� �������� 
struct table* tmpTable = NULL; // �ӽ÷� ���̺��� �ٷ� �� ����ϴ� ���̺������ͺ���  
struct table* firstTable = NULL; //  DB�� �������� �� DB�� ù��° ���̺� 
struct table* lastTable = NULL;  // DB�� �������� �� DB�� ������ ���̺� 
struct talbe* selectedTable = NULL;   //  ���� �������� ���̺� 


//Ʃ�� �������� 
struct tuple* tmpTuple = NULL;
struct tuple* firstTuple = NULL;
struct tuple* lastTuple = NULL;


//�ʵ� �������� 
struct field* tmpField = NULL;  // �ӽ÷� �ʵ带 �ٷ� �� ����ϴ� �ʵ�
struct field* firstField = NULL;  // ������ ���̺��� ù��° �ʵ�
struct field* lastField = NULL;  // ������ ���̺��� ������ �ʵ� 
struct field* selectedField = NULL; // ���� �������� �ʵ� (��������� �𸣰���)



//������ �������� 
struct data* tmpData = NULL;  // �ӽ÷� �����͸� �ٷ� �� ����ϴ� ������
struct data* firstData = NULL; // ���� Ʃ���� ù ������
struct data* lastData = NULL; // ���� Ʃ���� ������ ������

char id[100];
char passwd[100];
char passwdRe[100];
char mainMsg[100];


int main(int argc, char* argv[])
{
	int i = 0;
	char c;

	int monitor = 0;
	char order[1000];
	char getKey = '\0';
	int exit = 0;
	int tmp = 0;

	getUser(&firstUser , &lastUser , &tmpUser);

	while (1) {
		// monitor�� 0�̸� ����ȭ�� 1�̸� �α���ȭ�� 2�� ���ȭ���� ����Ѵ� 
		switch (monitor) {

			//����ȭ�� 
			case 0:
				//getKEy�� Null�̸� �Է��� ����ȭ�鿡�� �Է��� �޴´� 
				if (getKey == '\0') {

					showMain(mainMsg);
					getKey = getch();
				}


				// 0�� �Է������� ���α׷� ���� 
				if (getKey == 48) {
					exit = 1;
				}
				//1�� �Է������� �α��� ȭ������ �̵�
				else if (getKey == 49) {
					if (login(id , passwd, &firstUser, &selectedUser , &tmpUser) == 1) {
						firstDb = selectedUser->firstDB;
						lastDb = selectedUser->currentDB;
						monitor = 1;
						strcpy(mainMsg, "login success.\0");
						getKey = '\0';
					}
					else {
						strcpy(mainMsg, "login fail.\0");
						getKey = '\0';

					}

				}
				//2�� �Է������� ȸ������ ȭ������ �̵� 
				else if (getKey == 50) {
					// ȸ������ �����ϸ� ������������ �̵��ϰ� ���� ��� 
					if (signUp(&firstUser , &lastUser,&tmpUser) == 1) {
						strcpy(mainMsg, "sign up success!\0");
						getKey = '\0';
					}
					// ȸ������ �����ϸ� ������������ �̵��ϰ� ���� ��� 
					else {
						strcpy(mainMsg, "sign up fail.\0");
						getKey = '\0';
					}
				//3�� �Է��ϸ� ȸ�� Ż�� ȭ������ �̵�
				}else if (getKey == 51) {
					// ȸ��Ż�� �����ϸ� ������������ �̵��ϰ� ���� ��� 
					if (signOut(&firstUser, &lastUser,&tmpUser) == 1) {
						strcpy(mainMsg, "sign out success!\0");
						getKey = '\0';


					}
					// ȸ��Ż�� �����ϸ� ������������ �̵��ϰ� ���� ��� 
					else {
						strcpy(mainMsg, "sign out fail.\0");
						getKey = '\0';
					}

				}

				else {
					getKey = '\0';

				}


				break;
			//�α��� ȭ�� 
			case 1:
				//getKEy�� Null�̸� �Է��� ����ȭ�鿡�� �Է��� �޴´� 
				if (getKey == '\0') {
				//	printf("%s \n", selectedUser->id);
					showLogin(mainMsg , selectedUser->id);
					getKey = getch();
				}

				// 0�� �Է������� ���α׷� ���� 
				if (getKey == 48) {
					exit = 1;
				}
				else if (getKey == 49) {
					tmp = userOrder(&selectedUser, &tmpDb, &firstDb, &lastDb , &selectedDb , &firstTable , &lastTable , &tmpTable);
					if (tmp == 0) {
						strcpy(mainMsg, "order process fail \0");

					}else if(tmp == 1){
						strcpy(mainMsg, "create db success! \0");
					}
					else if (tmp == 2) {
						strcpy(mainMsg, "drop db success! \0");
					}
					else if (tmp == 3) {
						strcpy(mainMsg, "use db success! \0");
						monitor = 2;

					}
					getKey = '\0';

				}
				// 2�� �Է��ϸ� �α׾ƿ� 
				else if (getKey == 50) {
					selectedUser = NULL;
					monitor = 0;
					strcpy(mainMsg, "logout success!\0");
					getKey = '\0';

				}
				else {
					getKey = '\0';

				}



				break;
			case 2:				
				if (getKey == '\0') {
					//	printf("%s \n", selectedUser->id);
					showDb(mainMsg, selectedUser->id , selectedDb->name);
					getKey = getch();
				}

				if (getKey == 48) {
					exit;
				}
				else if (getKey == 49) {

					tmp = dbOrder(&selectedUser, &selectedDb, &firstTable, &lastTable, &tmpTable , &lastField , &tmpField);
					if (tmp == 0) {
						strcpy(mainMsg, "order process fail \0");

					}
					else if (tmp == 1) {
						strcpy(mainMsg, "create table success! \0");
					}
					else if (tmp == 2) {
						strcpy(mainMsg, "drop table success! \0");
					}
					else if (tmp == 3) {
						strcpy(mainMsg, "insert tuple success! \0");
						

					}
					else if (tmp == 4) {
						strcpy(mainMsg, "update tuple success! \0");
					}
					else if (tmp == 5) {
						strcpy(mainMsg, "dekete tuple success! \0");

					}
					else if (tmp == 6) {
						//select 

					}
					getKey = '\0';

				}
				else if(getKey == 50) {
					selectedDb = NULL;
					monitor = 1;
					strcpy(mainMsg, "logout success!\0");
					getKey = '\0';
				}
				else {
					getKey = '\0';
				}


			break;

		}



		if (exit == 1) {
			break;
		}


	}



	return 0;

}





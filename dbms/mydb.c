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

// 사용자 전역변수 
struct user* tmpUser = NULL;  // 임시로 user를 다룰 때 사용하는 임시 user변수
struct user* firstUser = NULL;  // 첫번쨰 user를 가리키는 user포인터
struct user* lastUser = NULL;   // 현재 user를 가리키는 user포인터(가장 마지막에 생성된 user)
struct user* selectedUser = NULL;   // 현재 접속중인 user를 가리키는 user 포인터 

// 디비 전역변수 
struct db* tmpDb = NULL; // 임시로 db를 다룰 때 사용하는 임시 db포인터변수
struct db* firstDb = NULL;  // 현재 유저가 가지고 있는 첫번쨰 db  로그인중이 아니라면 null   user의 firstDb가 이것이 된다
struct db* lastDb = NULL; // 현재 유저가 가지고 있는 마지막 db 로그인이 아니면 null user의 firstDb의 before가 있으면 그것이 되고    없다면 fristDb도 없으면 null
struct db* selectedDb = NULL;   //현재 유저가 선택한 db

//테이블 전역변수 
struct table* tmpTable = NULL; // 임시로 테이블을 다룰 때 사용하는 테이블포인터변수  
struct table* firstTable = NULL; //  DB를 선택했을 때 DB의 첫번째 테이블 
struct table* lastTable = NULL;  // DB를 선택했을 때 DB의 마지막 테이블 
struct talbe* selectedTable = NULL;   //  현재 선택중인 테이블 


//튜플 전역변수 
struct tuple* tmpTuple = NULL;
struct tuple* firstTuple = NULL;
struct tuple* lastTuple = NULL;


//필드 전역변수 
struct field* tmpField = NULL;  // 임시로 필드를 다룰 때 사용하는 필드
struct field* firstField = NULL;  // 선택한 테이블의 첫번째 필드
struct field* lastField = NULL;  // 선택한 테이블의 마지막 필드 
struct field* selectedField = NULL; // 현재 선택중인 필드 (사용할지는 모르겠음)



//데이터 전역변수 
struct data* tmpData = NULL;  // 임시로 데이터를 다룰 때 사용하는 데이터
struct data* firstData = NULL; // 현재 튜플의 첫 데이터
struct data* lastData = NULL; // 현재 튜플의 마지막 데이터

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
		// monitor가 0이면 메인화면 1이면 로그인화면 2면 디비화면을 출력한다 
		switch (monitor) {

			//메인화면 
			case 0:
				//getKEy가 Null이면 입력을 메인화면에서 입력을 받는다 
				if (getKey == '\0') {

					showMain(mainMsg);
					getKey = getch();
				}


				// 0을 입력했으면 프로그램 종료 
				if (getKey == 48) {
					exit = 1;
				}
				//1을 입력했으면 로그인 화면으로 이동
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
				//2를 입력했으면 회원가입 화면으로 이동 
				else if (getKey == 50) {
					// 회원가입 성공하면 메인페이지로 이동하고 성공 출력 
					if (signUp(&firstUser , &lastUser,&tmpUser) == 1) {
						strcpy(mainMsg, "sign up success!\0");
						getKey = '\0';
					}
					// 회원가입 실패하면 메인페이지로 이동하고 실패 출력 
					else {
						strcpy(mainMsg, "sign up fail.\0");
						getKey = '\0';
					}
				//3을 입력하면 회원 탈퇴 화면으로 이동
				}else if (getKey == 51) {
					// 회원탈퇴 성공하면 메인페이지로 이동하고 성공 출력 
					if (signOut(&firstUser, &lastUser,&tmpUser) == 1) {
						strcpy(mainMsg, "sign out success!\0");
						getKey = '\0';


					}
					// 회원탈퇴 실패하면 메인페이지로 이동하고 성공 출력 
					else {
						strcpy(mainMsg, "sign out fail.\0");
						getKey = '\0';
					}

				}

				else {
					getKey = '\0';

				}


				break;
			//로그인 화면 
			case 1:
				//getKEy가 Null이면 입력을 메인화면에서 입력을 받는다 
				if (getKey == '\0') {
				//	printf("%s \n", selectedUser->id);
					showLogin(mainMsg , selectedUser->id);
					getKey = getch();
				}

				// 0을 입력했으면 프로그램 종료 
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
				// 2를 입력하면 로그아웃 
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





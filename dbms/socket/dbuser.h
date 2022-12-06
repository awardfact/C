#include "dbfunction.h"

/*
계정을 생성하는 함수
유저가 없는경우 유저를 생성하고 firstUser와 lastUser를 생성한 유저로 한다
*/
int addUser(char id[], char password[], struct user** firstUser , struct user** lastUser , struct user** tmpUser) {


	//struct user* (*tmpUser) = *(*tmpUser);
	//struct user* (*tmpUser) = NULL;
	int folderAdd = 0;
	// 유저가 없는경우 
	if ( (*firstUser) == NULL) {

		(*tmpUser) = (struct user*)malloc(sizeof(struct user));
		strcpy((*tmpUser)->id, id);
		strcpy((*tmpUser)->password, password);
		(*tmpUser)->before = NULL;
		(*tmpUser)->after = NULL;
		(*tmpUser)->firstDB = NULL;
		(*tmpUser)->currentDB = NULL;
		(*tmpUser)->nextDB = NULL;
		(*firstUser) = (*tmpUser);
		(*lastUser) = (*tmpUser);


		//유저 폴더 만들고 user파일에 아이디 추가 
		folderAdd = mkdir(id, 0777);
		if (folderAdd == 0) {
			FILE* userFile = fopen("user.txt", "a");
			fprintf(userFile, "%s %s\n", id, password);
			fclose(userFile);

		}
		else {
			return 0;
		}



	}
	//유저가 있는경우 아이디 중복 체크하고 없는경우에 회원가입을 실행한다 
	else {
		(*tmpUser) = (*firstUser);
		// 중복 아이디가 있는지 체크 
		while ((*tmpUser)->after != NULL) {
			if (strcmp((*tmpUser)->id, id) == 0) {
				return 0;
			}
			(*tmpUser) = (*tmpUser)->after;
		}




		// 회원을 만들고 생성 계정의 전 계정이 현 마지막 계정이고 마지막 계정의 after가 현 계정이 된다 
		(*tmpUser) = (struct user*)malloc(sizeof(struct user));
		strcpy((*tmpUser)->id, id);
		strcpy((*tmpUser)->password, password);
		(*tmpUser)->before = (*lastUser);
		(*tmpUser)->after = NULL;
		(*tmpUser)->firstDB = NULL;
		(*tmpUser)->currentDB = NULL;
		(*tmpUser)->nextDB = NULL;
		(*lastUser)->after = (*tmpUser);
		(*lastUser) = (*tmpUser);

		//유저 폴더 만들고 user파일에 아이디 추가 
		folderAdd = mkdir(id, 0755);
		if (folderAdd == 0) {
			FILE* userFile = fopen("user.txt", "a");
			fprintf(userFile, "%s %s\n", id, password);
			fclose(userFile);

		}
		else {
			return 0;
		}

	}
	return 1;

}
// 파일에 저장되어있는 회원의 정보를 가져오는 함수 
void getUser(struct user** firstUser, struct user** lastUser , struct user** tmpUser , struct db** tmpDb , struct table** tmpTable, struct field** tmpField , struct field** tmpField2 , struct tuple** tmpTuple  , struct data** tmpData) {

	//struct user* (*tmpUser);
	//headerTest->id = "test2";


	char id[100];
	char passwd[100];
	FILE* userFile = fopen("user.txt", "r");
	//파일에서 유저 정보를 하나씩 가져옴
	while (fscanf(userFile, "%s %s", id, passwd) != -1) {
		// 유저 정보를 전역변수에 넣음 
		(*tmpUser) = (struct user*)malloc(sizeof(struct user));
		strcpy((*tmpUser)->id, id);
		strcpy((*tmpUser)->password, passwd);
		(*tmpUser)->before = NULL;
		(*tmpUser)->after = NULL;
		(*tmpUser)->firstDB = NULL;
		(*tmpUser)->currentDB = NULL;
		(*tmpUser)->nextDB = NULL;
		if ((*firstUser) == NULL) {

			(*firstUser) = (*tmpUser);
			(*lastUser) = (*tmpUser);

		}
		else {

			(*lastUser)->after = (*tmpUser);
			(*tmpUser)->before = (*lastUser);
			(*lastUser) = (*tmpUser);
		}
		getDb(tmpUser , tmpDb , tmpTable , tmpField , tmpField2 , tmpTuple , tmpData);
		//printf("%s %s\n", (*tmpUser)->id, (*tmpUser)->password);
		// 유저의 디비정보 가져오는 함수 호출 뒤에 추가 예정 
	}

	fclose(userFile);
	(*tmpUser) = NULL;



}

// 아이디와 패스워드를 받고 맞는 계정이 있으면 계정을 삭제하는 함수 
int deleteUser(char id[], char password[] , struct user** firstUser, struct user** lastUser , struct user** tmpUser) {
	//(*tmpUser) = firstUser;

	//struct user* (*tmpUser) = NULL;

	(*tmpUser) = NULL;
	char path[200];


	do {
		if ((*tmpUser) == NULL) {
			(*tmpUser) = (*firstUser);
		}
		else if ((*tmpUser)->after != NULL) {
			(*tmpUser) = (*tmpUser)->after;
		}
		// 입력한 아이디와 패스워드가 일치하는게 있는 경우 
		if ((*tmpUser) != NULL && strcmp((*tmpUser)->id, id) == 0 && strcmp((*tmpUser)->password, password) == 0) {



			//삭제 유저가 전역변수와 관련 있는 경우 전역 변수 수정
			if ((*tmpUser) == (*firstUser)) {
				if ((*firstUser)->after != NULL) {
					(*firstUser) = (*firstUser)->after;
				}
				else {
					(*firstUser) = NULL;
					(*lastUser) = NULL;
				}

			}
			else if ((*tmpUser) == (*lastUser)  ) {
				if ( (*lastUser)->before != NULL) {
					(*lastUser) = (*lastUser)->before;
				}
				else {
					(*firstUser) = NULL;
					(*lastUser) = NULL;
				}
			}

			//삭제 유저의 링크들을 수정해줌 
			if ((*tmpUser)->after != NULL) {
				if ((*tmpUser)->before != NULL) {
					(*tmpUser)->after->before = (*tmpUser)->before;
					(*tmpUser)->before->after = (*tmpUser)->after;
				}
				else {
					(*tmpUser)->after->before = NULL;
				}
			}
			else {
				if ((*tmpUser)->before != NULL) {
					(*tmpUser)->before->after = NULL;
				}
			}

			//유저의 DB들 삭제 함수 호출 추후 추가 예정 
			//해당 유저 디렉터리 안에 있는거 삭제 

			//strcpy(path, "./");
			//strcat(path, (*tmpUser)->id);
			rmdirs((*tmpUser)->id, 1);

			//해당 유저 메모리 할당 해제 
			free((*tmpUser));

			// userFile 현재 상태로 새로고침 
			(*tmpUser) = (*firstUser);


			//파일 현재상태로 다시씀 
			FILE* userFile = fopen("user.txt", "w");

			if ((*tmpUser) != NULL) {
				fprintf(userFile, "%s %s\n", (*tmpUser)->id, (*tmpUser)->password);
			}

			while ( (*tmpUser) != NULL && (*tmpUser)->after != NULL) {
				(*tmpUser) = (*tmpUser)->after;
				fprintf(userFile, "%s %s\n", (*tmpUser)->id, (*tmpUser)->password);
			}

			fclose(userFile);

			return 1;

		}



	} while ((*tmpUser)->after != NULL);


	return 0;
}


/*
로그인하는 함수
먼저 아이디와 패스워드를 입력받는다
유저 중에 아이디와 패스워드가 일치하는 유저가 있으면 selectUser를 해당 유저로 바꾼다
*/
int login(char id[] , char passwd[] , struct user** firstUser , struct user** selectedUser , struct user** tmpUser, struct sockS* sockTmp) {
	int result = 0;
	int wresult = 0;

	char id2[1024];
	char passwd2[1024];


	

	//struct user* (*tmpUser) = NULL;
	//(*tmpUser) = (struct user*)malloc(sizeof(struct user));
	int i = 0;
	wresult = write(sockTmp->cSock, "============================================================\n", 1024);
	wresult = write(sockTmp->cSock, "=                                                          =\n", 1024);
	wresult = write(sockTmp->cSock, "=                         login                            =\n", 1024);
	wresult = write(sockTmp->cSock, "=                                                          =\n" , 1024);

	wresult = write(sockTmp->cSock, "= id :\n", 1024);
	result = read(sockTmp->cSock, id2, 1024);

	printf("id : %s", id2);

	wresult = write(sockTmp->cSock, "= passwd :\n", 1024);
	result = read(sockTmp->cSock, passwd2, 1024);


	printf("pass : %s", passwd2);



	//printf("============================================================\n");
	//printf("=                                                          =\n");
	//printf("=                         login                            =\n");
	//printf("=                                                          =\n");
	//printf("= id :");
	//scanf("%s", id);
	//printf("= passwd :");
	//scanf("%s", passwd);

	(*tmpUser) = NULL;



	// 유저를 돌아서 입력한 아이디와 패스워드가 같은 회원이 있으면 로그인 
	do {
		if ((*tmpUser) == NULL) {
			(*tmpUser) = (*firstUser);
		}
		else if ((*tmpUser)->after != NULL) {
			(*tmpUser) = (*tmpUser)->after;
		}
		// 입력한 아이디와 패스워드가 일치하는게 있는 경우 
		if (strcmp((*tmpUser)->id, id2) == 0 && strcmp((*tmpUser)->password, passwd2) == 0) {
			(*selectedUser) = (*tmpUser);
			return 1;
		}
	} while ((*tmpUser)->after != NULL);


	return 0;
}






// 회원가입 화면 함수 회원정보 입력받고 addUser함수 호출 
int signUp(struct user** firstUser, struct user** lastUser , struct user** tmpUser, struct sockS* sockTmp) {
	int wresult = 0;
	char id[1024];
	char passwd[1024];
	char passwdRe[1024];
	int i = 0;
	int result = 0;


	wresult = write(sockTmp->cSock, "============================================================\n", 1024);
	wresult = write(sockTmp->cSock, "=                                                          =\n", 1024);
	wresult = write(sockTmp->cSock, "=                         sign Up                          =\n" , 1024);
	wresult = write(sockTmp->cSock, "=                                                          =\n", 1024);
	wresult = write(sockTmp->cSock, "= id :\n", 1024);
	result = read(sockTmp->cSock, id, 1024);

	printf("pass : %s", id);


	wresult = write(sockTmp->cSock, "= passwd :\n", 1024);
	result = read(sockTmp->cSock, passwd, 1024);

	printf("pass : %s", passwd);
	wresult = write(sockTmp->cSock, "= passwdRe :\n", 1024);
	result = read(sockTmp->cSock, passwdRe, 1024);

	printf("pass : %s", passwdRe);

	//printf("============================================================\n");
	//printf("=                                                          =\n");
	//printf("=                         sign Up                          =\n");
	//printf("=                                                          =\n");
	//printf("= id :");
	//scanf("%s", id);
	//printf("= passwd :");
	//scanf("%s", passwd);
	//printf("= input again passwd : ");
	//scanf("%s", passwdRe);

	// 패스워드와 패스워드 재입력이 같은 경우에만 addUser를 실행 
	if (strcmp(passwd, passwdRe) == 0) {
		return addUser( id, passwd, firstUser, lastUser , tmpUser);
	}
	else {
		return 0;
	}




}

// 로그아웃 하는 함수 
int logOut(struct user** selectedUser) {

	(*selectedUser) = NULL;


}

// 회원탈퇴 함수 아이디와 비밀번호를 입력하고 
int signOut(struct user** firstUser, struct user** lastUser , struct user** tmpUser, struct sockS* sockTmp) {
	char id[1024];
	char passwd[1024];
	int i = 0;
	int wresult = 0;
	int result = 0;

	wresult = write(sockTmp->cSock, "============================================================\n", 1024);
	wresult = write(sockTmp->cSock, "=                                                          =\n", 1024);
	wresult = write(sockTmp->cSock, "=                         sign Out                         =\n", 1024);
	wresult = write(sockTmp->cSock, "=                                                          =\n", 1024);
	wresult = write(sockTmp->cSock, "= id :\n", 1024);
	result = read(sockTmp->cSock, id, 1024);
	wresult = write(sockTmp->cSock, "= passwd :\n", 1024);
	result = read(sockTmp->cSock, passwd, 1024);


	//입력받은 아이디와 패스워드로 deleteUser함수 호출 
	return deleteUser(id, passwd , firstUser , lastUser , tmpUser);





}

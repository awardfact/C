#include "dbfunction.h"

//디비 생성 함수 
int createDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb){



	// id/  경로를 폴더 경로 변수에 넣음 
	char folderPath[100];
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	char pathTmp[100];
	int folderAdd = 0;
	//두번째 문장엔 database가 나와야함

	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}


	if (!((*order) != NULL && (strcmp((*order)->msg, "database") == 0 || strcmp((*order)->msg, "DATABASE") == 0 || strcmp((*order)->msg, "db") == 0 || strcmp((*order)->msg, "DB") == 0))) {
		return 0;
	}
	//세번쨰 문장은 디비 이름이 나옴 
	

	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}



	//디비가 없는경우 
	if ( (*firstDb) == NULL) {
		(*tmpDb) = (struct db*)malloc(sizeof(struct db));
		strcpy(  (*tmpDb)->name, (*order)->msg );
		(*tmpDb)->before = NULL;
		(*tmpDb)->after = NULL;
		(*tmpDb)->firstTable = NULL;
		(*tmpDb)->currentTable = NULL;
		(*tmpDb)->nextTable = NULL;
		(*firstDb) = (*tmpDb);
		(*lastDb) = (*tmpDb);
		(*selectedUser)->firstDB = (*firstDb);
		(*selectedUser)->currentDB = (*firstDb);


		// 파일에 디비 이름 추가하고 디비 폴더 추가 
		strcpy(pathTmp, folderPath);
		strcat(pathTmp, (*order)->msg);
		folderAdd = mkdir(pathTmp, 0777);
		if (folderAdd == 0) {

			strcpy(pathTmp, folderPath);
			strcat(pathTmp, "db.txt");
			FILE* dbFile = fopen(pathTmp, "a");
			fprintf(dbFile, "%s\n", (*order)->msg);
			fclose(dbFile);
		}
		else {
			return 0;
		}

	}
	else {

		
		//디비 이름중에 중복된 이름이 있는지 체크 
		do {
			if ((*tmpDb) == NULL) {
				(*tmpDb) = (*firstDb);
			}
			else {
				(*tmpDb) = (*tmpDb)->after;
			}

			if ((*tmpDb) != NULL && strcmp((*tmpDb)->name, (*order)->msg) == 0) {
				return 0;
			}
		} while ((*tmpDb)->after != NULL);


		(*tmpDb) = (struct db*)malloc(sizeof(struct db));
		strcpy((*tmpDb)->name, (*order)->msg);
		(*tmpDb)->before = NULL;
		(*tmpDb)->after = NULL;
		(*tmpDb)->firstTable = NULL;
		(*tmpDb)->currentTable = NULL;
		(*tmpDb)->nextTable = NULL;
		(*lastDb)->after = (*tmpDb);
		(*tmpDb)->before = (*lastDb);
		(*lastDb) = (*tmpDb);

		(*selectedUser)->currentDB = (*tmpDb);
		// 파일에 디비 이름 추가하고 디비 폴더 추가 

		strcpy(pathTmp, folderPath);
		strcat(pathTmp, (*order)->msg);
		folderAdd = mkdir(pathTmp, 0777);

		if (folderAdd == 0) {
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, "db.txt");
			FILE* dbFile = fopen(pathTmp, "a");
			fprintf(dbFile, "%s\n", (*order)->msg);
			fclose(dbFile);
		}
		else {
			return 0;
		}




	}


	return 1;



}


//디비 삭제하는 함수 
int dropDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb) {

	// id/  경로를 폴더 경로 변수에 넣음 
	char folderPath[100];
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	char pathTmp[100];
	(*tmpDb) = NULL;


	//drop 다음 문장이 있는경우 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	//없으면 실패 
	else {
		return 0;
	}


	// database가 아니면 실패 
	if (!((*order) != NULL && (strcmp((*order)->msg, "database") == 0 || strcmp((*order)->msg, "DATABASE") == 0  || strcmp((*order)->msg, "db") == 0  || strcmp((*order)->msg, "DB") == 0)   )  ) {
		return 0;
	}


	//삭제할 디비 이름이 없으면 실패 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}

	//디비가 하나도 없으면 실패 
	if ((*selectedUser)->firstDB == NULL) {
		return 0;
	}

	do {
		if ((*tmpDb) == NULL) {
			(*tmpDb) = (*selectedUser)->firstDB;
		}
		else if ((*tmpDb)->after != NULL) {
			(*tmpDb) = (*tmpDb)->after;

		}

		//입력한 디비가 있는 경우 
		if ((*tmpDb) != NULL  && (*order) != NULL && strcmp((*tmpDb)->name, (*order)->msg) == 0) {


			//삭제 유저가 전역변수와 관련 있는 경우 전역 변수 수정
			if ((*tmpDb) == (*firstDb)) {
				if ((*firstDb)->after != NULL) {
					(*firstDb) = (*firstDb)->after;
					(*selectedUser)->firstDB = (*firstDb);

				}
				else {
					(*firstDb) = NULL;
					(*lastDb) = NULL;
					(*selectedUser)->firstDB = NULL;
					(*selectedUser)->currentDB = NULL;

				}

			}
			else if ( (*tmpDb) == (*lastDb) ) {
				if ((*lastDb)->before != NULL) {
					(*lastDb) = (*lastDb)->before;
					(*selectedUser)->currentDB = (*lastDb)->before;
				}
				else {
					(*firstDb) = NULL;
					(*lastDb) = NULL;
					(*selectedUser)->firstDB = NULL;
					(*selectedUser)->currentDB = NULL;
				}
			}

			//삭제 유저의 링크들을 수정해줌 
			if ((*tmpDb)->after != NULL) {
				if ((*tmpDb)->before != NULL) {
					(*tmpDb)->after->before = (*tmpDb)->before;
					(*tmpDb)->before->after = (*tmpDb)->after;
				}
				else {
					(*tmpDb)->after->before = NULL;
				}
			}
			else {
				if ((*tmpDb)->before != NULL) {
					(*tmpDb)->before->after = NULL;
				}
			}


			//삭제 디비 밑에 있는 파일들 전부 삭제 
			//해당 유저 디렉터리 안에 있는거 삭제 
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, (*order)->msg);
			rmdirs(pathTmp, 1);

			free( (*tmpDb) );

			// userFile 현재 상태로 새로고침 
			(*tmpDb) = (*firstDb);
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, "db.txt");
			//파일 현재상태로 다시씀 
			FILE* userFile = fopen(pathTmp, "w");

			if ((*tmpDb) != NULL) {
				fprintf(userFile, "%s\n", (*tmpDb)->name);
			}

			while ((*tmpDb) != NULL && (*tmpDb)->after != NULL) {
				(*tmpDb) = (*tmpDb)->after;
				fprintf(userFile, "%s\n", (*tmpDb)->name);
			}
			fclose(userFile);

			return 2;

		}


	} while ((*tmpDb)->after != NULL);
	printf("4\n");


	return 0;

}

//디비를 사용하는 함수 
int useDb(struct dividedMsg** order, struct db** firstDb , struct db** tmpDb , struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable) {

	//use 다음 문장이 있는경우 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	//없으면 실패 
	else {
		return 0;
	}


	// database가 아니면 실패 
	if (!((*order) != NULL && (strcmp((*order)->msg, "database") == 0 || strcmp((*order)->msg, "DATABASE") == 0 || strcmp((*order)->msg, "db") == 0 || strcmp((*order)->msg, "DB") == 0))) {
		return 0;
	}


	//삭제할 디비 이름이 없으면 실패 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}


	do {
		if ( (*tmpDb) == NULL) {
			(*tmpDb) = (*firstDb);
		}
		else if ((*tmpDb)->after != NULL) {
			(*tmpDb) = (*tmpDb)->after;

		}


		printf("1\n");

		//입력한 디비가 있는 경우 
		if ((*tmpDb) != NULL && (*order) != NULL && strcmp((*tmpDb)->name, (*order)->msg) == 0) {

			(*selectedDb) = (*tmpDb);
			(*firstTable) = (*selectedDb)->firstTable;
			(*lastTable) = (*selectedDb)->currentTable;

			return 3;

		}


	} while ((*tmpDb)->after != NULL);





	return 0;

}

int showDb2(struct dividedMsg** order, struct db** firstDb, struct db** tmpDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable) {
	int i = 0;
	//show 다음 문장이 있는경우 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	//없으면 실패 
	else {
		return 0;
	}


	// database가 아니면 실패 
	if (!((*order) != NULL && (strcmp((*order)->msg, "DATABASES") == 0 || strcmp((*order)->msg, "databases") == 0 ))) {
		return 0;
	}


	(*tmpDb) = (*firstDb);

	printf("\n\n");
	printf("============================================================\n");
	printf("=                        Database                          =\n");
	printf("============================================================\n");
	while ((*tmpDb) != NULL) {

		printf("=");
		for (i = (56 - strlen((*tmpDb)->name)) / 2; i >= 0; i--) printf(" ");
		printf("%s", (*tmpDb)->name);
		for (i = (56 - strlen((*tmpDb)->name)) / 2; i >= 0; i--) printf(" ");
		if (strlen((*tmpDb)->name) % 2 != 0) printf(" ");
		printf("=\n");

		(*tmpDb) = (*tmpDb)->after;

	}
	printf("============================================================\n");
	return 4;

}

// 회원의 디비를 가져오는 함수 
void getDb(struct user** tmpUser ,struct db** tmpDb , struct table** tmpTable, struct field** tmpField , struct field** tmpField2 , struct tuple** tmpTuple , struct data** tmpData) {


	char folderPath[100];
	char pathTmp[100];
	//struct db* tmpDb = NULL;
	char name[100];



	strcpy(folderPath, (*tmpUser)->id);
	strcat(folderPath, "/");


	strcpy(pathTmp, folderPath);
	strcat(pathTmp, "db.txt");
	FILE* userFile = fopen(pathTmp, "r");



	if (userFile == NULL) {
		return;
	}
	//파일에서 유저 정보를 하나씩 가져옴
	while (fscanf(userFile, "%s", name) != -1) {


		// 유저 정보를 전역변수에 넣음 
		(*tmpDb) = (struct db*)malloc(sizeof(struct db));
		strcpy((*tmpDb)->name, name);;

		(*tmpDb)->before = NULL;
		(*tmpDb)->after = NULL;
		(*tmpDb)->firstTable = NULL;
		(*tmpDb)->currentTable = NULL;
		(*tmpDb)->nextTable = NULL;


		if ((*tmpUser)->firstDB == NULL) {
			(*tmpUser)->firstDB = (*tmpDb);
			(*tmpUser)->currentDB = (*tmpDb);
		}
		else {

			(*tmpUser)->currentDB->after = (*tmpDb);
			(*tmpDb)->before = (*tmpUser)->currentDB;
			(*tmpUser)->currentDB = (*tmpDb);



		}


		getTable(tmpUser ,  tmpDb , tmpTable , tmpField , tmpField2 , tmpTuple , tmpData );

		// 유저의 디비정보 가져오는 함수 호출 뒤에 추가 예정 
	}

	fclose(userFile);
	//(*tmpUser) = NULL;



}
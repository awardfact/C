#include "dbfunction.h"

//��� ���� �Լ� 
int createDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb){



	// id/  ��θ� ���� ��� ������ ���� 
	char folderPath[100];
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	char pathTmp[100];
	int folderAdd = 0;
	//�ι�° ���忣 database�� ���;���

	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}


	if (!((*order) != NULL && (strcmp((*order)->msg, "database") == 0 || strcmp((*order)->msg, "DATABASE") == 0 || strcmp((*order)->msg, "db") == 0 || strcmp((*order)->msg, "DB") == 0))) {
		return 0;
	}
	//������ ������ ��� �̸��� ���� 
	

	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}



	//��� ���°�� 
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


		// ���Ͽ� ��� �̸� �߰��ϰ� ��� ���� �߰� 
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

		
		//��� �̸��߿� �ߺ��� �̸��� �ִ��� üũ 
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
		// ���Ͽ� ��� �̸� �߰��ϰ� ��� ���� �߰� 

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


//��� �����ϴ� �Լ� 
int dropDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb) {

	// id/  ��θ� ���� ��� ������ ���� 
	char folderPath[100];
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	char pathTmp[100];
	(*tmpDb) = NULL;


	//drop ���� ������ �ִ°�� 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	//������ ���� 
	else {
		return 0;
	}


	// database�� �ƴϸ� ���� 
	if (!((*order) != NULL && (strcmp((*order)->msg, "database") == 0 || strcmp((*order)->msg, "DATABASE") == 0  || strcmp((*order)->msg, "db") == 0  || strcmp((*order)->msg, "DB") == 0)   )  ) {
		return 0;
	}


	//������ ��� �̸��� ������ ���� 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}

	//��� �ϳ��� ������ ���� 
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

		//�Է��� ��� �ִ� ��� 
		if ((*tmpDb) != NULL  && (*order) != NULL && strcmp((*tmpDb)->name, (*order)->msg) == 0) {


			//���� ������ ���������� ���� �ִ� ��� ���� ���� ����
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

			//���� ������ ��ũ���� �������� 
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


			//���� ��� �ؿ� �ִ� ���ϵ� ���� ���� 
			//�ش� ���� ���͸� �ȿ� �ִ°� ���� 
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, (*order)->msg);
			rmdirs(pathTmp, 1);

			free( (*tmpDb) );

			// userFile ���� ���·� ���ΰ�ħ 
			(*tmpDb) = (*firstDb);
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, "db.txt");
			//���� ������·� �ٽþ� 
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

//��� ����ϴ� �Լ� 
int useDb(struct dividedMsg** order, struct db** firstDb , struct db** tmpDb , struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable) {

	//use ���� ������ �ִ°�� 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	//������ ���� 
	else {
		return 0;
	}


	// database�� �ƴϸ� ���� 
	if (!((*order) != NULL && (strcmp((*order)->msg, "database") == 0 || strcmp((*order)->msg, "DATABASE") == 0 || strcmp((*order)->msg, "db") == 0 || strcmp((*order)->msg, "DB") == 0))) {
		return 0;
	}


	//������ ��� �̸��� ������ ���� 
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

		//�Է��� ��� �ִ� ��� 
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
	//show ���� ������ �ִ°�� 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	//������ ���� 
	else {
		return 0;
	}


	// database�� �ƴϸ� ���� 
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

// ȸ���� ��� �������� �Լ� 
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
	//���Ͽ��� ���� ������ �ϳ��� ������
	while (fscanf(userFile, "%s", name) != -1) {


		// ���� ������ ���������� ���� 
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

		// ������ ������� �������� �Լ� ȣ�� �ڿ� �߰� ���� 
	}

	fclose(userFile);
	//(*tmpUser) = NULL;



}
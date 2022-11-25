#include "dbfunction.h"


/*
������ �����ϴ� �Լ�
������ ���°�� ������ �����ϰ� firstUser�� lastUser�� ������ ������ �Ѵ�
*/
int addUser(char id[], char password[], struct user** firstUser , struct user** lastUser , struct user** tmpUser) {


	//struct user* (*tmpUser) = *(*tmpUser);
	//struct user* (*tmpUser) = NULL;
	int folderAdd = 0;
	// ������ ���°�� 
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


		//���� ���� ����� user���Ͽ� ���̵� �߰� 
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
	//������ �ִ°�� ���̵� �ߺ� üũ�ϰ� ���°�쿡 ȸ�������� �����Ѵ� 
	else {
		(*tmpUser) = (*firstUser);
		// �ߺ� ���̵� �ִ��� üũ 
		while ((*tmpUser)->after != NULL) {
			if (strcmp((*tmpUser)->id, id) == 0) {
				return 0;
			}
			(*tmpUser) = (*tmpUser)->after;
		}




		// ȸ���� ����� ���� ������ �� ������ �� ������ �����̰� ������ ������ after�� �� ������ �ȴ� 
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

		//���� ���� ����� user���Ͽ� ���̵� �߰� 
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
// ���Ͽ� ����Ǿ��ִ� ȸ���� ������ �������� �Լ� 
void getUser(struct user** firstUser, struct user** lastUser , struct user** tmpUser) {

	//struct user* (*tmpUser);

	char id[100];
	char passwd[100];
	FILE* userFile = fopen("user.txt", "r");

	//���Ͽ��� ���� ������ �ϳ��� ������
	while (fscanf(userFile, "%s %s", id, passwd) != -1) {


		// ���� ������ ���������� ���� 
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


		getDb(tmpUser);
		printf("%s %s\n", (*tmpUser)->id, (*tmpUser)->password);
		// ������ ������� �������� �Լ� ȣ�� �ڿ� �߰� ���� 
	}

	fclose(userFile);
	(*tmpUser) = NULL;



}

// ���̵�� �н����带 �ް� �´� ������ ������ ������ �����ϴ� �Լ� 
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
		// �Է��� ���̵�� �н����尡 ��ġ�ϴ°� �ִ� ��� 
		if ((*tmpUser) != NULL && strcmp((*tmpUser)->id, id) == 0 && strcmp((*tmpUser)->password, password) == 0) {



			//���� ������ ���������� ���� �ִ� ��� ���� ���� ����
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

			//���� ������ ��ũ���� �������� 
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

			//������ DB�� ���� �Լ� ȣ�� ���� �߰� ���� 
			//�ش� ���� ���͸� �ȿ� �ִ°� ���� 

			//strcpy(path, "./");
			//strcat(path, (*tmpUser)->id);
			rmdirs((*tmpUser)->id, 1);

			//�ش� ���� �޸� �Ҵ� ���� 
			free((*tmpUser));

			// userFile ���� ���·� ���ΰ�ħ 
			(*tmpUser) = (*firstUser);


			//���� ������·� �ٽþ� 
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
�α����ϴ� �Լ�
���� ���̵�� �н����带 �Է¹޴´�
���� �߿� ���̵�� �н����尡 ��ġ�ϴ� ������ ������ selectUser�� �ش� ������ �ٲ۴�
*/
int login(char id[] , char passwd[] , struct user** firstUser , struct user** selectedUser , struct user** tmpUser) {


	//struct user* (*tmpUser) = NULL;
	//(*tmpUser) = (struct user*)malloc(sizeof(struct user));
	int i = 0;
	printf("============================================================\n");
	printf("=                                                          =\n");
	printf("=                         login                            =\n");
	printf("=                                                          =\n");
	printf("= id :");
	scanf("%s", id);
	printf("= passwd :");
	scanf("%s", passwd);

	(*tmpUser) = NULL;



	// ������ ���Ƽ� �Է��� ���̵�� �н����尡 ���� ȸ���� ������ �α��� 
	do {
		if ((*tmpUser) == NULL) {
			(*tmpUser) = (*firstUser);
		}
		else if ((*tmpUser)->after != NULL) {
			(*tmpUser) = (*tmpUser)->after;
		}
		// �Է��� ���̵�� �н����尡 ��ġ�ϴ°� �ִ� ��� 
		if (strcmp((*tmpUser)->id, id) == 0 && strcmp((*tmpUser)->password, passwd) == 0) {
			(*selectedUser) = (*tmpUser);
			return 1;
		}
	} while ((*tmpUser)->after != NULL);


	return 0;
}






// ȸ������ ȭ�� �Լ� ȸ������ �Է¹ް� addUser�Լ� ȣ�� 
int signUp(struct user** firstUser, struct user** lastUser , struct user** tmpUser) {

	char id[100];
	char passwd[100];
	char passwdRe[100];
	int i = 0;
	printf("============================================================\n");
	printf("=                                                          =\n");
	printf("=                         sign Up                          =\n");
	printf("=                                                          =\n");
	printf("= id :");
	scanf("%s", id);
	printf("= passwd :");
	scanf("%s", passwd);
	printf("= input again passwd : ");
	scanf("%s", passwdRe);

	// �н������ �н����� ���Է��� ���� ��쿡�� addUser�� ���� 
	if (strcmp(passwd, passwdRe) == 0) {
		return addUser( id, passwd, firstUser, lastUser , tmpUser);
	}
	else {
		return 0;
	}




}

// �α׾ƿ� �ϴ� �Լ� 
int logOut(struct user** selectedUser) {

	(*selectedUser) = NULL;


}

// ȸ��Ż�� �Լ� ���̵�� ��й�ȣ�� �Է��ϰ� 
int signOut(struct user** firstUser, struct user** lastUser , struct user** tmpUser) {
	char id[100];
	char passwd[100];
	int i = 0;

	printf("============================================================\n");
	printf("=                                                          =\n");
	printf("=                         sign Out                         =\n");
	printf("=                                                          =\n");
	printf("= id :");
	scanf("%s", id);
	printf("= passwd :");
	scanf("%s", passwd);



	//�Է¹��� ���̵�� �н������ deleteUser�Լ� ȣ�� 
	return deleteUser(id, passwd , firstUser , lastUser , tmpUser);





}

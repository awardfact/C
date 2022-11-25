#include "dbfunction.h"



//CREATE TABLE ���̺��̸� (�ʵ��̸�1 Ÿ���̸�, �ʵ��̸�2 Ÿ���̸�2 .....);
int createTable(struct dividedMsg** order , char afterOrder[] ,struct user** selectedUser ,struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable , struct field** lastField , struct field** tmpField) {


	// id/  ��θ� ���� ��� ������ ���� 
	char folderPath[100];
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*selectedDb)->name);
	strcat(folderPath, "/");
	char pathTmp[100];
	char tableName[100];
	int folderAdd = 0;
	int isOpen = 0;
	int isClose = 0;
	int isComma = 0;

	char tmpMsg[100];
	char tmpMsg2[100];

	struct field* tmpF= NULL;

	char fName[100];
	char fType[100];
	int i = 0;
	int j = 0;
	int k = 0;
	int isName = 1;

	//�ι�° ���忣 database�� ���;���

	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}


	if (!((*order) != NULL && (strcmp((*order)->msg, "table") == 0 || strcmp((*order)->msg, "TABLE") == 0 )  ) ){
		return 0;
	}
	//������ ������ ��� �̸��� ���� 


	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}

	if ((*selectedDb)->firstTable != NULL) {

		(*tmpTable) = (*selectedDb)->firstTable;

		//�ߺ��� ���̺� �̸��� �ִ��� üũ 
		while ((*tmpTable)->after != NULL) {
			if (strcmp((*order)->msg, (*tmpTable)->name) == 0) {
				return 0;
			}
			(*tmpTable) = (*tmpTable)->after;
		}


	}

	(*tmpTable) = NULL;


	// ���̺��� �Ҵ��ϰ� ���� ���� �� �ִ� �������� �־��� 
	(*tmpTable) = (struct table*)malloc(sizeof(struct table));
	strcpy((*tmpTable)->name, (*order)->msg);
	(*tmpTable)->field = NULL;
	(*tmpTable)->firstData = NULL;
	(*tmpTable)->currentData = NULL;
	(*tmpTable)->nextData = NULL;



	fName[0] = '\0';
	fType[0] = '\0';
	printf("after : %s \n", afterOrder);
	//������ ������ �������� name���� ���� ���Ŀ��� type���� 
	while (afterOrder[i] != '\0') {

		printf("%c %d \n", afterOrder[i], i);

		
		if (isName == 1) {

			// ���� ������ �� ���������� Ÿ���̸�
			if (afterOrder[i] == 32 && fName[0] != '\0') {
				isName = 0;
				i++;
				//j = 0;
				continue;
			}
			//ó�� ������ ������ ��쿡�� �� ������ ���� 
			else if (afterOrder[i] == 32 && fName[0] == '\0') {
				i++;
				continue;
			}
			//���ڰ� ������ �̸��� �߰� 
			else {
				fName[j] = afterOrder[i];
				j++;
			}
		}
		else {
			//�ݴ°�ȣ�� ������ ��ɾ� ���� 
			if (afterOrder[i] == 41) {
				isClose = 1;
				//�̸��� �ʵ��� �������� ������ ���� �������ش� 
				fName[j] = '\0';
				fType[k] = '\0';
				// Ÿ���� int float char�� �ƴϸ� �Ҵ��ߴ��͵� ���� �����ϰ� ��� ���� 
				if (!(strcmp(fType, "INT") == 0 || strcmp(fType, "int") == 0 || strcmp(fType, "FLOAT") == 0 || strcmp(fType, "float") == 0 || strcmp(fType, "CHAR") == 0 || strcmp(fType, "char") == 0)) {

					freeField(&(*tmpTable)->field);
					free(tmpTable);
					return 0;

				}


				//�ʵ尡 ���°�� �ʵ带 �Ҵ����ְ� �̸��� Ÿ���� �־��ش� 
				if ((*tmpTable)->field == NULL) {
					(*tmpTable)->field = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpTable)->field->name, fName);
					strcpy((*tmpTable)->field->dataType, fType);
					(*tmpTable)->field->before = NULL;
					(*tmpTable)->field->after = NULL;
					(*lastField) = (*tmpTable)->field;
					

				}
				// �ʵ尡 �ִ°�� �ʵ带 �Ҵ����ְ� ������ �־��ش� 
				else {

					(*tmpField) = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpField)->name, fName);
					strcpy((*tmpField)->dataType, fType);
					(*lastField)->after = (*tmpField);
					(*tmpField)->before = (*lastField);
					(*tmpField)->after = NULL;
					(*lastField) = (*tmpField);
				}


				//���̺��� ���°�� ���� ����� ���� ���̺�� ���� ���̺��� ���� ���̺�� ���ְ� 
				if ((*firstTable) == NULL) {

					(*tmpTable)->before = NULL;
					(*tmpTable)->after = NULL;
					(*selectedDb)->firstTable = (*tmpTable);
					(*selectedDb)->currentTable = (*tmpTable);
				}
				//���̺��� ������ ������ �־��ش� 
				else {
					(*lastTable)->after = (*tmpTable);
					(*tmpTable)->before = (*lastTable);
					(*selectedDb)->currentTable->after = (*tmpTable);
					(*tmpTable)->before = (*selectedDb)->currentTable;

				}


				// ���̺� �̸� ������ �������ش� 
				strcpy(pathTmp, folderPath);
				strcat(pathTmp, (*tmpTable)->name);
				folderAdd = mkdir(pathTmp, 0755);
				//������ �����ϸ� ���̺� ���Ͽ� ���̺� �̸��� �߰����ش� 
				if (folderAdd == 0) {

					strcpy(pathTmp, folderPath);
					strcat(pathTmp, "table.txt");
					FILE* userFile = fopen(pathTmp , "a");
					fprintf(userFile, "%s\n", (*tmpTable)->name);
					fclose(userFile);
					//���̺� �̸��� �߰��� ������ �ʵ� �̸� ������ �߰����ش� 
					strcpy(pathTmp, folderPath);
					strcat(pathTmp, (*tmpTable)->name);
					strcat(pathTmp, "/field.txt");

					userFile = fopen(pathTmp, "a");
					(*tmpField) = (*tmpTable)->field;
					//�ʵ� �̸��� Ÿ�� �Է� 
					while ((*tmpField) != NULL) {
						fprintf(userFile, "%s %s\n", (*tmpField)->name, (*tmpField)->dataType);
						(*tmpField) = (*tmpField)->after;
					}
				}
				//���̺� ������ �����ؼ� ���� 1
				return 1;

			}
			//�޸��� ������ �ʵ带 �Ҵ��ϰ� �� �ʵ��� �̸��� Ÿ���� �־��ش� 
			else if (afterOrder[i] == 44) {
				printf("comma\n");
				//�̸��� �ʵ��� �������� ������ ���� �������ش� 
				fName[j] = '\0';
  				fType[k] = '\0';

				printf("0\n");

				printf("%s \n", fType);
				// Ÿ���� int float char�� �ƴϸ� �Ҵ��ߴ��͵� ���� �����ϰ� ��� ���� 
				if (!(strcmp(fType, "INT") == 0 || strcmp(fType, "int") == 0 || strcmp(fType, "FLOAT") == 0 || strcmp(fType, "float") == 0 || strcmp(fType, "CHAR") == 0 || strcmp(fType, "char") == 0)) {
				
					printf("4\n");
					freeField(&(*tmpTable)->field);
					printf("5\n");
					free((*tmpTable));
					return 0;

				}
				printf("1\n");
				//�ʵ尡 ���°�� �ʵ带 �Ҵ����ְ� �̸��� Ÿ���� �־��ش� 
				if ((*tmpTable)->field == NULL) {
					(*tmpTable)->field = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpTable)->field->name, fName);
					strcpy((*tmpTable)->field->dataType, fType);
					(*tmpTable)->field->before = NULL;
					(*tmpTable)->field->after = NULL;
					(*lastField) = (*tmpTable)->field;
						
				}
				
				// �ʵ尡 �ִ°�� �ʵ带 �Ҵ����ְ� ������ �־��ش� 
				else {

					(*tmpField) = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpField)->name, fName);
					strcpy((*tmpField)->dataType, fType);
					(*lastField)->after = (*tmpField);
					(*tmpField)->before = (*lastField);
					(*lastField) = (*tmpField);
				}

				printf("2\n");
				isName = 1;
				k = 0;
				j = 0;

				fName[0] = '\0';
				fType[0] = '\0';
			}
			else {
				if (afterOrder[i] == 32) {
					i++;
					continue;
				}
				fType[k] = afterOrder[i];
				k++;
			}
		}

		i++;


	}

	printf("5\n");
	//�ݺ��� �������Դٴ°��� ��ȣ �ݴ°� ���°��̱� ������ ���� �׷��� �޸� �Ҵ��� �߱⶧���� �Ҵ� �����ϰ� ���� 
	freeField(&(*tmpTable)->field);
	free(tmpTable);
	return 0;
	i++;


	//���̺��̸� ���� 
	//strcpy(tableName, (*order)->msg);
	
	//tmpMsg[0] = '\0';
	//tmpMsg2[0] = '\0';
	//tableName[0] = '\0';
	//printf("1 :  %s \n ", (*order)->msg);
	//���̺��̸��ڿ� �ٷ� ��ȣ�� �������� üũ 
	//isOpen  = openCheck((*order)->msg , tmpMsg , tmpMsg2);




	/*
	//��ȣ�� �ֵ� ���� tmpMsg�� ���̺��̸� 
	strcpy(tableName, tmpMsg);
	//���̺��̸��� ������ ���������� �߸���
	if (tableName[0] == '\0') {
		return 0;
	}

	//��ȣ�� ���°�� ��ȣ�� ã�ƾ� �Ѵ� 
	if (isOpen != 1) {
		tmpMsg[0] = '\0';
		tmpMsg2[0] = '\0';
		//��ȣ�� ã������ ���� �������� ������ 
		if ((*order)->after != NULL) {
			(*order) = (*order)->after;
		}
		else {
			return 0;
		}

		//�ٽ� openCheck�Լ��� ��� �������̶�� ������ ��ȣ�� �ְ� �ڿ� ������ ������ tmpMsg2�� ������ �������� �ƴϸ� ��ȣ�� ���� 
		isOpen = openCheck((*order)->msg, tmpMsg, tmpMsg2);

		//���̺� �̸� �ڿ��� ��ȣ�� ������ ���������� �߸���
		if (isOpen == 0) {
			return 0;
		}
	}


	//��ȣ�� ã�Ҵµ� tmp2Msg�� �ִ°��� ���°�찡 ���� ���Ǹ� ���� �ִ°�쿡�� msg�� �ش� �޽����� ���°�쿡�� ���� ������ ����Ų��
	if (tmpMsg2[0] == '\0') {

		if ((*order)->after != NULL) {
			(*order) = (*order)->after;
		}
		else {
			return 0;
		}
		
	}
	else {
		strcpy((*order)->msg, tmpMsg2);
	}


	// ��ɹ��� ������ ó�� �ʵ��̸��� ����Ų�� ���� �ݺ��� ������ �ʵ� ������ ��´�  �̰������� �޸𸮸� �Ҵ���Ѿ� �ϴµ� ��ȣ�ݴ°� ������ �ٽ� free��Ŵ
	while ((*order)->after != NULL) {

		//�ʵ��̸��� �־��ش� 
		strcpy(fName, (*order)->msg);

		// �ʵ� �̸� �ڿ��� Ÿ���� �پ�� �ϱ� ������ �� ������ ������ ���� 
		if ((*order)->after != NULL) {
			(*order) = (*order)->after;
		}
		else {
			return 0;
		}
		//�ʵ��̸��ڿ� ,�� �������� �ְ� )�� �������� ���� 
		tmpMsg[0] = '\0';
		tmpMsg2[0] = '\0';
		isClose = 0;
		isComma = 0;
		closeCheck((*order)->msg, tmpMsg, tmpMsg2 , &isClose , &isComma);
		//�޸��� ���°�쿡�� Ÿ���� tmpMsg�� �ǰ� �ڿ� �޸��� �־���� 
		if (isComma == 0) {
			strcpy(fType, tmpMsg);
		}


	}


	
	printf("1 :  %s  ,   2 : %s \n ", tmpMsg, tmpMsg2);
	
	
	
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}


	//��ȣüũ 
	if (strcmp((*order)->msg, "(")) {

		isOpen = 1;
		if ((*order)->after != NULL) {
			(*order) = (*order)->after;
		}
		else {
			return 0;
		}

		printf("1\n");

	}
	else if ( (*order)->msg[0] ==  40) {
		isOpen = 1;
		substring((*order)->msg , 1, 0);
		printf("2\n");

	}

	*/

}
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
	char fLength[100];
	int length = 0;
	char fNull[100];
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int m = 0;
	int intTmp = 0;

	int isName = 1;
	int isType = 0;
	int isLength = 0;
	int isNull = 0;

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


			//printf("compare  %s  and   %s \n",  (*order)->msg , (*tmpTable)->name);
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

	//printf("%d \n", strlen ((*order)->msg) );
	(*tmpTable)->field = NULL;
	(*tmpTable)->firstData = NULL;
	(*tmpTable)->currentData = NULL;
	(*tmpTable)->nextData = NULL;



	fName[0] = '\0';
	fType[0] = '\0';
	fLength[0] = '\0';
	fNull[0] = '\0';
	length = 0;


	printf("0\n");
	//������ ������ �������� name���� ���� ���Ŀ��� type���� 
	while (afterOrder[i] != '\0') {

		

		
		if (isName == 1) {

			// ���� ������ �� ���������� Ÿ���̸�
			if (afterOrder[i] == 32 && fName[0] != '\0') {
				isName = 0;
				isType = 1;
				i++;
				fName[j] = '\0';
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
		//Ÿ���� �Է��� ����
		else if (isType == 1) {
			//���� ��ȣ�� ������ Ÿ���� ������ ���̸� �Է��� ����
			if (afterOrder[i] == 40) {
				fType[k] = '\0';
				// Ÿ���� int float char�� �ƴϸ� �Ҵ��ߴ��͵� ���� �����ϰ� ��� ���� 
				if (!(strcmp(fType, "INT") == 0 || strcmp(fType, "int") == 0 || strcmp(fType, "FLOAT") == 0 || strcmp(fType, "float") == 0 || strcmp(fType, "CHAR") == 0 || strcmp(fType, "char") == 0 || strcmp(fType, "varchar") == 0 || strcmp(fType, "VARCHAR") == 0)) {

					freeField(&(*tmpTable)->field);
					free(tmpTable);
					return 0;

				}
				isType = 0;
				isLength = 1;

			}
			else {
				if (afterOrder[i] == 32) {
					i++;
					continue;
				}

				fType[k] = afterOrder[i];
				k++;
			}
		//���̸� �Է��� ����
		}else if(isLength == 1){




			//�ݴ� ��ȣ�� ������ ���� ���� 
			if (afterOrder[i] == 41) {
				fLength[l] = '\0';
				length = atoi2(fLength);

				isLength = 0;
				isNull = 1;

			}
			else {


				if (afterOrder[i] == 32) {
					i++;
					continue;
				}
				//���ڰ� �ƴϸ� ���������� �߸��Ȱſ��� ����
				if (!(afterOrder[i] >= 48 && afterOrder[i] <= 57)) {
					return 0;
				}
				//������ ù�ڸ��� 0�� ������ continue
				if (fLength[0] == '\0' && afterOrder[i] == 48) {
					i++;
					continue;
				}

				fLength[l] = afterOrder[i];
				l++;
			}



		}else if(isNull == 1) {
			
			
			printf("1\n");
			//�ݴ°�ȣ�� ������ ��ɾ� ���� 
			if (afterOrder[i] == 41) {
				isClose = 1;
				//�̸��� �ʵ��� �������� ������ ���� �������ش� 

				//�ʵ尡 ���°�� �ʵ带 �Ҵ����ְ� �̸��� Ÿ���� �־��ش� 
				if ((*tmpTable)->field == NULL) {

					(*tmpField) = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpField)->name, fName);
					strcpy((*tmpField)->dataType, fType);
					(*tmpField)->length = length;


					//printf("%s   \n", fNull);
					if (strcmp(fNull, "NOT NULL") == 0 || strcmp(fNull, "not null") == 0) {
						(*tmpField)->notNull = 1;
					}
					else {
						(*tmpField)->notNull = 0;
					}



					(*tmpField)->before = NULL;
					(*tmpField)->after = NULL;
					(*lastField) = (*tmpField);

					(*tmpTable)->field = (*tmpField);

				}
				// �ʵ尡 �ִ°�� �ʵ带 �Ҵ����ְ� ������ �־��ش� 
				else {


					(*tmpField) = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpField)->name, fName);
					strcpy((*tmpField)->dataType, fType);
					(*tmpField)->length = length;
					//printf("%s   \n", fNull);
					if (strcmp(fNull, "NOT NULL") == 0 || strcmp(fNull, "not null") == 0) {
						(*tmpField)->notNull = 1;
					}
					else {
						(*tmpField)->notNull = 0;
					}

					(*lastField)->after = (*tmpField);
					(*tmpField)->before = (*lastField);
					(*lastField) = (*tmpField);




				}


				//���̺��� ���°�� ���� ����� ���� ���̺�� ���� ���̺��� ���� ���̺�� ���ְ� 
				if ((*firstTable) == NULL) {

					(*tmpTable)->before = NULL;
					(*tmpTable)->after = NULL;
					(*selectedDb)->firstTable = (*tmpTable);
					(*selectedDb)->currentTable = (*tmpTable);
					(*firstTable) = (*tmpTable);
					(*lastTable) = (*tmpTable);
				}
				//���̺��� ������ ������ �־��ش� 
				else {
					(*lastTable)->after = (*tmpTable);
					(*tmpTable)->before = (*lastTable);
					(*selectedDb)->currentTable->after = (*tmpTable);
					(*tmpTable)->before = (*selectedDb)->currentTable;
					(*lastTable) = (*tmpTable);
					(*selectedDb)->currentTable = (*tmpTable);


				}


				// ���̺� �̸� ������ �������ش� 1
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


					//printf("%s \n", pathTmp);

					userFile = fopen(pathTmp, "a");
					(*tmpField) = (*tmpTable)->field;



					//�ʵ� �̸��� Ÿ�� �Է� 
					while ((*tmpField) != NULL) {
						//printf("%s %s %d %d\n", (*tmpField)->name, (*tmpField)->dataType , (*tmpField)->length , (*tmpField)->notNull );
						intTmp = fprintf(userFile, "%s %s %d %d\n", (*tmpField)->name, (*tmpField)->dataType,  (*tmpField)->length, (*tmpField)->notNull);
						//printf("intTmp = %d \n ", intTmp);
						(*tmpField) = (*tmpField)->after;
					}

					fclose(userFile);

				}
				//���̺� ������ �����ؼ� ���� 1



				(*tmpTable) = (*selectedDb)->firstTable;
				while ((*tmpTable) != NULL) {

					//printf("table : %s\n ", (*tmpTable)->name);
					if ((*tmpTable)->after != NULL) {
						(*tmpTable) = (*tmpTable)->after;
					}
					else {
						break;
					}
				}

				return 1;

			}
			//��ȣ�� ������ �ʵ峻�� �Ϸ�
			else if (afterOrder[i] == 44) {

				//�̸��� �ʵ��� �������� ������ ���� �������ش� 
				fName[j] = '\0';
  				fType[k] = '\0';
				fNull[m] = '\0';



				//�ʵ尡 ���°�� �ʵ带 �Ҵ����ְ� �̸��� Ÿ���� �־��ش� 
				if ((*tmpTable)->field == NULL) {


					(*tmpField) = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpField)->name, fName);
					strcpy((*tmpField)->dataType, fType);
					(*tmpField)->length = length;
					//printf("%s   \n", fNull);
					if (strcmp(fNull, "NOT NULL") == 0 || strcmp(fNull, "not null") == 0) {
						(*tmpField)->notNull = 1;
					}
					else {
						(*tmpField)->notNull = 0;
					}


					
					(*tmpField)->before = NULL;
					(*tmpField)->after = NULL;
					(*lastField) = (*tmpField);

					(*tmpTable)->field = (*tmpField);

						
				}
				
				// �ʵ尡 �ִ°�� �ʵ带 �Ҵ����ְ� ������ �־��ش� 
				else {

					(*tmpField) = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpField)->name, fName);
					strcpy((*tmpField)->dataType, fType);
					(*tmpField)->length = length;
					//printf("%s   \n", fNull);
					if (strcmp(fNull, "NOT NULL") == 0 || strcmp(fNull, "not null") == 0) {
						(*tmpField)->notNull = 1;
					}
					else {
						(*tmpField)->notNull = 0;
					}

					(*lastField)->after = (*tmpField);
					(*tmpField)->before = (*lastField);
					(*lastField) = (*tmpField);

				}


				isName = 1;
				isType = 0;
				isNull = 0;
				isLength = 0;
				k = 0;
				j = 0;
				l = 0;
				m = 0;

				fName[0] = '\0';
				fType[0] = '\0';
				length = 0;
				fLength[0] = '\0';
				fNull[0] = '\0';
			}
			else {

				//ó���� ������ ���°�� ó�� ������ ���� 
				if (fNull[0] == '\0' && afterOrder[i] == 32) {
					i++;
					continue;
				}
				//NOT NULL�� �ϼ��ǰ� ������ ���ڵ��� ���� ,�� )�� ������ Ÿ�� ����  NOT NULL �� ������ NULL�� ���� ������ NOT NULL�̿ܿ��� üũ�� �ʿ䰡 ���� 
				if (strcmp(fNull, "NOT NULL") == 0 || strcmp(fNull, "not null") == 0) {
					i++;
					continue;
				}
				fNull[m] = afterOrder[i];
				fNull[m + 1] = '\0';
				m++;
			}
		}

		i++;


	}


	//�ݺ��� �������Դٴ°��� ��ȣ �ݴ°� ���°��̱� ������ ���� �׷��� �޸� �Ҵ��� �߱⶧���� �Ҵ� �����ϰ� ���� 
	freeField(&(*tmpTable)->field);
	free(tmpTable);
	return 0;
	i++;

}

//����� ���̺��� �������� �Լ� 
void getTable(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField , struct field** tmpField2 , struct tuple** tmpTuple , struct data** tmpData) {

	char folderPath[200];
	char pathTmp[200];
	char name[100];


	char fName[100];
	char fType[100];
	int fLength = 0;
	int fnotNull = 0;

	struct field* field2;
	//������� 
	strcpy(folderPath, (*tmpUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*tmpDb)->name);
	strcat(folderPath, "/");

	strcpy(pathTmp, folderPath);
	strcat(pathTmp, "table.txt");

	//���̺� ���� ����
	FILE* userFile = fopen(pathTmp, "r");
	FILE* fuserFile = NULL;
	//���̺� ������ ������ ���� 
	if (userFile == NULL) {
		return;
	}

	//���Ͽ��� ���̺� ������ �ϳ��� ������
	while (fscanf(userFile, "%s", name) != -1) {
		
		(*tmpTable) = (struct table*)malloc(sizeof(struct table));
		strcpy((*tmpTable)->name, name);

		// ���̺��� ���� �־��� 
		(*tmpTable)->before = NULL;
		(*tmpTable)->after = NULL;
		(*tmpTable)->firstData = NULL;
		(*tmpTable)->currentData = NULL;
		(*tmpTable)->nextData = NULL;
		if ((*tmpDb)->firstTable == NULL) {
			(*tmpDb)->firstTable = (*tmpTable);
			(*tmpDb)->currentTable = (*tmpTable);
		}
		else {

			(*tmpDb)->currentTable->after = (*tmpTable);
			(*tmpTable)->before = (*tmpDb)->currentTable;
			(*tmpDb)->currentTable = (*tmpTable);
		}



		//�ʵ� ��� ����
		strcpy(pathTmp, folderPath);
		strcat(pathTmp, name);
		strcat(pathTmp, "/");
		strcat(pathTmp, "field.txt");
		//�ʵ� ������ ���� 
		fuserFile = fopen(pathTmp, "r");

		while (fscanf(fuserFile, "%s %s %d %d", fName , fType , &fLength , &fnotNull) != -1) {

			(*tmpField) = (struct field*)malloc(sizeof(struct field));
			strcpy((*tmpField)->name, fName);
			strcpy((*tmpField)->dataType, fType);
			(*tmpField)->length = fLength;
			(*tmpField)->notNull = fnotNull;
			(*tmpField)->before = NULL;
			(*tmpField)->after = NULL;
			if ((*tmpTable)->field == NULL) {
				(*tmpTable)->field = (*tmpField);
				field2 = (*tmpField);
			}
			else {
				field2->after = (*tmpField);
				(*tmpField)->before = field2;
				field2 = (*tmpField);
			}



		}
		fclose(fuserFile);

		getTuple(tmpUser, tmpDb, tmpTable, tmpField , tmpField2, tmpTuple , tmpData);
	}
	fclose(userFile);
}



//drop table ���̤����̸�
int dropTable(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable , struct field** tmpField) {

	char folderPath[100];
	char pathTmp[100];
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*selectedDb)->name);
	strcat(folderPath, "/");

	(*tmpTable) = NULL;
	//drop ���� �޽����� ������ ����  
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}
	

	//���� ������ ���̺��� �ƴϸ� ���� 
	if (!((*order) != NULL && (strcmp((*order)->msg, "table") == 0 || strcmp((*order)->msg, "TABLE") == 0))) {
		return 0;
	}

	// ���� order�� msg�� ������ ���̺� �̸��� �ȴ� 
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}

	if ((*selectedDb)->firstTable == NULL) {

		return 0;
	}


	do {

		if ((*tmpTable) == NULL) {
			(*tmpTable) = (*selectedDb)->firstTable;

		}
		else if ((*tmpTable)->after != NULL) {

			(*tmpTable) = (*tmpTable)->after;
		}


		//�Է��� ���̺��� �ִ°�� 
		if ((*tmpTable) != NULL && (*order) != NULL && strcmp((*tmpTable)->name, (*order)->msg) == 0  ) {

			//���� ������ ���������� ���� �ִ� ��� ���� ���� ����
			if ((*tmpTable) == (*firstTable)) {
				if ((*firstTable)->after != NULL) {
					(*firstTable) = (*firstTable)->after;
					(*selectedDb)->firstTable = (*firstTable);

				}
				else {
					(*firstTable) = NULL;
					(*lastTable) = NULL;
					(*selectedDb)->firstTable = NULL;
					(*selectedDb)->currentTable = NULL;

				}

			}

			else if ((*tmpTable) == (*lastTable)) {
				if ((*lastTable)->before != NULL) {
					(*lastTable) = (*lastTable)->before;
					(*selectedDb)->currentTable = (*lastTable)->before;
				}
				else {
					(*firstTable) = NULL;
					(*lastTable) = NULL;
					(*selectedDb)->firstTable = NULL;
					(*selectedDb)->currentTable = NULL;
				}
			}


			//���� ������ ��ũ���� �������� 
			if ((*tmpTable)->after != NULL) {
				if ((*tmpTable)->before != NULL) {
					(*tmpTable)->after->before = (*tmpTable)->before;
					(*tmpTable)->before->after = (*tmpTable)->after;
				}
				else {
					(*tmpTable)->after->before = NULL;
				}
			}
			else {
				if ((*tmpTable)->before != NULL) {
					(*tmpTable)->before->after = NULL;
				}
			}

			//���� ��� �ؿ� �ִ� ���ϵ� ���� ���� 
			freeField( &(*tmpTable)->field );
			//�ش� ���� ���͸� �ȿ� �ִ°� ���� 
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, (*order)->msg);
			rmdirs(pathTmp, 1);

			free((*tmpTable));
			// table ���� ���·� ���ΰ�ħ 
			(*tmpTable) = (*firstTable);
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, "table.txt");
			//���� ������·� �ٽþ� 
			FILE* userFile = fopen(pathTmp, "w");

			if ((*tmpTable) != NULL) {
				fprintf(userFile, "%s\n", (*tmpTable)->name);
			}

			while ((*tmpTable) != NULL && (*tmpTable)->after != NULL) {
				(*tmpTable) = (*tmpTable)->after;
				fprintf(userFile, "%s\n", (*tmpTable)->name);
			}
			fclose(userFile);


			(*tmpTable) = (*selectedDb)->firstTable;
			while ((*tmpTable) != NULL) {

				//printf("table : %s\n ", (*tmpTable)->name);
				if ((*tmpTable)->after != NULL) {
					(*tmpTable) = (*tmpTable)->after;
				}
				else {
					break;
				}
			}

			(*tmpTable) = NULL;


			return 2;




		}




	} while ((*tmpTable)->after != NULL);


	return 0;
}
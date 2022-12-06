#include "dbfunction.h"


// INSERT��� 
int insertTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField , struct field** tmpField2, struct tuple** tmpTuple , struct data** tmpData ) {



	char folderPath[1024];
	char pathTmp[1024];
	//�⺻��� ����
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*selectedDb)->name);
	strcat(folderPath, "/");
	int insertType = 0;
	char msgTmp[1024];
	char msgTmp2[1024];
	int isSpace = 0;
	int i = 0;
	int j = 0;
	char tableName[1024];
	struct dividedMsg* ftype = NULL;
	struct dividedMsg* ltype = NULL;
	struct dividedMsg* ttype = NULL;
	struct dividedMsg* fdata = NULL;
	struct dividedMsg* ldata = NULL;
	struct dividedMsg* tdata = NULL;

	struct dividedMsg* idata = NULL;
	int typeCount = 0;
	int dataCount = 0;
	int existTable = 0;



	printf("insert1\n");
	printf("%s \n", afterOrder);
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}
	if ( !((*order) != NULL && (strcmp((*order)->msg, "INTO") == 0 || strcmp((*order)->msg, "into") == 0 )   )    ) {
		return 0;
	}

	// insert into ���̺� �̸� �ڿ� VALUE�� �� ���� �ְ� (�� ���� �ʵ��̸��� �� ���� �ִ�  �׷��� VALUE���� Ÿ������ (���� Ÿ������ ������ �ؾ��Ѵ� 
	msgTmp[0] = '\0';
	while (afterOrder[i] != '\0') {


		//�տ� ������ ���°�쿡�� ������ ���� 
		if (msgTmp[0] == '\0' && afterOrder[i] == 32) {
			i++;
			continue;
		
		}
		else {

			// ���̺��̸��� �ְ� (�� ������ ��� Ÿ�� 1�� �̵�
			if (msgTmp[0] != '\0' && afterOrder[i] == 40) {
				insertType = 1;
				i++;
				break;
			}else if (msgTmp[0] != '\0' && afterOrder[i] == 32) {
				
				insertType = 2;
				i++;
				break;
			}
			else {
				msgTmp[j] = afterOrder[i];
				i++;
				j++;
			}
		}
	}
	msgTmp[j] = '\0';
	printf("insert2\n");
	strcpy(tableName, msgTmp);
	// ���̺��� �ִ� ���̺����� ������ �ؾ���
	(*tmpTable) = (*selectedDb)->firstTable;
	//���̺� ������ üũ�ؼ� ������ ������ exsitTable�� 1�� ������ش� �׸��� tmpTable�� insert�� ���̺��� �ȴ� 
	while ( (*tmpTable) != NULL) {

		if (strcmp(tableName, (*tmpTable)->name) == 0) {
			existTable = 1;
			break;
		}

		(*tmpTable) = (*tmpTable)->after;

	}

	(*tmpField) = (*tmpTable)->field;

 	//���� �̸��� ���̺��� ������ ���� 
	if (existTable == 0) {
		return 0;
	}
	msgTmp[0] = '\0';
	j = 0;
	//insert �ڿ� �ʵ��̸����� �����°��
	if (insertType == 1) {
		//�ʵ��̸��� ��ȣ�� ���������� 
		while (afterOrder[i] != '\0') {

			//��ȣ�ȿ� ������ ������ ��쿡�� �ǹ̰� ��� ���� 
			if (afterOrder[i] == 32) {
				i++;
				continue;
			//��ǥ�� ������ ��쿡�� ���� �ʵ��̸��� �Է¹��� 
			}else if (afterOrder[i] == 44) {
				msgTmp[j] = '\0';
				// �ʵ��̸��� ������ ���� 
				if (msgTmp[0] == '\0') {
					return 0;
				}

				//ùŸ���� NULL�ΰ�� 
				if (ftype == NULL) {
					ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					ttype->after = NULL;
					ttype->before = NULL;
					strcpy(ttype->msg, msgTmp);
					ftype = ttype;
					ltype = ttype;
				}
				//NULL�� �ƴѰ�� 
				else {
					ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					ttype->after = NULL;
					ttype->before = NULL;
					strcpy(ttype->msg, msgTmp);
					ltype->after = ttype;
					ttype->before = ltype;
					ltype = ttype;

				}
				typeCount++;


				//���� �ʵ带 �ޱ� ���� �ʱ�ȭ 
				j = 0;
				msgTmp[0] = '\0';
				i++;
			}
			//��ȣ�� ������ �ʵ��̸� �� 
			else if (afterOrder[i] == 41) {
				msgTmp[j] = '\0';
				// �ʵ��̸��� ������ ���� 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				
				//ùŸ���� NULL�ΰ�� 
				if (ftype == NULL) {
					ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					ttype->after = NULL;
					ttype->before = NULL;
					strcpy(ttype->msg, msgTmp);
					ftype = ttype;
					ltype = ttype;
				}
				//NULL�� �ƴѰ�� 
				else {
					ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					ttype->after = NULL;
					ttype->before = NULL;
					strcpy(ttype->msg, msgTmp);
					ltype->after = ttype;
					ttype->before = ltype;
					ltype = ttype;

				}
				typeCount++;

				j = 0;
				msgTmp[0] = '\0';
				i++;
				break;

			}
			//�Ϲ� ���ڸ� ���� �߰� 
			else {
				msgTmp[j] = afterOrder[i];
				j++;
				i++;
			}
		}

		//�ʵ��̸� �ڿ��� VALUES�� ���;� �� 
		while (afterOrder[i] != '\0') {

			//��ȣ��� VALUE������ ������ �ǹ̰� ���� 
			if (afterOrder[i] == 32) {
				i++;
				continue;

			}
			// ���°�ȣ�� ������ �����Ͱ� ���� 
			else if (afterOrder[i] == 40) {
				i++;
				break;
			}
			//�ٸ� ���ڸ� msgTmp�� ����
			else {
				msgTmp[j] = afterOrder[i];
				i++;
				j++;
			} 
		}

		//��ȣ�� ���̿� ������ ���� values�� �ƴϸ� ���� 
		if (  !(strcmp(msgTmp, "values") == 0 || strcmp(msgTmp, "VALUES")  == 0 )  ) {
			return 0;
		}

		msgTmp[0] = '\0';
		j = 0;

		//������ ���� �޴� �ݺ� 
		while (afterOrder[i] != '\0') {

			//��ȣ�ȿ� ������ ������ ��쿡�� �ǹ̰� ��� ���� 
			if (afterOrder[i] == 32 || afterOrder[i] == 34) {
				i++;
				continue;
				
			}
			//��ǥ�� ������ ��쿡�� ���������͸� �Է¹��� 
			else if (afterOrder[i] == 44) {

				// �����Ͱ� ������ ���� 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				msgTmp[j] = '\0';
				//ù�����Ͱ� NULL�ΰ�� 
				if (fdata == NULL) {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata = tdata;
					fdata = tdata;
				}
				//NULL�� �ƴѰ�� 
				else {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata->after = tdata;
					tdata->before = ldata;
					ldata = tdata;

				}
				dataCount++;


				//���� �����͸� �ޱ� ���� �ʱ�ȭ 
				j = 0;
				msgTmp[0] = '\0';
				i++;
			}
			//��ȣ�� ������ ������ �� 
			else if (afterOrder[i] == 41) {
				// �����Ͱ� ������ ���� 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				msgTmp[j] = '\0';
				//ù�����Ͱ� NULL�ΰ�� 
				if (fdata == NULL) {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata = tdata;
					fdata = tdata;
				}
				//NULL�� �ƴѰ�� 
				else {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata->after = tdata;
					tdata->before = ldata;
					ldata = tdata;

				}
				dataCount++;

				j = 0;
				msgTmp[0] = '\0';
				i++;
				break;

			}
			//�Ϲ� ���ڸ� ���� �߰� 
			else {
				msgTmp[j] = afterOrder[i];
				j++;
				i++;
			}
		}

		//�ʵ��� ������ �������� ������ �ٸ��� ����
		if (dataCount != typeCount) {
			return 0;
		}


		(*tmpField) = (*tmpTable)->field;

	}
	//insertŸ�� VLAUE�� �����°�� 
	else if (insertType == 2) {

		msgTmp[0] = '\0';
		j = 0;
		//�ʵ��̸� �ڿ��� VALUES�� ���;� �� 
		while (afterOrder[i] != '\0') {

			//��ȣ��� VALUE������ ������ �ǹ̰� ���� 
			if (afterOrder[i] == 32) {
				i++;
				continue;

			}
			// ���°�ȣ�� ������ �����Ͱ� ���� 
			else if (afterOrder[i] == 40) {
				i++;
				break;
			}
			//�ٸ� ���ڸ� msgTmp�� ����
			else {
				msgTmp[j] = afterOrder[i];
				i++;
				j++;
			}
		}

		//��ȣ�� ���̿� ������ ���� values�� �ƴϸ� ���� 
		if (!(strcmp(msgTmp, "values") == 0 || strcmp(msgTmp, "VALUES") == 0)) {
			return 0;
		}

		msgTmp[0] = '\0';
		j = 0;
		//������ ���� �޴� �ݺ� 
		while (afterOrder[i] != '\0') {

			//��ȣ�ȿ� ������ ������ ��쿡�� �ǹ̰� ��� ���� 
			if (afterOrder[i] == 32 || afterOrder[i] == 34) {
				i++;
				continue;

			}
			//��ǥ�� ������ ��쿡�� ���������͸� �Է¹��� 
			else if (afterOrder[i] == 44) {

				// �����Ͱ� ������ ���� 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				msgTmp[j] = '\0';
				//ù�����Ͱ� NULL�ΰ�� 
				if (fdata == NULL) {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata = tdata;
					fdata = tdata;
				}
				//NULL�� �ƴѰ�� 
				else {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata->after = tdata;
					tdata->before = ldata;
					ldata = tdata;

				}
				dataCount++;


				//���� �����͸� �ޱ� ���� �ʱ�ȭ 
				j = 0;
				msgTmp[0] = '\0';
				i++;
			}
			//��ȣ�� ������ ������ �� 
			else if (afterOrder[i] == 41) {
				// �����Ͱ� ������ ���� 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				msgTmp[j] = '\0';
				//ù�����Ͱ� NULL�ΰ�� 
				if (fdata == NULL) {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata = tdata;
					fdata = tdata;
				}
				//NULL�� �ƴѰ�� 
				else {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata->after = tdata;
					tdata->before = ldata;
					ldata = tdata;

				}
				dataCount++;

				j = 0;
				msgTmp[0] = '\0';
				i++;
				break;

			}
			//�Ϲ� ���ڸ� ���� �߰� 
			else {
				msgTmp[j] = afterOrder[i];
				j++;
				i++;
			}
		}

		(*tmpField) = (*tmpTable)->field;
		tdata = fdata;
		ttype = ftype;

		printf("inserttt3333\n");
		while (tdata != NULL) {

			//ùŸ���� NULL�ΰ�� 
			if (ftype == NULL) {
				ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
				ttype->after = NULL;
				ttype->before = NULL;
				strcpy(ttype->msg, (*tmpField)->name);
				ftype = ttype;
				ltype = ttype;
			}
			//NULL�� �ƴѰ�� 
			else {
				ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
				ttype->after = NULL;
				ttype->before = NULL;
				strcpy(ttype->msg, (*tmpField)->name);
				ltype->after = ttype;
				ttype->before = ltype;
				ltype = ttype;

			}

			tdata = tdata->after;
			if ((*tmpField)->after != NULL) {
				(*tmpField) = (*tmpField)->after;
			}


		}

	}
	else {
	(*tmpTuple) = NULL;
	(*tmpField) = NULL;
	(*tmpField2) = NULL;
	(*tmpData) = NULL;
		return 0;
	}
	printf("insert3\n");


	(*tmpField) = (*tmpTable)->field;

	while ((*tmpField) != NULL) {

		printf("fff %s   \n", (*tmpField)->name);
		(*tmpField) = (*tmpField)->after;


	}

	(*tmpTuple) = NULL;

	(*tmpField) = (*tmpTable)->field;
	ttype = NULL;
	tdata = NULL;

	while ((*tmpField) != NULL) {
		// Ÿ���� ���°��� ó���̳� �� �ʵ带 ������ �� 
		if (ttype == NULL) {
			ttype = ftype;
			tdata = fdata;
		}
		//�ƴϸ� ttype�� ��ĭ �ڷ� ���ش� 
		else if (ttype->after != NULL) {
			ttype = ttype->after;
			tdata = tdata->after;
		}
		printf("insert4  %s   \n" , (*tmpField)->name);
		//Ÿ���� �´°�� 
		if (strcmp((*tmpField)->name, ttype->msg) == 0) {


			//���̰� �����Ǿ��ִ°ź��� �� ��� 
			if (strlen(tdata->msg) > (*tmpField)->length) {
				freeMsg(&ftype);
				freeMsg(&fdata);


				if ((*tmpTuple) != NULL) {
					free((*tmpTuple));
				}
				(*tmpTuple) = NULL;
				(*tmpField) = NULL;
				(*tmpField2) = NULL;
				(*tmpData) = NULL;
				return 0;
			}

			printf("insert5\n");
			//�����Ͱ� ���°�� Ʃ���� �Ҵ����ְ� �ʵ� �����͸� ���� Ʃ���� ó��,������ �����ͷ� ���� 
			if ((*tmpTuple) == NULL) {
				(*tmpTuple) = (struct tuple*)malloc(sizeof(struct tuple));
				(*tmpField2) = (struct field*)malloc(sizeof(struct field));
				(*tmpData) = (struct data*)malloc(sizeof(struct data));
				(*tmpField2)->after = (*tmpField)->after;
				(*tmpField2)->before = (*tmpField)->before;
				strcpy((*tmpField2)->name, (*tmpField)->name);
				strcpy((*tmpField2)->dataType, (*tmpField)->dataType);
				(*tmpField2)->length = (*tmpField)->length;
				(*tmpField2)->notNull = (*tmpField)->notNull;
				printf("insert6\n");
				//������Ÿ�Կ� ���� �ٸ� ���� �Ҵ� 
				if (strcmp((*tmpField)->dataType, "CHAR") == 0 || strcmp((*tmpField)->dataType, "char") == 0 || strcmp((*tmpField)->dataType, "varchar") == 0 || strcmp((*tmpField)->dataType, "VARCHAR") == 0) {

					strcpy((*tmpData)->cdata, tdata->msg);
				}
				else if (strcmp((*tmpField)->dataType, "float") == 0 || strcmp((*tmpField)->dataType, "FLOAT") == 0) {

					if (tdata->msg[0] == 34 || tdata->msg[0] == 39) {
						freeMsg(&ftype);
						freeMsg(&fdata);
						if ((*tmpTuple) != NULL) {
							free((*tmpTuple));
						}
						(*tmpTuple) = NULL;
						(*tmpField) = NULL;
						(*tmpField2) = NULL;
						(*tmpData) = NULL;
						return 0;
					}
					(*tmpData)->fdata = atof2(tdata->msg);
				}
				else if (strcmp((*tmpField)->dataType, "int") == 0 || strcmp((*tmpField)->dataType, "INT") == 0) {

					if (tdata->msg[0] == 34 || tdata->msg[0] == 39) {
						freeMsg(&ftype);
						freeMsg(&fdata);
						if ((*tmpTuple) != NULL) {
							free((*tmpTuple));
						}
						(*tmpTuple) = NULL;
						(*tmpField) = NULL;
						(*tmpField2) = NULL;
						(*tmpData) = NULL;
						return 0;
					}
					(*tmpData)->idata = atoi2(tdata->msg);
				}
				(*tmpTuple)->firstData = (*tmpData);
				(*tmpTuple)->currentData = (*tmpData);

				(*tmpTuple)->firstField = (*tmpField2);
				(*tmpTuple)->currentField = (*tmpField2);
				printf("insert7\n");
			}
			//�����Ͱ� �ִ°�� �ʵ�� �����͸� �Ҵ��ϰ� Ʃ���� ���� �����͸� �Ҵ��� �����ͷ� �Ѵ� 
			else {
				(*tmpField2) = (struct field*)malloc(sizeof(struct field));
				(*tmpData) = (struct data*)malloc(sizeof(struct data));
				(*tmpField2)->after = (*tmpField)->after;
				(*tmpField2)->before = (*tmpField)->before;
				strcpy((*tmpField2)->name, (*tmpField)->name);
				strcpy((*tmpField2)->dataType, (*tmpField)->dataType);
				(*tmpField2)->length = (*tmpField)->length;
				(*tmpField2)->notNull = (*tmpField)->notNull;


				if (strcmp((*tmpField)->dataType, "CHAR") == 0 || strcmp((*tmpField)->dataType, "char") == 0 || strcmp((*tmpField)->dataType, "varchar") == 0 || strcmp((*tmpField)->dataType, "VARCHAR") == 0) {
					strcpy((*tmpData)->cdata, tdata->msg);
				}
				else if (strcmp((*tmpField)->dataType, "float") == 0 || strcmp((*tmpField)->dataType, "FLOAT") == 0) {
					if (tdata->msg[0] == 34 || tdata->msg[0] == 39) {
						freeMsg(&ftype);
						freeMsg(&fdata);
						if ((*tmpTuple) != NULL) {
							free((*tmpTuple));
						}
						(*tmpTuple) = NULL;
						(*tmpField) = NULL;
						(*tmpField2) = NULL;
						(*tmpData) = NULL;
						return 0;
					}

					(*tmpData)->fdata = atof2(tdata->msg);
				}
				else if (strcmp((*tmpField)->dataType, "int") == 0 || strcmp((*tmpField)->dataType, "INT") == 0) {

					if (tdata->msg[0] == 34 || tdata->msg[0] == 39) {
						freeMsg(&ftype);
						freeMsg(&fdata);
						if ((*tmpTuple) != NULL) {
							free((*tmpTuple));
						}
						(*tmpTuple) = NULL;
						(*tmpField) = NULL;
						(*tmpField2) = NULL;
						(*tmpData) = NULL;
						return 0;
					}
					(*tmpData)->idata = atoi2(tdata->msg);
				}
				(*tmpTuple)->currentData->after = (*tmpData);
				(*tmpData)->before = (*tmpTuple)->currentData->before;
				(*tmpTuple)->currentData = (*tmpData);

				(*tmpTuple)->currentField->after = (*tmpField2);
				(*tmpField2)->before = (*tmpTuple)->currentField->before;
				(*tmpTuple)->currentField = (*tmpField2);

				printf("insert8\n");
			}

			if (ttype->after != NULL) {
				if (ttype->before != NULL) {
					ttype->before->after = ftype->after;
					ttype->after->before = ftype->before;
				}
				else {
					ttype->after->before = NULL;
				}
			}
			else {
				if (ttype->before != NULL) {
					ttype->before->after = NULL;

				}
			}
			printf("insert9\n");
			if (tdata->after != NULL) {
				if (tdata->before != NULL) {
					tdata->before->after = tdata->after;
					tdata->after->before = tdata->before;
				}
				else {
					tdata->after->before = NULL;
				}
			}
			else {
				if (tdata->before != NULL) {
					tdata->before->after = NULL;

				}
			}
			ttype = NULL;
			tdata = NULL;


			(*tmpField) = (*tmpField)->after;
			printf("insert10\n");
		}
		//ttype�� after�� NULL�̸� �ش� �ʵ�� �Է��� �ʵ忡 ���ٴ� ���̱� ������ NOTNULLüũ 
		else if (ttype->after == NULL || ttype == NULL) {
			//notNULL�������ε� �ʵ��̸��� ������ ���� 
			if ((*tmpField)->notNull == 1) {
				if ((*tmpTuple) != NULL) {
					free((*tmpTuple));
				}
				freeMsg(&ftype);
				freeMsg(&fdata);
				(*tmpTuple) = NULL;
				(*tmpField) = NULL;
				(*tmpField2) = NULL;
				(*tmpData) = NULL;

				return 0;
			}


			//�����Ͱ� ���°�� Ʃ���� �Ҵ����ְ� �ʵ� �����͸� ���� Ʃ���� ó��,������ �����ͷ� ���� 
			if ((*tmpTuple) == NULL) {


				(*tmpTuple) = (struct tuple*)malloc(sizeof(struct tuple));
				(*tmpField2) = (struct field*)malloc(sizeof(struct field));
				(*tmpData) = (struct data*)malloc(sizeof(struct data));
				(*tmpField2)->after = (*tmpField)->after;
				(*tmpField2)->before = (*tmpField)->before;
				strcpy((*tmpField2)->name, (*tmpField)->name);
				strcpy((*tmpField2)->dataType, (*tmpField)->dataType);
				(*tmpField2)->length = (*tmpField)->length;
				(*tmpField2)->notNull = (*tmpField)->notNull;


				//������Ÿ�Կ� ���� �ٸ� ���� �Ҵ� 
				if (strcmp((*tmpField)->dataType, "CHAR") == 0 || strcmp((*tmpField)->dataType, "char") == 0 || strcmp((*tmpField)->dataType, "varchar") == 0 || strcmp((*tmpField)->dataType, "VARCHAR") == 0) {
					strcpy((*tmpData)->cdata, "NULL");
				}
				else if (strcmp((*tmpField)->dataType, "float") == 0 || strcmp((*tmpField)->dataType, "FLOAT") == 0) {
					(*tmpData)->fdata = atof2("0");
				}
				else if (strcmp((*tmpField)->dataType, "int") == 0 || strcmp((*tmpField)->dataType, "INT") == 0) {
					(*tmpData)->idata = atoi2("0");
				}

				(*tmpTuple)->firstData = (*tmpData);
				(*tmpTuple)->currentData = (*tmpData);

				(*tmpTuple)->firstField = (*tmpField2);
				(*tmpTuple)->currentField = (*tmpField2);

				printf("insert11\n");
			}
			//�����Ͱ� �ִ°�� �ʵ�� �����͸� �Ҵ��ϰ� Ʃ���� ���� �����͸� �Ҵ��� �����ͷ� �Ѵ� 
			else {

				(*tmpField2) = (struct field*)malloc(sizeof(struct field));
				(*tmpData) = (struct data*)malloc(sizeof(struct data));
				(*tmpField2)->after = (*tmpField)->after;
				(*tmpField2)->before = (*tmpField)->before;
				strcpy((*tmpField2)->name, (*tmpField)->name);
				strcpy((*tmpField2)->dataType, (*tmpField)->dataType);
				(*tmpField2)->length = (*tmpField)->length;
				(*tmpField2)->notNull = (*tmpField)->notNull;


				if (strcmp((*tmpField)->dataType, "CHAR") == 0 || strcmp((*tmpField)->dataType, "char") == 0 || strcmp((*tmpField)->dataType, "varchar") == 0 || strcmp((*tmpField)->dataType, "VARCHAR") == 0) {
					strcpy((*tmpData)->cdata, "NULL");
				}
				else if (strcmp((*tmpField)->dataType, "float") == 0 || strcmp((*tmpField)->dataType, "FLOAT") == 0) {
					(*tmpData)->fdata = atof2("0");
				}
				else if (strcmp((*tmpField)->dataType, "int") == 0 || strcmp((*tmpField)->dataType, "INT") == 0) {
					(*tmpData)->idata = atoi2("0");
				}
				(*tmpTuple)->currentData->after = (*tmpData);
				(*tmpData)->before = (*tmpTuple)->currentData->before;
				(*tmpTuple)->currentData = (*tmpData);
				(*tmpTuple)->currentField->after = (*tmpField2);
				(*tmpField2)->before = (*tmpTuple)->currentField->before;
				(*tmpTuple)->currentField = (*tmpField2);
			}
			ttype = NULL;
			tdata = NULL;

			(*tmpField) = (*tmpField)->after;

			printf("insert12\n");
		}
	}
	printf("insert4\n");
	strcat(folderPath, (*tmpTable)->name);
	strcat(folderPath, "/");
	//���̺� ���� Ʃ���� ��ũ���� 
	if ((*tmpTable)->firstData == NULL) {

		(*tmpTable)->firstData = (*tmpTuple);
		(*tmpTable)->currentData = (*tmpTuple);

	}
	else {

		(*tmpTable)->currentData->after = (*tmpTuple);
		(*tmpTuple)->before = (*tmpTable)->currentData;
		(*tmpTable)->currentData = (*tmpTuple);

	}

	// data���Ͽ� Ʃ���� ���� 
	strcpy(pathTmp, folderPath);
	strcat(pathTmp, "data.txt");
	FILE* dbFile = fopen(pathTmp, "a");
	(*tmpField) = (*tmpTuple)->firstField;
	(*tmpData) = (*tmpTuple)->firstData;
	while ((*tmpField) != NULL) {
		if (strcmp((*tmpField)->dataType, "CHAR") == 0 || strcmp((*tmpField)->dataType, "char") == 0 || strcmp((*tmpField)->dataType, "varchar") == 0 || strcmp((*tmpField)->dataType, "VARCHAR") == 0) {
			fprintf(dbFile, "%s ", (*tmpData)->cdata);

		}
		else if (strcmp((*tmpField)->dataType, "float") == 0 || strcmp((*tmpField)->dataType, "FLOAT") == 0) {
			fprintf(dbFile, "%f ", (*tmpData)->fdata);

		}
		else if (strcmp((*tmpField)->dataType, "int") == 0 || strcmp((*tmpField)->dataType, "INT") == 0) {
			fprintf(dbFile, "%d ", (*tmpData)->idata);

		}
		(*tmpField) = (*tmpField)->after;
		(*tmpData) = (*tmpData)->after;

	}
	fprintf(dbFile, "\n");
	fclose(dbFile);
	printf("insert5\n");
	(*tmpTuple) = NULL;
	(*tmpField) = NULL;
	(*tmpField2) = NULL;
	(*tmpData) = NULL;
	return 3;

}


//Ʃ���� �������� �Լ� 
void getTuple(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2  , struct tuple** tmpTuple , struct data** tmpData) {

	char folderPath[1024];
	strcpy(folderPath, (*tmpUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*tmpDb)->name);
	strcat(folderPath, "/");
	strcat(folderPath, (*tmpTable)->name);
	strcat(folderPath, "/");


	char tmpChar[1024];
	float tmpFloat;
	int tmpInt;
	char pathTmp[1024];

	strcpy(pathTmp, folderPath);
	strcat(pathTmp, "data.txt");
	int read = 0;
	FILE* userFile = fopen(pathTmp, "r");

	if (!userFile) {
		return;
	}

	//Ʃ���� �������� �ݺ���
	while (1) {

		(*tmpField) = (*tmpTable)->field;
		(*tmpTuple) = NULL;
		while ((*tmpField) != NULL) {
			//ó�� �ʵ�� Ʃ���� ��� Ʃ�� ���� 
			if ((*tmpTuple) == NULL) {
				(*tmpTuple) = (struct tuple*)malloc(sizeof(struct tuple));
				//(*tmpField2) = (struct field*)malloc(sizeof(struct field));
				(*tmpData) = (struct data*)malloc(sizeof(struct data));
				(*tmpField2) = (*tmpField);
				(*tmpTuple)->firstData = (*tmpData);
				(*tmpTuple)->currentData = (*tmpData);
				(*tmpTuple)->firstField = (*tmpField2);
				(*tmpTuple)->currentField = (*tmpField2);
			}
			//Ʃ���� ������ �������� ���� ��ġ�� ���� 
			else {
				//(*tmpField2) = (struct field*)malloc(sizeof(struct field));
				(*tmpData) = (struct data*)malloc(sizeof(struct data));

				(*tmpField2) = (*tmpField);
				(*tmpTuple)->currentData->after = (*tmpData);
				(*tmpData)->before = (*tmpTuple)->currentData->before;
				(*tmpTuple)->currentData = (*tmpData);

				(*tmpTuple)->currentField->after = (*tmpField2);
				(*tmpField2)->before = (*tmpTuple)->currentField->before;
				(*tmpTuple)->currentField = (*tmpField2);
			}



			//���Ͽ��� ������ �ϳ��� �޾ƿ�
			if (strcmp((*tmpTuple)->currentField->dataType, "CHAR") == 0 || strcmp((*tmpTuple)->currentField->dataType, "char") == 0 || strcmp((*tmpTuple)->currentField->dataType, "varchar") == 0 || strcmp((*tmpTuple)->currentField->dataType, "VARCHAR") == 0) {
				read = fscanf(userFile, "%s ", tmpChar);
				strcpy((*tmpData)->cdata, tmpChar);
			}
			else if (strcmp((*tmpTuple)->currentField->dataType, "float") == 0 || strcmp((*tmpTuple)->currentField->dataType, "FLOAT") == 0) {
				read = fscanf(userFile, "%f ", &tmpFloat);
				(*tmpData)->fdata = tmpFloat;
			}
			else if (strcmp((*tmpTuple)->currentField->dataType, "int") == 0 || strcmp((*tmpTuple)->currentField->dataType, "INT") == 0) {
				read = fscanf(userFile, "%d ", &tmpInt);
				(*tmpData)->idata = tmpInt;
			}
			//������ �޴°� �����ϸ� ���� �⺻���� ���� 
			if (read == -1) {
				free((*tmpTuple));
				free((*tmpData));
				break;
			}
			(*tmpTuple)->after = NULL;
			(*tmpTuple)->before = NULL;

			(*tmpField) = (*tmpField)->after;
		}

		//���� �� �о����� �ݺ����� ��������
		if (read == -1) {
			break;
		}
		fprintf(userFile, "\n");
		//���̺� �����Ͱ� ������ ó�� �����ͷ� �ƴϸ� ���� �����ͷ� �ִ´� 
		if ((*tmpTable)->firstData == NULL) {
			(*tmpTable)->firstData = (*tmpTuple);
			(*tmpTable)->currentData = (*tmpTuple);
		}
		else {
			(*tmpTable)->currentData->after = (*tmpTuple);
			(*tmpTuple)->before = (*tmpTable)->currentData;
			(*tmpTable)->currentData = (*tmpTuple);

		}



	}


	fclose(userFile);
	return;



}



// delete��� 
int deleteTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData) {
	char folderPath[1024];
	char pathTmp[1024];
	//�⺻��� ����
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*selectedDb)->name);
	strcat(folderPath, "/");
	char msgTmp[1024];
	int i = 0;
	int j = 0;
	int isDelete = 0;
	int isCondition = 0;
	char tableName[1024];
	struct whereCondition* where = NULL;
	struct whereCondition* whereTmp = NULL;
	struct whereCondition* whereEnd = NULL;
	(*tmpTable) = NULL;

	i = findMsg(afterOrder, "from", "FROM", 0);
	//from �� ������ ���� 
	if (i == -1) {
		return 0;
	}

	i = beforeSpace(afterOrder, msgTmp, i);
	strcpy(tableName, msgTmp);

	strcat(folderPath, tableName);
	strcat(folderPath, "/");
	strcat(folderPath, "data.txt");


	j = findTable(firstTable, lastTable, tmpTable, tableName);

	(*tmpTuple) = (*tmpTable)->firstData;
	//���̺��� ���°�� ���� 
	if (j == 0) {
		freeWhereCondition(&where);
		return 0;
	}

	i = findMsg(afterOrder, "where", "WHERE", i);
	//where �� ������ ������ ������ 
	if (i != -1) {

		i = findCondition(afterOrder, &where, &whereTmp, &whereEnd, i);
		if (i == -1) {
			freeWhereCondition(&where);
			return 0;
		}
		isCondition = 1;
	}


	while ((*tmpTuple) != NULL) {


		//������ üũ�Ѵ� ������ ���°��� where�� ��� 1���� 
		j = conditionCheck(&where, &whereTmp, tmpTuple);
		
		//���ǿ� �´°͸� ���� 
		if (j == 1) {
			//���� Ʃ���� ���̺� Ʃ�ð� ���� �ִ� ��� ���̺� Ʃ�� ����
			if ((*tmpTuple) == (*tmpTable)->firstData) {
				if ((*tmpTable)->firstData->after != NULL) {
					(*tmpTable)->firstData = (*tmpTable)->firstData->after;
				}
				else {
					(*tmpTable)->firstData = NULL;
					(*tmpTable)->currentData = NULL;
				}


			}
			else if ((*tmpTuple) == (*tmpTable)->currentData) {
				if ((*tmpTable)->currentData->before != NULL) {
					(*tmpTable)->currentData = (*tmpTable)->currentData->before;
				}
				else {
					(*tmpTable)->firstData = NULL;
					(*tmpTable)->currentData = NULL;
				}
			}

			//���� Ʃ���� ��ũ���� �������� 
			if ((*tmpTuple)->after != NULL) {
				if ((*tmpTuple)->before != NULL) {
					(*tmpTuple)->after->before = (*tmpTuple)->before;
					(*tmpTuple)->before->after = (*tmpTuple)->after;
				}
				else {
					(*tmpTuple)->after->before = NULL;
				}
			}
			else {
				if ((*tmpTuple)->before != NULL) {
					(*tmpTuple)->before->after = NULL;
				}
			}


			(*tmpTable)->nextData = (*tmpTuple)->after;
			freeData( &(*tmpTuple)->firstData);
			free((*tmpTuple));
			(*tmpTuple) = (*tmpTable)->nextData;
			isDelete = 4;
		}
		else {
			(*tmpTuple) = (*tmpTuple)->after;
		}
		



		
	}

	//��ũ�� ������ ������ ���� ���� 

	(*tmpTuple) = (*tmpTable)->firstData;
	j = writeTuple(tmpTuple, folderPath);

	return isDelete;

}


//update��� 
int updateTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData) {
	char folderPath[1024];
	char pathTmp[1024];
	//�⺻��� ����
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*selectedDb)->name);
	strcat(folderPath, "/");
	char msgTmp[1024];
	int i = 0;
	int j = 0;
	int isUpdate = 0;
	int isCondition = 0;
	char tableName[1024];
	struct whereCondition* where = NULL;
	struct whereCondition* whereTmp = NULL;
	struct whereCondition* whereEnd = NULL;

	char cTmp[1024];
	float fTmp;
	int iTmp;

	struct whereCondition* change = NULL;
	struct whereCondition* changeTmp = NULL;
	struct whereCondition* changeEnd = NULL;





	//���̺� �̸��� ������ 
	i = beforeSpace(afterOrder, msgTmp, i);
	strcpy(tableName, msgTmp);

	strcat(folderPath, tableName);
	strcat(folderPath, "/");
	strcat(folderPath, "data.txt");


	(*tmpTable) = NULL;

	


	i = findMsg(afterOrder, "set", "SET", i);
	//set �� ������ ���� 
	if (i == -1) {
		return 0;
	}


	//���̺��̸��� �ִ� ���̺����� üũ 
	j = findTable(firstTable, lastTable, tmpTable, tableName);
	(*tmpTuple) = (*tmpTable)->firstData;
	//���̺��� ���°�� ���� 
	if (j == 0) {
		return 0;
	}
	// � �ʵ尪�� ��� �������� �������� �Լ� 
	i = findUpdate(afterOrder, &change, &changeTmp, &changeEnd, i);


	//set�� ����� �ȵǾ������� ���� 
	if (i == -1) {
		freeWhereCondition(&change);
		freeWhereCondition(&where);
		return 0;
	}

	//�״����� where�� ã�� 
	i = findMsg(afterOrder, "where", "WHERE", i);

	if (i != -1) {

		i = findCondition(afterOrder, &where, &whereTmp, &whereEnd, i);
		if (i == -1) {
			freeWhereCondition(&change);
			freeWhereCondition(&where);
			return 0;
		}
		isCondition = 1;
	}
	printf("%d \n", i);


	changeTmp = change;
	while ((*tmpTuple) != NULL) {

		//������ üũ�Ѵ� ������ ���°��� where�� ��� 1���� 
		j = conditionCheck(&where, &whereTmp, tmpTuple);

		//���ǿ� �´� Ʃ�� ���� 
		if (j == 1) {
			changeTmp = change;
			(*tmpField) = (*tmpTuple)->firstField;
			(*tmpData) = (*tmpTuple)->firstData;
			while (changeTmp != NULL) {

				//�ʵ� �̸��� ������� 
				if (strcmp(changeTmp->field, (*tmpField)->name) == 0) {


					//������ ���� �ٲ���
					if (strcmp((*tmpField)->dataType, "CHAR") == 0 || strcmp((*tmpField)->dataType, "char") == 0 || strcmp((*tmpField)->dataType, "varchar") == 0 || strcmp((*tmpField)->dataType, "VARCHAR") == 0) {
						strcpy( (*tmpData)->cdata, changeTmp->value);
					}
					else if (strcmp((*tmpField)->dataType, "float") == 0 || strcmp((*tmpField)->dataType, "FLOAT") == 0) {
						fTmp = atof2(changeTmp->value);
						(*tmpData)->fdata = fTmp;
					}
					else if (strcmp((*tmpField)->dataType, "int") == 0 || strcmp((*tmpField)->dataType, "INT") == 0) {
						iTmp = atoi2(changeTmp->value);
						(*tmpData)->idata = iTmp;
					}



					//������ �� �ٲٰ� ������������ �Ѿ 
					(*tmpField) = (*tmpTuple)->firstField;
					(*tmpData) = (*tmpTuple)->firstData;
					changeTmp = changeTmp->after;

				}
				else {
					//üũ�ؾ��ϴ� ���� �ʵ尡 ������ ���� �ʵ�� �Ѿ�� 
					if ((*tmpField)->after != NULL) {
						(*tmpField) = (*tmpField)->after;
						(*tmpData) = (*tmpData)->after;

					//���ǿ� �´� �ʵ尡 ������ where���� �߸��Ȱſ��� ���� ����
					} else {
						freeWhereCondition(&change);
						freeWhereCondition(&where);
						return 0;
					}



				}




			}
			isUpdate = 5;

		}

		(*tmpTuple) = (*tmpTuple)->after;

	}



	//��ũ�� ������ ������ ���� ���� 

	(*tmpTuple) = (*tmpTable)->firstData;
	j = writeTuple(tmpTuple, folderPath);

	return isUpdate;





}



int selectTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData, struct sockS* sockTmp) {


	char msgTmp[1024];
	int i = 0;
	int j = 0;
	int isSelect = 0;
	int isUpdate = 0;
	int isCondition = 0;
	int star = 0;
	char tableName[1024];
	struct whereCondition* where = NULL;
	struct whereCondition* whereTmp = NULL;
	struct whereCondition* whereEnd = NULL;
	int count = 0;
	char cTmp[1024];
	float fTmp;
	int iTmp;

	struct whereCondition* select = NULL;
	struct whereCondition* selectTmp = NULL;
	struct whereCondition* selectEnd = NULL;

	int fieldNum = 0;
	int wresult = 0;
	int result = 0;

	j = findStar(afterOrder);

	if (j == -1) {

		return 0;
	}
	else if (j > 0) {
		star = 1;
		i = j;
	}
	else {
		i = findSelect(afterOrder, &select, &selectTmp, &selectEnd, i);
	}
	selectTmp = select;


	//strcpy(msgTmp, "!start!");
	msgTmp[strlen(msgTmp)] = '\n';
	wresult = write(sockTmp->cSock, msgTmp, 1024);

//	usleep(100000);

	//select�ʵ尪�� ����� �ȵǾ������� ���� 
	if (i == -1) {
		freeWhereCondition(&select);
		freeWhereCondition(&where);
		return 0;
	}

	//from�� ã��
	if (star == 0) {
		i = findMsg(afterOrder, "from", "FROM", i);
	}
	


	//from �� ������ ���� 
	if (i == -1) {
		freeWhereCondition(&where);
		freeWhereCondition(&select);
		return 0;
	}

	//���̺� �̸��� ã�� 
	i = beforeSpace(afterOrder, msgTmp, i);


	strcpy(tableName, msgTmp);
	//���̺��� �ִ� ���̺����� üũ 
	j = findTable(firstTable, lastTable, tmpTable, tableName);

	//������ ���� 
	if (j == 0) {
		freeWhereCondition(&where);
		freeWhereCondition(&select);
		return 0;
	}


	(*tmpTuple) = NULL;

	if ((*tmpTable)->currentData != NULL) {
		(*tmpTuple) = (*tmpTable)->currentData;


	}
	else {
		return 0;
	}
	


	//printf("tuple :  %s\n", (*tmpTable)->currentData->firstData->cdata);
	//�״����� where�� ã�� 
	i = findMsg(afterOrder, "where", "WHERE", i);
	if (i != -1) {
		//where�� ������ ������ ������ 
		i = findCondition(afterOrder, &where, &whereTmp, &whereEnd, i);
		if (i == -1) {
			freeWhereCondition(&select);
			freeWhereCondition(&where);
			return 0;
		}
		isCondition = 1;
	}





	//printf("4\n");

	(*tmpField) = (*tmpTuple)->firstField;
	selectTmp = select;
	if (star == 1) {

		while ((*tmpField) != NULL) {

			selectTmp = NULL;

			selectTmp = (struct whereCondition*)malloc(sizeof(struct whereCondition));
			selectTmp->after  = NULL;
			selectTmp->value[0] = '\0';
			//selectTmp->type = (*tmpField)->dataType;
			selectTmp->afterType = 0;
			strcpy(selectTmp->field , (*tmpField)->name ) ;
			if (select == NULL) {

				select = selectTmp;
				selectEnd = selectTmp;
			}
			else {
				selectEnd->after = selectTmp;
				selectEnd = selectTmp;
			}



			(*tmpField) = (*tmpField)->after;
		}
		


	}
	//printf("5\n");


	(*tmpField) = (*tmpTuple)->firstField;
	selectTmp = select;
	//printf("\n");
//	printf("\n");

	while (selectTmp != NULL) {


		selectTmp = selectTmp->after;
		fieldNum++;


	}


	//wresult = write(sockTmp->cSock, "\n\n", 1024);
	



	for (i = 0; i < fieldNum; i++) {
		//wresult = write(sockTmp->cSock, "================================", 1024);
		//printf("================================");
	}
	//wresult = write(sockTmp->cSock, "\n=", 1024);
	selectTmp = select;
	while (selectTmp != NULL) {



		//wresult = write(sockTmp->cSock, "selectTmp->field", 1024);
		//printf("     %20s     =", selectTmp->field);
		selectTmp = selectTmp->after;


	}
	//wresult = write(sockTmp->cSock, "\n", 1024);
	for (i = 0; i < fieldNum; i++) {
	//	wresult = write(sockTmp->cSock, "================================", 1024);
	}



	//while (selectTmp != NULL) {
	//	printf("%s ", selectTmp->field);
	//	//�ʵ��̸��� ���� ������ 
	//	if (strcmp(selectTmp->field, (*tmpField)->name) == 0) {


	//		printf("%10s ", (*tmpField)->name);
	//				selectTmp = selectTmp->after;
	//	}
	//	//�ƴѰ�� 
	//	else {


	//		printf("%10s ", (*tmpField)->name);

	//		//üũ�ؾ��ϴ� ���� �ʵ尡 ������ ���� �ʵ�� �Ѿ�� 
	//		if ((*tmpField)->after != NULL) {
	//			(*tmpField) = (*tmpField)->after;
	//			(*tmpData) = (*tmpData)->after;

	//			//���ǿ� �´� �ʵ尡 ������ where���� �߸��Ȱſ��� ���� ����
	//		}
	//		else {
	//			freeWhereCondition(&select);
	//			freeWhereCondition(&where);
	//			return 0;
	//		}

	//	}


	//}




	selectTmp = select;

	//�ٹٲ� 

	while ((*tmpTuple) != NULL) {


		//������ üũ�Ѵ� ������ ���°��� where�� ��� 1���� 
		j = conditionCheck(&where, &whereTmp, tmpTuple);

		//���ǿ� �´� Ʃ�� ���� 
		if (j == 1) {

			//wresult = write(sockTmp->cSock, "\n=", 1024);
			
			selectTmp = select;
			(*tmpField) = (*tmpTuple)->firstField;
			(*tmpData) = (*tmpTuple)->firstData;
			while (selectTmp != NULL) {

				//�ʵ� �̸��� ������� 
				if (strcmp(selectTmp->field, (*tmpField)->name) == 0) {

					//������ ���� ���
					if (strcmp((*tmpField)->dataType, "CHAR") == 0 || strcmp((*tmpField)->dataType, "char") == 0 || strcmp((*tmpField)->dataType, "varchar") == 0 || strcmp((*tmpField)->dataType, "VARCHAR") == 0) {
						//printf("     %20s     =", (*tmpData)->cdata);
					//	usleep(100000);
						memset(msgTmp, 0x00, 1024);
						strcpy(msgTmp, (*tmpData)->cdata);
						//msgTmp[strlen(msgTmp)] = '\n';
						printf("printf\n");
						//strcat(msgTmp , "\0");

						wresult = write(sockTmp->cSock, msgTmp, 1024 );

					}
					else if (strcmp((*tmpField)->dataType, "float") == 0 || strcmp((*tmpField)->dataType, "FLOAT") == 0) {
						sprintf(msgTmp, "%f", (*tmpData)->fdata);
						//strcat(msgTmp, "\n");
						wresult = write(sockTmp->cSock, msgTmp, 1024);
						//printf("      %20.5f     =", (*tmpData)->fdata);
					}
					else if (strcmp((*tmpField)->dataType, "int") == 0 || strcmp((*tmpField)->dataType, "INT") == 0) {
						sprintf(msgTmp, "%d", (*tmpData)->idata);
						//strcat(msgTmp, "\n");
						wresult = write(sockTmp->cSock, msgTmp, 1024);
						//wresult = write(sockTmp->cSock, (*tmpData)->idata, 1024);
						//printf("     %20i     =", (*tmpData)->idata);
					}
					selectTmp = selectTmp->after;
				}
				else {

					//üũ�ؾ��ϴ� ���� �ʵ尡 ������ ���� �ʵ�� �Ѿ�� 
					if ((*tmpField)->after != NULL) {
						(*tmpField) = (*tmpField)->after;
						(*tmpData) = (*tmpData)->after;

						//���ǿ� �´� �ʵ尡 ������ where���� �߸��Ȱſ��� ���� ����
					}
					else {
						freeWhereCondition(&select);
						freeWhereCondition(&where);
						return 0;
					}
				}


			}

			isSelect = 6;
		}
		(*tmpTuple) = (*tmpTuple)->before;
		count++;

		if (count >= 10) {
			break;
		}

	}
	//usleep(100000);
	//usleep(15000);
	memset(msgTmp, 0x00, 1024);
	strcpy(msgTmp, "!end!");
	//msgTmp[strlen(msgTmp)] = '\n';
	wresult = write(sockTmp->cSock, msgTmp, 1024);
	printf("printf\n");

	//wresult = write(sockTmp->cSock, msgTmp, 7);
	//wresult = write(sockTmp->cSock, msgTmp, 7);
	//wresult = write(sockTmp->cSock, "\n", 1024);
	//for (i = 0; i < fieldNum; i++) {
	//	wresult = write(sockTmp->cSock, "================================", 1024);
	//}
	//wresult = write(sockTmp->cSock, "\n", 1024);

	//sleep(1);
	//sleep(1);
	return isSelect;
}
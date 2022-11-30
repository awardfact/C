
//update��� 
int updateTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData) {
	char folderPath[200];
	char pathTmp[200];
	//�⺻��� ����
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*selectedDb)->name);
	strcat(folderPath, "/");
	char msgTmp[200];
	int i = 0;
	int j = 0;
	int isUpdate = 0;
	int isCondition = 0;
	char tableName[100];
	struct whereCondition* where = NULL;
	struct whereCondition* whereTmp = NULL;
	struct whereCondition* whereEnd = NULL;

	char cTmp[200];
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
						strcpy((*tmpData)->cdata, changeTmp->value);
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
					}
					else {
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


//update명령 
int updateTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData) {
	char folderPath[200];
	char pathTmp[200];
	//기본경로 설정
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





	//테이블 이름을 가져옴 
	i = beforeSpace(afterOrder, msgTmp, i);
	strcpy(tableName, msgTmp);

	strcat(folderPath, tableName);
	strcat(folderPath, "/");
	strcat(folderPath, "data.txt");


	(*tmpTable) = NULL;




	i = findMsg(afterOrder, "set", "SET", i);
	//set 이 없으면 실패 
	if (i == -1) {
		return 0;
	}


	//테이블이름이 있는 테이블인지 체크 
	j = findTable(firstTable, lastTable, tmpTable, tableName);
	(*tmpTuple) = (*tmpTable)->firstData;
	//테이블이 없는경우 실패 
	if (j == 0) {
		return 0;
	}
	// 어떤 필드값을 어떻게 수정할지 가져오는 함수 
	i = findUpdate(afterOrder, &change, &changeTmp, &changeEnd, i);


	//set이 제대로 안되어있으면 실패 
	if (i == -1) {
		freeWhereCondition(&change);
		freeWhereCondition(&where);
		return 0;
	}

	//그다음에 where을 찾음 
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

		//조건을 체크한다 조건이 없는경우는 where이 없어서 1리턴 
		j = conditionCheck(&where, &whereTmp, tmpTuple);

		//조건에 맞는 튜플 수정 
		if (j == 1) {
			changeTmp = change;
			(*tmpField) = (*tmpTuple)->firstField;
			(*tmpData) = (*tmpTuple)->firstData;
			while (changeTmp != NULL) {

				//필드 이름이 같은경우 
				if (strcmp(changeTmp->field, (*tmpField)->name) == 0) {


					//데이터 값을 바꿔줌
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



					//데이터 값 바꾸고 다음조건으로 넘어감 
					(*tmpField) = (*tmpTuple)->firstField;
					(*tmpData) = (*tmpTuple)->firstData;
					changeTmp = changeTmp->after;

				}
				else {
					//체크해야하는 다음 필드가 있으면 다음 필드로 넘어가고 
					if ((*tmpField)->after != NULL) {
						(*tmpField) = (*tmpField)->after;
						(*tmpData) = (*tmpData)->after;

						//조건에 맞는 필드가 없으면 where절이 잘못된거여서 문법 오류
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



	//링크를 삭제한 다음에 파일 수정 

	(*tmpTuple) = (*tmpTable)->firstData;
	j = writeTuple(tmpTuple, folderPath);

	return isUpdate;





}

#include "dbfunction.h"


// INSERT명령 
int insertTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField , struct field** tmpField2, struct tuple** tmpTuple , struct data** tmpData ) {



	char folderPath[1024];
	char pathTmp[1024];
	//기본경로 설정
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

	// insert into 테이블 이름 뒤에 VALUE가 올 수도 있고 (가 오고 필드이름이 올 수도 있다  그래서 VALUE오는 타입인지 (오는 타입인지 구분을 해야한다 
	msgTmp[0] = '\0';
	while (afterOrder[i] != '\0') {


		//앞에 공백이 오는경우에는 공백을 무시 
		if (msgTmp[0] == '\0' && afterOrder[i] == 32) {
			i++;
			continue;
		
		}
		else {

			// 테이블이름이 있고 (가 나오는 경우 타입 1로 이동
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
	// 테이블이 있는 테이블인지 검증을 해야함
	(*tmpTable) = (*selectedDb)->firstTable;
	//테이블 끝까지 체크해서 같은게 있으면 exsitTable을 1로 만들어준다 그리고 tmpTable은 insert할 테이블이 된다 
	while ( (*tmpTable) != NULL) {

		if (strcmp(tableName, (*tmpTable)->name) == 0) {
			existTable = 1;
			break;
		}

		(*tmpTable) = (*tmpTable)->after;

	}

	(*tmpField) = (*tmpTable)->field;

 	//같은 이름의 테이블이 없으면 실패 
	if (existTable == 0) {
		return 0;
	}
	msgTmp[0] = '\0';
	j = 0;
	//insert 뒤에 필드이름부터 나오는경우
	if (insertType == 1) {
		//필드이름은 괄호가 닫힐때까지 
		while (afterOrder[i] != '\0') {

			//괄호안에 공백이 나오는 경우에는 의미가 없어서 무시 
			if (afterOrder[i] == 32) {
				i++;
				continue;
			//쉼표가 나오는 경우에는 다음 필드이름을 입력받음 
			}else if (afterOrder[i] == 44) {
				msgTmp[j] = '\0';
				// 필드이름이 없으면 실패 
				if (msgTmp[0] == '\0') {
					return 0;
				}

				//첫타입이 NULL인경우 
				if (ftype == NULL) {
					ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					ttype->after = NULL;
					ttype->before = NULL;
					strcpy(ttype->msg, msgTmp);
					ftype = ttype;
					ltype = ttype;
				}
				//NULL이 아닌경우 
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


				//다음 필드를 받기 위해 초기화 
				j = 0;
				msgTmp[0] = '\0';
				i++;
			}
			//괄호가 닫히면 필드이름 끝 
			else if (afterOrder[i] == 41) {
				msgTmp[j] = '\0';
				// 필드이름이 없으면 실패 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				
				//첫타입이 NULL인경우 
				if (ftype == NULL) {
					ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					ttype->after = NULL;
					ttype->before = NULL;
					strcpy(ttype->msg, msgTmp);
					ftype = ttype;
					ltype = ttype;
				}
				//NULL이 아닌경우 
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
			//일반 문자면 내용 추가 
			else {
				msgTmp[j] = afterOrder[i];
				j++;
				i++;
			}
		}

		//필드이름 뒤에는 VALUES가 나와야 함 
		while (afterOrder[i] != '\0') {

			//괄호들과 VALUE사이의 공백은 의미가 없음 
			if (afterOrder[i] == 32) {
				i++;
				continue;

			}
			// 여는괄호가 나오면 데이터값 시작 
			else if (afterOrder[i] == 40) {
				i++;
				break;
			}
			//다른 문자면 msgTmp에 넣음
			else {
				msgTmp[j] = afterOrder[i];
				i++;
				j++;
			} 
		}

		//괄호들 사이에 나오는 값이 values가 아니면 실패 
		if (  !(strcmp(msgTmp, "values") == 0 || strcmp(msgTmp, "VALUES")  == 0 )  ) {
			return 0;
		}

		msgTmp[0] = '\0';
		j = 0;

		//데이터 값을 받는 반복 
		while (afterOrder[i] != '\0') {

			//괄호안에 공백이 나오는 경우에는 의미가 없어서 무시 
			if (afterOrder[i] == 32 || afterOrder[i] == 34) {
				i++;
				continue;
				
			}
			//쉼표가 나오는 경우에는 다음데이터를 입력받음 
			else if (afterOrder[i] == 44) {

				// 데이터가 없으면 실패 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				msgTmp[j] = '\0';
				//첫데이터가 NULL인경우 
				if (fdata == NULL) {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata = tdata;
					fdata = tdata;
				}
				//NULL이 아닌경우 
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


				//다음 데이터를 받기 위해 초기화 
				j = 0;
				msgTmp[0] = '\0';
				i++;
			}
			//괄호가 닫히면 데이터 끝 
			else if (afterOrder[i] == 41) {
				// 데이터가 없으면 실패 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				msgTmp[j] = '\0';
				//첫데이터가 NULL인경우 
				if (fdata == NULL) {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata = tdata;
					fdata = tdata;
				}
				//NULL이 아닌경우 
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
			//일반 문자면 내용 추가 
			else {
				msgTmp[j] = afterOrder[i];
				j++;
				i++;
			}
		}

		//필드의 개수와 데이터의 개수가 다르면 실패
		if (dataCount != typeCount) {
			return 0;
		}


		(*tmpField) = (*tmpTable)->field;

	}
	//insert타입 VLAUE가 나오는경우 
	else if (insertType == 2) {

		msgTmp[0] = '\0';
		j = 0;
		//필드이름 뒤에는 VALUES가 나와야 함 
		while (afterOrder[i] != '\0') {

			//괄호들과 VALUE사이의 공백은 의미가 없음 
			if (afterOrder[i] == 32) {
				i++;
				continue;

			}
			// 여는괄호가 나오면 데이터값 시작 
			else if (afterOrder[i] == 40) {
				i++;
				break;
			}
			//다른 문자면 msgTmp에 넣음
			else {
				msgTmp[j] = afterOrder[i];
				i++;
				j++;
			}
		}

		//괄호들 사이에 나오는 값이 values가 아니면 실패 
		if (!(strcmp(msgTmp, "values") == 0 || strcmp(msgTmp, "VALUES") == 0)) {
			return 0;
		}

		msgTmp[0] = '\0';
		j = 0;
		//데이터 값을 받는 반복 
		while (afterOrder[i] != '\0') {

			//괄호안에 공백이 나오는 경우에는 의미가 없어서 무시 
			if (afterOrder[i] == 32 || afterOrder[i] == 34) {
				i++;
				continue;

			}
			//쉼표가 나오는 경우에는 다음데이터를 입력받음 
			else if (afterOrder[i] == 44) {

				// 데이터가 없으면 실패 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				msgTmp[j] = '\0';
				//첫데이터가 NULL인경우 
				if (fdata == NULL) {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata = tdata;
					fdata = tdata;
				}
				//NULL이 아닌경우 
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


				//다음 데이터를 받기 위해 초기화 
				j = 0;
				msgTmp[0] = '\0';
				i++;
			}
			//괄호가 닫히면 데이터 끝 
			else if (afterOrder[i] == 41) {
				// 데이터가 없으면 실패 
				if (msgTmp[0] == '\0') {
					return 0;
				}
				msgTmp[j] = '\0';
				//첫데이터가 NULL인경우 
				if (fdata == NULL) {
					tdata = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
					tdata->after = NULL;
					tdata->before = NULL;
					strcpy(tdata->msg, msgTmp);
					ldata = tdata;
					fdata = tdata;
				}
				//NULL이 아닌경우 
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
			//일반 문자면 내용 추가 
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

			//첫타입이 NULL인경우 
			if (ftype == NULL) {
				ttype = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
				ttype->after = NULL;
				ttype->before = NULL;
				strcpy(ttype->msg, (*tmpField)->name);
				ftype = ttype;
				ltype = ttype;
			}
			//NULL이 아닌경우 
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
		// 타입이 없는경우는 처음이나 한 필드를 끝냈을 때 
		if (ttype == NULL) {
			ttype = ftype;
			tdata = fdata;
		}
		//아니면 ttype을 한칸 뒤로 해준다 
		else if (ttype->after != NULL) {
			ttype = ttype->after;
			tdata = tdata->after;
		}
		printf("insert4  %s   \n" , (*tmpField)->name);
		//타입이 맞는경우 
		if (strcmp((*tmpField)->name, ttype->msg) == 0) {


			//길이가 지정되어있는거보다 긴 경우 
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
			//데이터가 없는경우 튜플을 할당해주고 필드 데이터를 만들어서 튜플의 처음,마지막 데이터로 설정 
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
				//데이터타입에 따라 다른 값을 할당 
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
			//데이터가 있는경우 필드랑 데이터만 할당하고 튜플의 현재 데이터를 할당한 데이터로 한다 
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
		//ttype의 after가 NULL이면 해당 필드는 입력한 필드에 없다는 것이기 때문에 NOTNULL체크 
		else if (ttype->after == NULL || ttype == NULL) {
			//notNULL데이터인데 필드이름이 없으면 실패 
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


			//데이터가 없는경우 튜플을 할당해주고 필드 데이터를 만들어서 튜플의 처음,마지막 데이터로 설정 
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


				//데이터타입에 따라 다른 값을 할당 
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
			//데이터가 있는경우 필드랑 데이터만 할당하고 튜플의 현재 데이터를 할당한 데이터로 한다 
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
	//테이블에 만든 튜플을 링크연결 
	if ((*tmpTable)->firstData == NULL) {

		(*tmpTable)->firstData = (*tmpTuple);
		(*tmpTable)->currentData = (*tmpTuple);

	}
	else {

		(*tmpTable)->currentData->after = (*tmpTuple);
		(*tmpTuple)->before = (*tmpTable)->currentData;
		(*tmpTable)->currentData = (*tmpTuple);

	}

	// data파일에 튜플을 넣음 
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


//튜플을 가져오는 함수 
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

	//튜플을 가져오는 반복문
	while (1) {

		(*tmpField) = (*tmpTable)->field;
		(*tmpTuple) = NULL;
		while ((*tmpField) != NULL) {
			//처음 필드는 튜플이 없어서 튜플 생성 
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
			//튜플이 있으면 데이터의 현재 위치만 조정 
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



			//파일에서 데이터 하나를 받아옴
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
			//데이터 받는거 실패하면 값에 기본값을 넣음 
			if (read == -1) {
				free((*tmpTuple));
				free((*tmpData));
				break;
			}
			(*tmpTuple)->after = NULL;
			(*tmpTuple)->before = NULL;

			(*tmpField) = (*tmpField)->after;
		}

		//파일 다 읽었으면 반복문을 빠져나옴
		if (read == -1) {
			break;
		}
		fprintf(userFile, "\n");
		//테이블에 데이터가 없으면 처음 데이터로 아니면 현재 데이터로 넣는다 
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



// delete명령 
int deleteTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData) {
	char folderPath[1024];
	char pathTmp[1024];
	//기본경로 설정
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
	//from 이 없으면 실패 
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
	//테이블이 없는경우 실패 
	if (j == 0) {
		freeWhereCondition(&where);
		return 0;
	}

	i = findMsg(afterOrder, "where", "WHERE", i);
	//where 이 있으면 조건을 가져옴 
	if (i != -1) {

		i = findCondition(afterOrder, &where, &whereTmp, &whereEnd, i);
		if (i == -1) {
			freeWhereCondition(&where);
			return 0;
		}
		isCondition = 1;
	}


	while ((*tmpTuple) != NULL) {


		//조건을 체크한다 조건이 없는경우는 where이 없어서 1리턴 
		j = conditionCheck(&where, &whereTmp, tmpTuple);
		
		//조건에 맞는것만 삭제 
		if (j == 1) {
			//삭제 튜플이 테이블 튜플과 관련 있는 경우 테이블 튜플 수정
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

			//삭제 튜플의 링크들을 수정해줌 
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

	//링크를 삭제한 다음에 파일 수정 

	(*tmpTuple) = (*tmpTable)->firstData;
	j = writeTuple(tmpTuple, folderPath);

	return isDelete;

}


//update명령 
int updateTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData) {
	char folderPath[1024];
	char pathTmp[1024];
	//기본경로 설정
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
	printf("%d \n", i);


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



	//링크를 삭제한 다음에 파일 수정 

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

	//select필드값이 제대로 안되어있으면 실패 
	if (i == -1) {
		freeWhereCondition(&select);
		freeWhereCondition(&where);
		return 0;
	}

	//from을 찾음
	if (star == 0) {
		i = findMsg(afterOrder, "from", "FROM", i);
	}
	


	//from 이 없으면 실패 
	if (i == -1) {
		freeWhereCondition(&where);
		freeWhereCondition(&select);
		return 0;
	}

	//테이블 이름을 찾음 
	i = beforeSpace(afterOrder, msgTmp, i);


	strcpy(tableName, msgTmp);
	//테이블이 있는 테이블인지 체크 
	j = findTable(firstTable, lastTable, tmpTable, tableName);

	//없으면 실패 
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
	//그다음에 where을 찾음 
	i = findMsg(afterOrder, "where", "WHERE", i);
	if (i != -1) {
		//where이 있으면 조건을 가져옴 
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
	//	//필드이름이 같은 ㄱㅇ우 
	//	if (strcmp(selectTmp->field, (*tmpField)->name) == 0) {


	//		printf("%10s ", (*tmpField)->name);
	//				selectTmp = selectTmp->after;
	//	}
	//	//아닌경우 
	//	else {


	//		printf("%10s ", (*tmpField)->name);

	//		//체크해야하는 다음 필드가 있으면 다음 필드로 넘어가고 
	//		if ((*tmpField)->after != NULL) {
	//			(*tmpField) = (*tmpField)->after;
	//			(*tmpData) = (*tmpData)->after;

	//			//조건에 맞는 필드가 없으면 where절이 잘못된거여서 문법 오류
	//		}
	//		else {
	//			freeWhereCondition(&select);
	//			freeWhereCondition(&where);
	//			return 0;
	//		}

	//	}


	//}




	selectTmp = select;

	//줄바꿈 

	while ((*tmpTuple) != NULL) {


		//조건을 체크한다 조건이 없는경우는 where이 없어서 1리턴 
		j = conditionCheck(&where, &whereTmp, tmpTuple);

		//조건에 맞는 튜플 수정 
		if (j == 1) {

			//wresult = write(sockTmp->cSock, "\n=", 1024);
			
			selectTmp = select;
			(*tmpField) = (*tmpTuple)->firstField;
			(*tmpData) = (*tmpTuple)->firstData;
			while (selectTmp != NULL) {

				//필드 이름이 같은경우 
				if (strcmp(selectTmp->field, (*tmpField)->name) == 0) {

					//데이터 값을 출력
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

					//체크해야하는 다음 필드가 있으면 다음 필드로 넘어가고 
					if ((*tmpField)->after != NULL) {
						(*tmpField) = (*tmpField)->after;
						(*tmpData) = (*tmpData)->after;

						//조건에 맞는 필드가 없으면 where절이 잘못된거여서 문법 오류
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
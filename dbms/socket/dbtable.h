#include "dbfunction.h"


//CREATE TABLE 테이블이름 (필드이름1 타입이름, 필드이릅2 타입이름2 .....);
int createTable(struct dividedMsg** order , char afterOrder[] ,struct user** selectedUser ,struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable , struct field** lastField , struct field** tmpField) {


	// id/  경로를 폴더 경로 변수에 넣음 
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

	//두번째 문장엔 database가 나와야함

	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}


	if (!((*order) != NULL && (strcmp((*order)->msg, "table") == 0 || strcmp((*order)->msg, "TABLE") == 0 )  ) ){
		return 0;
	}
	//세번쨰 문장은 디비 이름이 나옴 


	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}

	if ((*selectedDb)->firstTable != NULL) {

		(*tmpTable) = (*selectedDb)->firstTable;

		//중복된 테이블 이름이 있는지 체크 

		while ((*tmpTable)->after != NULL) {


			//printf("compare  %s  and   %s \n",  (*order)->msg , (*tmpTable)->name);
			if (strcmp((*order)->msg, (*tmpTable)->name) == 0) {
				return 0;
			}
			(*tmpTable) = (*tmpTable)->after;
		}


	}

	(*tmpTable) = NULL;


	// 테이블을 할당하고 지금 넣을 수 있는 정보들을 넣어줌 
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
	//공백이 나오기 전까지는 name으로 공백 이후에는 type으로 
	while (afterOrder[i] != '\0') {

		

		
		if (isName == 1) {

			// 공백 나오면 그 다음문장은 타입이름
			if (afterOrder[i] == 32 && fName[0] != '\0') {
				isName = 0;
				isType = 1;
				i++;
				fName[j] = '\0';
				//j = 0;
				continue;
			}
			//처음 공백이 나오는 경우에는 그 공백은 무시 
			else if (afterOrder[i] == 32 && fName[0] == '\0') {
				i++;
				continue;
			}
			//문자가 나오면 이름에 추가 
			else {
				fName[j] = afterOrder[i];
				j++;
			}
		}
		//타입을 입력할 차례
		else if (isType == 1) {
			//여는 괄호가 나오면 타입이 끝나고 길이를 입력할 차례
			if (afterOrder[i] == 40) {
				fType[k] = '\0';
				// 타입이 int float char형 아니면 할당했던것들 전부 해제하고 명령 실패 
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
		//길이를 입력할 차례
		}else if(isLength == 1){




			//닫는 괄호가 나오면 길이 종료 
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
				//숫자가 아니면 문법적으로 잘못된거여서 실패
				if (!(afterOrder[i] >= 48 && afterOrder[i] <= 57)) {
					return 0;
				}
				//숫자의 첫자리가 0이 나오면 continue
				if (fLength[0] == '\0' && afterOrder[i] == 48) {
					i++;
					continue;
				}

				fLength[l] = afterOrder[i];
				l++;
			}



		}else if(isNull == 1) {
			
			
			printf("1\n");
			//닫는괄호가 나오면 명령어 종료 
			if (afterOrder[i] == 41) {
				isClose = 1;
				//이름과 필드의 마지막은 문장을 끝을 가르켜준다 

				//필드가 없는경우 필드를 할당해주고 이름과 타입을 넣어준다 
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
				// 필드가 있는경우 필드를 할당해주고 순서를 넣어준다 
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


				//테이블이 없는경우 현재 디비의 시작 테이블과 현재 테이블을 만든 테이블로 해주고 
				if ((*firstTable) == NULL) {

					(*tmpTable)->before = NULL;
					(*tmpTable)->after = NULL;
					(*selectedDb)->firstTable = (*tmpTable);
					(*selectedDb)->currentTable = (*tmpTable);
					(*firstTable) = (*tmpTable);
					(*lastTable) = (*tmpTable);
				}
				//테이블이 있으면 순서를 넣어준다 
				else {
					(*lastTable)->after = (*tmpTable);
					(*tmpTable)->before = (*lastTable);
					(*selectedDb)->currentTable->after = (*tmpTable);
					(*tmpTable)->before = (*selectedDb)->currentTable;
					(*lastTable) = (*tmpTable);
					(*selectedDb)->currentTable = (*tmpTable);


				}


				// 테이블 이름 폴더를 생성해준다 1
				strcpy(pathTmp, folderPath);
				strcat(pathTmp, (*tmpTable)->name);
				folderAdd = mkdir(pathTmp, 0755);
				//생성에 성공하면 테이블 파일에 테이블 이름을 추가해준다 
				if (folderAdd == 0) {

					strcpy(pathTmp, folderPath);
					strcat(pathTmp, "table.txt");
					FILE* userFile = fopen(pathTmp , "a");
					fprintf(userFile, "%s\n", (*tmpTable)->name);
					fclose(userFile);
					//테이블 이름을 추가한 다음에 필드 이름 파일을 추가해준다 
					strcpy(pathTmp, folderPath);
					strcat(pathTmp, (*tmpTable)->name);
					strcat(pathTmp, "/field.txt");


					//printf("%s \n", pathTmp);

					userFile = fopen(pathTmp, "a");
					(*tmpField) = (*tmpTable)->field;



					//필드 이름과 타입 입력 
					while ((*tmpField) != NULL) {
						//printf("%s %s %d %d\n", (*tmpField)->name, (*tmpField)->dataType , (*tmpField)->length , (*tmpField)->notNull );
						intTmp = fprintf(userFile, "%s %s %d %d\n", (*tmpField)->name, (*tmpField)->dataType,  (*tmpField)->length, (*tmpField)->notNull);
						//printf("intTmp = %d \n ", intTmp);
						(*tmpField) = (*tmpField)->after;
					}

					fclose(userFile);

				}
				//테이블 생성에 성공해서 리턴 1



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
			//괄호가 나오면 필드내용 완료
			else if (afterOrder[i] == 44) {

				//이름과 필드의 마지막은 문장을 끝을 가르켜준다 
				fName[j] = '\0';
  				fType[k] = '\0';
				fNull[m] = '\0';



				//필드가 없는경우 필드를 할당해주고 이름과 타입을 넣어준다 
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
				
				// 필드가 있는경우 필드를 할당해주고 순서를 넣어준다 
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

				//처음에 공백이 오는경우 처음 공백은 무시 
				if (fNull[0] == '\0' && afterOrder[i] == 32) {
					i++;
					continue;
				}
				//NOT NULL이 완성되고 나오는 문자들은 무시 ,나 )면 이쪽을 타지 않음  NOT NULL 이 없으면 NULL로 들어가기 때문에 NOT NULL이외에는 체크할 필요가 없음 
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


	//반복을 빠져나왔다는것은 괄호 닫는게 없는것이기 때문에 실패 그런데 메모리 할당은 했기때문에 할당 해제하고 리턴 
	freeField(&(*tmpTable)->field);
	free(tmpTable);
	return 0;
	i++;

}

//디비의 테이블을 가져오는 함수 
void getTable(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField , struct field** tmpField2 , struct tuple** tmpTuple , struct data** tmpData) {

	char folderPath[200];
	char pathTmp[200];
	char name[100];


	char fName[100];
	char fType[100];
	int fLength = 0;
	int fnotNull = 0;

	struct field* field2;
	//경로지정 
	strcpy(folderPath, (*tmpUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*tmpDb)->name);
	strcat(folderPath, "/");

	strcpy(pathTmp, folderPath);
	strcat(pathTmp, "table.txt");

	//테이블 파일 읽음
	FILE* userFile = fopen(pathTmp, "r");
	FILE* fuserFile = NULL;
	//테이블 파일이 없으면 리턴 
	if (userFile == NULL) {
		return;
	}

	//파일에서 테이블 정보를 하나씩 가져옴
	while (fscanf(userFile, "%s", name) != -1) {
		
		(*tmpTable) = (struct table*)malloc(sizeof(struct table));
		strcpy((*tmpTable)->name, name);

		// 테이블의 값을 넣어줌 
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



		//필드 경로 설정
		strcpy(pathTmp, folderPath);
		strcat(pathTmp, name);
		strcat(pathTmp, "/");
		strcat(pathTmp, "field.txt");
		//필드 파일을 읽음 
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



//drop table 테이ㅁ블이름
int dropTable(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable , struct field** tmpField) {

	char folderPath[100];
	char pathTmp[100];
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	strcat(folderPath, (*selectedDb)->name);
	strcat(folderPath, "/");

	(*tmpTable) = NULL;
	//drop 다음 메시지가 없으면 오류  
	if ((*order)->after != NULL) {
		(*order) = (*order)->after;
	}
	else {
		return 0;
	}
	

	//뒤의 문장이 테이블이 아니면 오류 
	if (!((*order) != NULL && (strcmp((*order)->msg, "table") == 0 || strcmp((*order)->msg, "TABLE") == 0))) {
		return 0;
	}

	// 이제 order의 msg는 삭제할 테이블 이름이 된다 
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


		//입력한 테이블이 있는경우 
		if ((*tmpTable) != NULL && (*order) != NULL && strcmp((*tmpTable)->name, (*order)->msg) == 0  ) {

			//삭제 유저가 전역변수와 관련 있는 경우 전역 변수 수정
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


			//삭제 유저의 링크들을 수정해줌 
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

			//삭제 디비 밑에 있는 파일들 전부 삭제 
			freeField( &(*tmpTable)->field );
			//해당 유저 디렉터리 안에 있는거 삭제 
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, (*order)->msg);
			rmdirs(pathTmp, 1);

			free((*tmpTable));
			// table 현재 상태로 새로고침 
			(*tmpTable) = (*firstTable);
			strcpy(pathTmp, folderPath);
			strcat(pathTmp, "table.txt");
			//파일 현재상태로 다시씀 
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
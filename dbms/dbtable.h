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
	int i = 0;
	int j = 0;
	int k = 0;
	int isName = 1;

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
	(*tmpTable)->field = NULL;
	(*tmpTable)->firstData = NULL;
	(*tmpTable)->currentData = NULL;
	(*tmpTable)->nextData = NULL;



	fName[0] = '\0';
	fType[0] = '\0';
	printf("after : %s \n", afterOrder);
	//공백이 나오기 전까지는 name으로 공백 이후에는 type으로 
	while (afterOrder[i] != '\0') {

		printf("%c %d \n", afterOrder[i], i);

		
		if (isName == 1) {

			// 공백 나오면 그 다음문장은 타입이름
			if (afterOrder[i] == 32 && fName[0] != '\0') {
				isName = 0;
				i++;
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
		else {
			//닫는괄호가 나오면 명령어 종료 
			if (afterOrder[i] == 41) {
				isClose = 1;
				//이름과 필드의 마지막은 문장을 끝을 가르켜준다 
				fName[j] = '\0';
				fType[k] = '\0';
				// 타입이 int float char형 아니면 할당했던것들 전부 해제하고 명령 실패 
				if (!(strcmp(fType, "INT") == 0 || strcmp(fType, "int") == 0 || strcmp(fType, "FLOAT") == 0 || strcmp(fType, "float") == 0 || strcmp(fType, "CHAR") == 0 || strcmp(fType, "char") == 0)) {

					freeField(&(*tmpTable)->field);
					free(tmpTable);
					return 0;

				}


				//필드가 없는경우 필드를 할당해주고 이름과 타입을 넣어준다 
				if ((*tmpTable)->field == NULL) {
					(*tmpTable)->field = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpTable)->field->name, fName);
					strcpy((*tmpTable)->field->dataType, fType);
					(*tmpTable)->field->before = NULL;
					(*tmpTable)->field->after = NULL;
					(*lastField) = (*tmpTable)->field;
					

				}
				// 필드가 있는경우 필드를 할당해주고 순서를 넣어준다 
				else {

					(*tmpField) = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpField)->name, fName);
					strcpy((*tmpField)->dataType, fType);
					(*lastField)->after = (*tmpField);
					(*tmpField)->before = (*lastField);
					(*tmpField)->after = NULL;
					(*lastField) = (*tmpField);
				}


				//테이블이 없는경우 현재 디비의 시작 테이블과 현재 테이블을 만든 테이블로 해주고 
				if ((*firstTable) == NULL) {

					(*tmpTable)->before = NULL;
					(*tmpTable)->after = NULL;
					(*selectedDb)->firstTable = (*tmpTable);
					(*selectedDb)->currentTable = (*tmpTable);
				}
				//테이블이 있으면 순서를 넣어준다 
				else {
					(*lastTable)->after = (*tmpTable);
					(*tmpTable)->before = (*lastTable);
					(*selectedDb)->currentTable->after = (*tmpTable);
					(*tmpTable)->before = (*selectedDb)->currentTable;

				}


				// 테이블 이름 폴더를 생성해준다 
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

					userFile = fopen(pathTmp, "a");
					(*tmpField) = (*tmpTable)->field;
					//필드 이름과 타입 입력 
					while ((*tmpField) != NULL) {
						fprintf(userFile, "%s %s\n", (*tmpField)->name, (*tmpField)->dataType);
						(*tmpField) = (*tmpField)->after;
					}
				}
				//테이블 생성에 성공해서 리턴 1
				return 1;

			}
			//콤마가 나오면 필드를 할당하고 그 필드의 이름과 타입을 넣어준다 
			else if (afterOrder[i] == 44) {
				printf("comma\n");
				//이름과 필드의 마지막은 문장을 끝을 가르켜준다 
				fName[j] = '\0';
  				fType[k] = '\0';

				printf("0\n");

				printf("%s \n", fType);
				// 타입이 int float char형 아니면 할당했던것들 전부 해제하고 명령 실패 
				if (!(strcmp(fType, "INT") == 0 || strcmp(fType, "int") == 0 || strcmp(fType, "FLOAT") == 0 || strcmp(fType, "float") == 0 || strcmp(fType, "CHAR") == 0 || strcmp(fType, "char") == 0)) {
				
					printf("4\n");
					freeField(&(*tmpTable)->field);
					printf("5\n");
					free((*tmpTable));
					return 0;

				}
				printf("1\n");
				//필드가 없는경우 필드를 할당해주고 이름과 타입을 넣어준다 
				if ((*tmpTable)->field == NULL) {
					(*tmpTable)->field = (struct field*)malloc(sizeof(struct field));
					strcpy((*tmpTable)->field->name, fName);
					strcpy((*tmpTable)->field->dataType, fType);
					(*tmpTable)->field->before = NULL;
					(*tmpTable)->field->after = NULL;
					(*lastField) = (*tmpTable)->field;
						
				}
				
				// 필드가 있는경우 필드를 할당해주고 순서를 넣어준다 
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
	//반복을 빠져나왔다는것은 괄호 닫는게 없는것이기 때문에 실패 그런데 메모리 할당은 했기때문에 할당 해제하고 리턴 
	freeField(&(*tmpTable)->field);
	free(tmpTable);
	return 0;
	i++;


	//테이블이름 추출 
	//strcpy(tableName, (*order)->msg);
	
	//tmpMsg[0] = '\0';
	//tmpMsg2[0] = '\0';
	//tableName[0] = '\0';
	//printf("1 :  %s \n ", (*order)->msg);
	//테이블이름뒤에 바로 괄호가 나오는지 체크 
	//isOpen  = openCheck((*order)->msg , tmpMsg , tmpMsg2);




	/*
	//괄호가 있든 없든 tmpMsg는 테이블이름 
	strcpy(tableName, tmpMsg);
	//테이블이름이 없으면 문법적으로 잘못됨
	if (tableName[0] == '\0') {
		return 0;
	}

	//괄호가 없는경우 괄호를 찾아야 한다 
	if (isOpen != 1) {
		tmpMsg[0] = '\0';
		tmpMsg2[0] = '\0';
		//괄호를 찾으려면 다음 문장으로 들어가야함 
		if ((*order)->after != NULL) {
			(*order) = (*order)->after;
		}
		else {
			return 0;
		}

		//다시 openCheck함수를 사용 정상적이라면 무조건 괄호가 있고 뒤에 문장이 있으면 tmpMsg2에 문장이 있을꺼고 아니면 괄호만 있음 
		isOpen = openCheck((*order)->msg, tmpMsg, tmpMsg2);

		//테이블 이름 뒤에도 괄호가 없으면 문법적으로 잘못됨
		if (isOpen == 0) {
			return 0;
		}
	}


	//괄호는 찾았는데 tmp2Msg가 있는경우와 없는경우가 생김 편의를 위해 있는경우에는 msg에 해당 메시지를 없는경우에는 다음 문장을 가리킨다
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


	// 명령문의 문장은 처음 필드이름을 가리킨다 이제 반복을 돌려서 필드 값들을 얻는다  이과정에서 메모리를 할당시켜야 하는데 괄호닫는게 없으면 다시 free시킴
	while ((*order)->after != NULL) {

		//필드이름을 넣어준다 
		strcpy(fName, (*order)->msg);

		// 필드 이름 뒤에는 타입이 붙어야 하기 때문이 뒤 문장이 없으면 실패 
		if ((*order)->after != NULL) {
			(*order) = (*order)->after;
		}
		else {
			return 0;
		}
		//필드이름뒤에 ,이 붙을수도 있고 )이 붙을수도 있음 
		tmpMsg[0] = '\0';
		tmpMsg2[0] = '\0';
		isClose = 0;
		isComma = 0;
		closeCheck((*order)->msg, tmpMsg, tmpMsg2 , &isClose , &isComma);
		//콤마가 없는경우에는 타입은 tmpMsg가 되고 뒤에 콤마가 있어야함 
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


	//괄호체크 
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
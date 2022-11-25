#include "dbfunction.h"



// 명령을 공백으로 분리하는 함수 where이 2개까지 올 수 있어서 where절의 괄호는 의미가 없기 떄문에
//괄호가 오는것은 INSERT문과 CREATE문이라고 생각하면 되기 때문에 괄호가 나오기 전까지는 공백으로 분리하고 괄호가 나오면 
void divide(char msg[] , char msgTmp[] , struct dividedMsg** dorderF , struct dividedMsg** dorderL, struct dividedMsg** dorderT) {

	char tmp[100] = "\0";
	char tmp2[2] = "\0";
	int i = 0;
	int isOpen = 0;
	int j = 0;
	int beforeMsg = 0;

	//문장이 끝이 나올떄까지 한문장씩 읽는다 
	while (msg[i] != '\0') {

		// 공백이면 그 전에 있었던 문자를 구조체를 할당하고 넣는다 


		if (isOpen == 0) {



			if (msg[i] == 40) {
				isOpen = 1;
				i++;
				continue;

			}else if (msg[i] == 32) {

				if (tmp[0] == 32) {
					continue;
				}

				(*dorderT) = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
				strcpy((*dorderT)->msg, tmp);
				if ((*dorderF) == NULL) {
					(*dorderF) = (*dorderT);
					(*dorderL) = (*dorderT);
				}
				else {
					(*dorderL)->after = (*dorderT);
					(*dorderL) = (*dorderT);

				}

				tmp[0] = '\0';

			}
			//공백이 아니면 문자를 추가한다 
			else {
				tmp2[0] = msg[i];
				tmp2[1] = '\0';
				strcat(tmp, tmp2);

			}












		}
		else {

			//이전문장도 공백이면 이번 공백은 무시 
			if (beforeMsg == 32 && msg[i] == 32) {
				i++;
				continue;
			}
			msgTmp[j++] = msg[i];
			beforeMsg = msg[i];
		


		}

		i++;
	}

	//반복이 종료되고 문자가 남아있으면 추가 
	if (tmp[0] != '\0') {
		(*dorderT) = (struct dividedMsg*)malloc(sizeof(struct dividedMsg));
		strcpy((*dorderT)->msg, tmp);
		if ((*dorderF) == NULL) {
			(*dorderF) = (*dorderT);
			(*dorderL) = (*dorderT);
		}
		else {
			(*dorderL)->after = (*dorderT);
			(*dorderL) = (*dorderT);


		}


	}

	msgTmp[j] = '\0';


}


//공백을 비우고 반환해주는 함수 
//char* zip(char msg[]) {
//
//	char orderTmp[1000];
//	int i = 0;
//	int j = 0;
//	while (msg[i] != '\0') {
//		//공백이 아닌경우에만 넣어준다 
//		if (msg[i] != 32) {
//			orderTmp[j++] = msg[i];
//		}
//		i++;
//
//	}
//
//	return orderTmp;
//
//}


// 사용자가 명령어를 입력하는 함수 
int userOrder(struct user** selectedUser , struct db** tmpDb , struct db** firstDb , struct db** lastDb , struct db** selectedDb , struct table** firstTable , struct table** lastTable , struct table** tmpTable) {
	char order[1000];
	char tmpOrder[1000];
	struct dividedMsg* dorderF = NULL;
	struct dividedMsg* dorderL = NULL;
	struct dividedMsg* dorderT = NULL;
	printf("============================================================\n");
	printf("=                                                          =\n");
	printf("=                     input Order                          =\n");
	printf("=                                                          =\n");

	printf("= input : ");
	//입력을 받고 입력 받은 값을 공백으로 분리 
	scanf("%[^\n]s", order);
	// printf("%s \n", order);
	divide(order, tmpOrder ,  &dorderF, &dorderL , &dorderT);

	(*tmpDb) = NULL;





	//create database인경우 
	if (strcmp(dorderF->msg, "CREATE") == 0 || strcmp(dorderF->msg, "create") == 0) {

		return createDb(&dorderF, selectedUser, tmpDb, firstDb, lastDb);

	}
	//drop databaSE인경우 
	else if( strcmp(dorderF->msg, "DROP") == 0 || strcmp(dorderF->msg, "drop") == 0) {

		return dropDb(&dorderF , selectedUser , tmpDb , firstDb , lastDb);
	}
	else  if (strcmp(dorderF->msg, "USE") == 0 || strcmp(dorderF->msg, "use") == 0) {
		return useDb(&dorderF , firstDb , tmpDb , selectedDb, firstTable, lastTable, tmpTable);
	}else{
		return 0;

	}

} 





// 데이터베이스 명령어 입력하는 함수 
int dbOrder(struct user** selectedUser ,struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable  , struct field** lastField , struct field** tmpField) {

	char order[1000];


	char tmpOrder[1000];
	struct dividedMsg* dorderF = NULL;
	struct dividedMsg* dorderL = NULL;
	struct dividedMsg* dorderT = NULL;
	printf("============================================================\n");
	printf("=                                                          =\n");
	printf("=                     input Order                          =\n");
	printf("=                                                          =\n");

	printf("= input : ");
	//입력을 받고 입력 받은 값을 공백으로 분리 
	scanf("%[^\n]s", order);
	// printf("%s \n", order);
	 divide(order , tmpOrder, &dorderF, &dorderL, &dorderT);

	(*tmpTable) = NULL;


	if (strcmp(dorderF->msg, "CREATE") == 0 || strcmp(dorderF->msg, "create") == 0) {


		return createTable(&dorderF, tmpOrder, selectedUser, selectedDb, firstTable, lastTable, tmpTable  , lastField , tmpField);
	}
	else {

		return 0;
	}
	//입력을 받고 입력 받은 값을 공백으로 분리



}
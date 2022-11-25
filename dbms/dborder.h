#include "dbfunction.h"



// ����� �������� �и��ϴ� �Լ� where�� 2������ �� �� �־ where���� ��ȣ�� �ǹ̰� ���� ������
//��ȣ�� ���°��� INSERT���� CREATE���̶�� �����ϸ� �Ǳ� ������ ��ȣ�� ������ �������� �������� �и��ϰ� ��ȣ�� ������ 
void divide(char msg[] , char msgTmp[] , struct dividedMsg** dorderF , struct dividedMsg** dorderL, struct dividedMsg** dorderT) {

	char tmp[100] = "\0";
	char tmp2[2] = "\0";
	int i = 0;
	int isOpen = 0;
	int j = 0;
	int beforeMsg = 0;

	//������ ���� ���Ë����� �ѹ��徿 �д´� 
	while (msg[i] != '\0') {

		// �����̸� �� ���� �־��� ���ڸ� ����ü�� �Ҵ��ϰ� �ִ´� 


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
			//������ �ƴϸ� ���ڸ� �߰��Ѵ� 
			else {
				tmp2[0] = msg[i];
				tmp2[1] = '\0';
				strcat(tmp, tmp2);

			}












		}
		else {

			//�������嵵 �����̸� �̹� ������ ���� 
			if (beforeMsg == 32 && msg[i] == 32) {
				i++;
				continue;
			}
			msgTmp[j++] = msg[i];
			beforeMsg = msg[i];
		


		}

		i++;
	}

	//�ݺ��� ����ǰ� ���ڰ� ���������� �߰� 
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


//������ ���� ��ȯ���ִ� �Լ� 
//char* zip(char msg[]) {
//
//	char orderTmp[1000];
//	int i = 0;
//	int j = 0;
//	while (msg[i] != '\0') {
//		//������ �ƴѰ�쿡�� �־��ش� 
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


// ����ڰ� ��ɾ �Է��ϴ� �Լ� 
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
	//�Է��� �ް� �Է� ���� ���� �������� �и� 
	scanf("%[^\n]s", order);
	// printf("%s \n", order);
	divide(order, tmpOrder ,  &dorderF, &dorderL , &dorderT);

	(*tmpDb) = NULL;





	//create database�ΰ�� 
	if (strcmp(dorderF->msg, "CREATE") == 0 || strcmp(dorderF->msg, "create") == 0) {

		return createDb(&dorderF, selectedUser, tmpDb, firstDb, lastDb);

	}
	//drop databaSE�ΰ�� 
	else if( strcmp(dorderF->msg, "DROP") == 0 || strcmp(dorderF->msg, "drop") == 0) {

		return dropDb(&dorderF , selectedUser , tmpDb , firstDb , lastDb);
	}
	else  if (strcmp(dorderF->msg, "USE") == 0 || strcmp(dorderF->msg, "use") == 0) {
		return useDb(&dorderF , firstDb , tmpDb , selectedDb, firstTable, lastTable, tmpTable);
	}else{
		return 0;

	}

} 





// �����ͺ��̽� ��ɾ� �Է��ϴ� �Լ� 
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
	//�Է��� �ް� �Է� ���� ���� �������� �и� 
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
	//�Է��� �ް� �Է� ���� ���� �������� �и�



}
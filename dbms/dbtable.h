#include "dbfunction.h"



//CREATE TABLE ���̺��̸� (�ʵ��̸�1 Ÿ���̸�, �ʵ��̸�2 Ÿ���̸�2 .....);
int createTable(struct dividedMsg** order , char afterOrder[] ,struct user** selectedUser ,struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable) {


	// id/  ��θ� ���� ��� ������ ���� 
	char folderPath[100];
	strcpy(folderPath, (*selectedUser)->id);
	strcat(folderPath, "/");
	char pathTmp[100];
	char tableName[100];
	int folderAdd = 0;
	int isOpen = 0;
	int isClose = 0;
	int isComma = 0;

	char tmpMsg[100];
	char tmpMsg2[100];


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
	

	strcpy(tableName, (*order)->msg);

	fName[0] = '\0';
	fType[0] = '\0';

	//������ ������ �������� name���� ���� ���Ŀ��� type���� 
	while (afterOrder[i] != '\0') {
		
		if (isName == 1) {
			// ���� ������ �� ���������� Ÿ���̸�
			if (afterOrder[i] == 32 && fName[0] != '\0') {
				isName = 0;
				i++;
				j = 0;

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



			}
			//�޸��� ������ �ʵ� �̸��� Ÿ���� ���� 
			else if (afterOrder[i] == 44) {

				isName = 0;


			}
			else {

				fType[k] = afterOrder[i];
				j++;


			}




		}


		i++;

	}



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
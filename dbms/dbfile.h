#include "dbfunction.h"



//select�ڿ� *�� �������� üũ�ϴ� �Լ� 
int findStar(char msg[]) {

	char tmpMsg[100];


	int i = 0;
	int j = 0;
	int star = 0;
	tmpMsg[0] = '\0';
	while (msg[i] != '\0') {

		if (msg[i] == 32) {
			tmpMsg[j] = '\0';
			
			printf("%s %d\n",  tmpMsg , star);
			if (tmpMsg[0] != '\0') {


				if (strcmp(tmpMsg, "*") == 0) {
					star = 1;

				}
				if (strcmp(tmpMsg, "from") == 0 || strcmp(tmpMsg, "from") == 0) {
					i++;
					if (star) {
						return i;

					}
					else {
						return 0;
					}

				}
			}

			j = 0;
		}else {

			tmpMsg[j] = msg[i];
			j++;


		}

		i++;
	}

	return -1;

}



//select���� �������� �Լ� 
int findSelect(char msg[], struct whereCondition** select, struct whereCondition** selectTmp, struct whereCondition** selectEnd, int start) {

	int i = start;
	int tmpI = 0;
	int j = 0;
	char tmpMsg[100];
	int k = 0;
	int findType = 0;
	int bk = 0;
	tmpMsg[0] = '\0';

	int isSpace = 0;


	//set�κ� ����  �ʵ��̸� -> = -> �� -> , ->���̰ų� where
	while (msg[i] != '\0') {
		//���� ����ü�� ������ �������� 
		if ((*selectTmp) == NULL) {

			(*selectTmp) = (struct whereCondition*)malloc(sizeof(struct whereCondition));
			(*selectTmp)->type = 0;
			(*selectTmp)->field[0] = '\0';
			(*selectTmp)->value[0] = '\0';
			(*selectTmp)->afterType = 0;
			(*selectTmp)->after = NULL;
			//set ���ۺκ�üũ
			k = 0;
		}

		//ó���� ������ ������ ���� 
		if (tmpMsg[0] == '\0' && msg[i] == 32) {
			break;
		}			//���Ŀ� ,�����ڰ� ������ �� �� 
		else if (msg[i] == 44) {
			tmpMsg[j] = '\0';
			strcpy((*selectTmp)->field, tmpMsg);

			//������ �ϳ��� ������ ó�� ������ �Ѵٿ� �ְ� ������ ������ ���ǿ� �ִ´� 
			if ((*select) == NULL) {

				(*select) = (*selectTmp);
				(*selectEnd) = (*selectTmp);
			}
			else {
				(*selectEnd)->after = (*selectTmp);
				(*selectEnd) = (*selectTmp);
			}

			(*selectTmp) = NULL;
			tmpMsg[0] = '\0';
			j = 0;
			findType = 0;
			k = 1;
			isSpace = 0;
		}			
		//���⼭ ������ ������ ���� �ְ� �����̶� �� �ڿ� ��ǥ�� �����ų� ���� �����ų� where�� ����  
		else if (msg[i] == 32) {
			isSpace = 1;
		}
		else if (msg[i] != 34 && msg[i] != 39) {

			//������ ���´����� �� ���ڰ� ������ break;
			if (isSpace) {
				bk = 1;
				break;
			}

			tmpMsg[j] = msg[i];
			j++;
		}

		i++;

	}

	if (k == 0) {
		//�ʵ��̸� ������ �������� 
		if ((*selectTmp)->field[0] == '\0') {
			freeWhereCondition(select);
			free((*selectTmp));
			return -1;
		}


		if (tmpMsg[0] != '\0') {

			tmpMsg[j] = '\0';
			strcpy((*selectTmp)->value, tmpMsg);
		}




		//�񱳰��� ������ �������� 
		if ((*selectTmp)->value[0] == '\0') {
			freeWhereCondition(select);
			free((*selectTmp));
			return -1;
		}


		//������ �ϳ��� ������ ó�� ������ �Ѵٿ� �ְ� ������ ������ ���ǿ� �ִ´� 
		if ((*select) == NULL) {
			(*select) = (*selectTmp);
			(*selectEnd) = (*selectTmp);
		}
		else {
			(*selectEnd)->after = (*selectTmp);
			(*selectEnd) = (*selectTmp);
		}



	}


	return i;




}


//update���� �������� �Լ� 
int findUpdate(char msg[], struct whereCondition** change, struct whereCondition** changeTmp, struct whereCondition** changeEnd, int start) {

	int i = start;
	int tmpI = 0;
	int j = 0;
	char tmpMsg[100];
	int k = 0;
	int findType = 0;
	int bk = 0;
	tmpMsg[0] = '\0';

	int isSpace = 0;


	//set�κ� ����  �ʵ��̸� -> = -> �� -> , ->���̰ų� where
	while (msg[i] != '\0') {
		//���� ����ü�� ������ �������� 
		if ((*changeTmp) == NULL) {

			(*changeTmp) = (struct whereCondition*)malloc(sizeof(struct whereCondition));
			(*changeTmp)->type = 0;
			(*changeTmp)->field[0] = '\0';
			(*changeTmp)->value[0] = '\0';
			(*changeTmp)->afterType = 0;
			(*changeTmp)->after = NULL;
			//set ���ۺκ�üũ
			k = 0;
		}


		switch (findType) {
		//�ʵ��̸� 
		case 0:
			// = �����ڰ� ����
			if ( msg[i] == 61) {
				tmpMsg[j] = '\0';
				strcpy((*changeTmp)->field, tmpMsg);
				j = 0;

				tmpMsg[j] = msg[i];
				findType = 1;
				isSpace = 0;

				j = 0;
				tmpMsg[0] = '\0';

			}
			//�����ڰ� �ƴϸ� �ʵ��̸��� �߰� 
			else if (msg[i] != 32) {
				tmpMsg[j] = msg[i];
				j++;
			}



			break;
			//�� 
		case 1:
			//ó���� ������ ������ ���� 
			if (tmpMsg[0] == '\0' && msg[i] == 32) {
				break;
			}
			//���Ŀ� ,�����ڰ� ������ �� �� 
			else if (msg[i] == 44) {
				tmpMsg[j] = '\0';
				strcpy((*changeTmp)->value, tmpMsg);

				//������ �ϳ��� ������ ó�� ������ �Ѵٿ� �ְ� ������ ������ ���ǿ� �ִ´� 
				if ((*change) == NULL) {

					(*change) = (*changeTmp);
					(*changeEnd) = (*changeTmp);
				}
				else {
					(*changeEnd)->after = (*changeTmp);
					(*changeEnd) = (*changeTmp);
				}

				(*changeTmp) = NULL;
				tmpMsg[0] = '\0';
				j = 0;
				findType = 0;
				k = 1;
				isSpace = 0;
			}
			//���⼭ ������ ������ ���� �ְ� �����̶� �� �ڿ� ��ǥ�� �����ų� ���� �����ų� where�� ����  
			else if (msg[i] == 32) {
				isSpace = 1;
			}
			else if(msg[i] != 34 && msg[i] != 39) {

				//������ ���´����� �� ���ڰ� ������ break;
				if (isSpace) {
					bk = 1;
					break;
				}

				tmpMsg[j] = msg[i];
				j++;
			}
			break;

		}


		if (bk == 1) {
			break;
		}

		i++;

	}

	if (k == 0) {
		//�ʵ��̸� ������ �������� 
		if ((*changeTmp)->field[0] == '\0') {
			freeWhereCondition(change);
			free((*changeTmp));
			return -1;
		}


		if (tmpMsg[0] != '\0') {

			tmpMsg[j] = '\0';
			strcpy((*changeTmp)->value, tmpMsg);
		}




		//�񱳰��� ������ �������� 
		if ((*changeTmp)->value[0] == '\0') {
			freeWhereCondition(change);
			free((*changeTmp));
			return -1;
		}


		//������ �ϳ��� ������ ó�� ������ �Ѵٿ� �ְ� ������ ������ ���ǿ� �ִ´� 
		if ((*change) == NULL) {
			(*change) = (*changeTmp);
			(*changeEnd) = (*changeTmp);
		}
		else {
			(*changeEnd)->after = (*changeTmp);
			(*changeEnd) = (*changeTmp);
		}



	}


	return i;




}
//Ʃ���� �ٽ� ���� �Լ� 
int writeTuple(struct tuple** tmpTuple, char filePath[]) {


	FILE* userFile = fopen(filePath, "w");


	struct field* fieldTmp = NULL;
	struct data* dataTmp = NULL;
	while ((*tmpTuple) != NULL) {

		//�ʵ尡 NULL�̸� Ʃ���� ó�� �ʵ带 �� 
		if (fieldTmp == NULL) {
			fieldTmp = (*tmpTuple)->firstField;
			dataTmp = (*tmpTuple)->firstData;
		}


		// ������Ÿ�Կ� ���� ���Ͽ� �Է�
		if (strcmp(fieldTmp->dataType, "CHAR") == 0 || strcmp(fieldTmp->dataType, "char") == 0 || strcmp(fieldTmp->dataType, "varchar") == 0 || strcmp(fieldTmp->dataType, "VARCHAR") == 0) {
			fprintf(userFile, "%s ", dataTmp->cdata);

		}
		else if (strcmp(fieldTmp->dataType, "float") == 0 || strcmp(fieldTmp->dataType, "FLOAT") == 0) {
			fprintf(userFile, "%f ", dataTmp->fdata);

		}
		else if (strcmp(fieldTmp->dataType, "int") == 0 || strcmp(fieldTmp->dataType, "INT") == 0) {
			fprintf(userFile, "%d ", dataTmp->idata);

		}

		//���� �ʵ�� �̵� 
		if (fieldTmp->after != NULL) {
			fieldTmp = fieldTmp->after;
			dataTmp = dataTmp->after;
		}
		//���� �ʵ尡 ������ ���� Ʃ�÷� �̵�
		else {
			fprintf(userFile, "\n");
			fieldTmp = NULL;
			dataTmp = NULL;
			(*tmpTuple) = (*tmpTuple)->after;
		}

	}
	fclose(userFile);
	return 1;
}


//���ǿ� �´� Ʃ������ üũ�ϴ� �Լ� 
int conditionCheck(struct whereCondition** where, struct whereCondition** whereTmp, struct tuple** tmpTuple) {



	int i = 0;
	int j = 0;

	int con1 = 0;
	int conTmp = 0;

	int beforeType = 0;
	int beforeValue = 0;
	char tmpMsg[100];
	(*whereTmp) = (*where);
	float fTmp;
	int iTmp;

	struct field* tmpField = (*tmpTuple)->firstField;
	struct data* tmpData = (*tmpTuple)->firstData;
	//������ ���������� üũ 
	while ( (*whereTmp) != NULL) {


		//�ʵ� �̸��� ������� 
		if (strcmp((*whereTmp)->field, tmpField->name) == 0) {

			con1 = 1;
			//�ʵ尡 �����ΰ�� 
			if (strcmp(tmpField->dataType, "CHAR") == 0 || strcmp(tmpField->dataType, "char") == 0 || strcmp(tmpField->dataType, "varchar") == 0 || strcmp(tmpField->dataType, "VARCHAR") == 0) {
				i = strcmp(tmpData->cdata, (*whereTmp)->value);
				switch ((*whereTmp)->type) {
					//������� 
					case 1 :
						//���� ������ ���� ���� 
						if (i != 0) {
							con1 = 0;
						}
					break;
					//���� ���� ��� 
					case 2:
						//������� ���� 
						if (i == 0) {
							con1 = 0;
						}
					break;
					//���� ū��� 
					case 3:

						if (i >= 0) {
							con1 = 0;
						}
					break;
					//���� ũ�ų� ������� 
					case 4:
						if (i > 0) {
							con1 = 0;
						}
					break;
					//�ʵ尡 ū��� 
					case 5:
						if (i <= 0) {
							con1 = 0;
						}
					break;
					//�ʵ尡 ũ�ų� ������� 
					case 6:
						if (i < 0) {
							con1 = 0;
						}
					break;
				}


			}
			//�ʵ尡 float�ΰ�� 
			else if (strcmp(tmpField->dataType, "float") == 0 || strcmp(tmpField->dataType, "FLOAT") == 0) {


				fTmp = atof2((*whereTmp)->value);
				switch ((*whereTmp)->type) {
					//������� 
				case 1:
					//���� ������ ���� ���� 
					if (tmpData->fdata != fTmp) {
						con1 = 0;
					}
					break;
					//���� ���� ��� 
				case 2:
					//������� ���� 
					if (tmpData->fdata == fTmp) {
						con1 = 0;
					}
					break;
					//���� ū��� 
				case 3:

					if (tmpData->fdata >= fTmp) {
						con1 = 0;
					}
					break;
					//���� ũ�ų� ������� 
				case 4:
					if (tmpData->fdata > fTmp) {
						con1 = 0;
					}
					break;
					//�ʵ尡 ū��� 
				case 5:
					if (tmpData->fdata <= fTmp) {
						con1 = 0;
					}
					break;
					//�ʵ尡 ũ�ų� ������� 
				case 6:
					if (tmpData->fdata < fTmp) {
						con1 = 0;
					}
					break;
				}
			}
			//�ʵ尡 int�ΰ�� 
			else if (strcmp(tmpField->dataType, "int") == 0 || strcmp(tmpField->dataType, "INT") == 0) {
				iTmp = atoi2((*whereTmp)->value);
				switch ((*whereTmp)->type) {
					//������� 
				case 1:
					//���� ������ ���� ���� 
					if (tmpData->idata != iTmp) {
						con1 = 0;
					}
					break;
					//���� ���� ��� 
				case 2:
					//������� ���� 
					if (tmpData->idata == iTmp) {
						con1 = 0;
					}
					break;
					//���� ū��� 
				case 3:

					if (tmpData->idata >= iTmp) {
						con1 = 0;
					}
					break;
					//���� ũ�ų� ������� 
				case 4:
					if (tmpData->idata > iTmp) {
						con1 = 0;
					}
					break;
					//�ʵ尡 ū��� 
				case 5:
					if (tmpData->idata <= iTmp) {
						con1 = 0;
					}
					break;
					//�ʵ尡 ũ�ų� ������� 
				case 6:
					if (tmpData->idata < iTmp ) {
						con1 = 0;
					}
					break;
				}
			}



			printf("con1 : %d\n", con1);
			//���� Ÿ���� or�ΰ�� 
			if (beforeType == 1) {

				//�Ѵ� 1�� �ƴϸ� ���ǿ� ���� ����
				if (!(con1 || conTmp)) {
					return 0;
				}

				//������ �켱������ ���ʿ��� �������̶� ���� ���°� 0�̿��� or�� 1�̵Ǽ� �� �������� 1�̶� ������ �� 
				conTmp = 1;
			}
			//����Ÿ���� and�ΰ�� 
			else if (beforeType == 2) {
				//���� �ϳ��� 1�� �ƴϸ� ���ǿ� ���� ����
				if (!(con1 && conTmp)) {
					return 0;
				}

				conTmp = 1;
			}
			else {
				conTmp = con1;
			}



			beforeType = (*whereTmp)->afterType;
			//���� �����ڰ� ���µ� �������� 0�̸� ���ǿ� ���� ���� 
			if (beforeType == 0 && conTmp == 0) {
				return 0;
			}


			//������������ �Ѿ 
			tmpField = (*tmpTuple)->firstField;
			tmpData = (*tmpTuple)->firstData;
			(*whereTmp) = (*whereTmp)->after;

		}

		//�ʵ��̸��� �ٸ��� ���� �ʵ�� �Ѿ 
		else {

			//üũ�ؾ��ϴ� ���� �ʵ尡 ������ ���� �ʵ�� �Ѿ�� 
			if (tmpField->after != NULL) {
				tmpField = tmpField->after;
				tmpData = tmpData->after;
				
			}
			//���ǿ� �´� �ʵ尡 ������ where���� �߸��Ȱſ��� ���� ����  
			else {
				freeWhereCondition(where);
				return -1;
			}


		}




	}

	return 1;
}


//���̺��� ã�� �Լ� 
int findTable(struct table** firstTable, struct table** lastTable, struct table** tmpTable, char tableName[]) {


	(*tmpTable) = (*firstTable);
	while ((*tmpTable) != NULL) {

		if (strcmp((*tmpTable)->name, tableName) == 0) {
			return 1;
		}
		(*tmpTable) = (*tmpTable)->after;
	}

	return 0;
}


//where���� ������ ã�� �Լ� 
int findCondition(char msg[], struct whereCondition **where , struct whereCondition **whereTmp, struct whereCondition **whereEnd ,  int start) {

	int i = start;
	int tmpI = 0;
	int j = 0;
	char tmpMsg[100];
	int k = 0;
	int findType = 0;
	int bk = 0;

	//where�� ����   �ʵ��̸�-> ������ -> ��  -> and �Ǵ� or   -> .... 
	tmpMsg[0] = '\0';
	while (msg[i] != '\0') {
		
		//���� ����ü�� ������ �������� 
		if ((*whereTmp) == NULL) {

			(*whereTmp) = (struct whereCondition*)malloc(sizeof(struct whereCondition));
			(*whereTmp)->type = 0;
			(*whereTmp)->field[0] = '\0';
			(*whereTmp)->value[0] = '\0';
			(*whereTmp)->afterType = 0;
			(*whereTmp)->after = NULL;

			k = 0;
		}


		switch (findType) {
			//�ʵ��̸� 
			case 0:
				//�����ڰ� ����
				if (msg[i] == 33 || msg[i] == 60 || msg[i] == 61 || msg[i] == 62) {
					tmpMsg[j] = '\0';
					strcpy((*whereTmp)->field, tmpMsg);
					j = 0;

					tmpMsg[j] = msg[i];
					findType = 1;
					j++;
				}
				//�����ڰ� �ƴϸ� �ʵ��̸��� �߰� 
				else if(msg[i] != 32){
					tmpMsg[j] = msg[i];
					j++;
				}



			break;
			//������ 
			case 1:

				//�����ڰ� �����°�� ������ ���ڸ� �߰��Ѵ� 
				if (msg[i] == 33 || msg[i] == 60 || msg[i] == 61 || msg[i] == 62) {
					tmpMsg[j] = msg[i];
					j++;

				}
				//�����ڰ� �ȳ����� ���  ������ ��
				else {
					tmpMsg[j] = '\0';
					
					//= �̸� 1   != �� 2  <�� 3  <=�� 4   >��5 >=�� 6
					if (strcmp(tmpMsg, "=") == 0) {
						(*whereTmp)->type = 1;
					}
					else if (strcmp(tmpMsg, "!=") == 0) {
						(*whereTmp)->type = 2;
					}
					else if (strcmp(tmpMsg, "<") == 0) {
						(*whereTmp)->type = 3;
					}
					else if (strcmp(tmpMsg, "<=") == 0) {
						(*whereTmp)->type = 4;
					}
					else if (strcmp(tmpMsg, ">") == 0) {
						(*whereTmp)->type = 5;
					}
					else if (strcmp(tmpMsg, ">=") == 0) {
						(*whereTmp)->type = 6;
					}
					//�����ڰ� �ƴϸ� ��� ���� 
					else {
						freeWhereCondition(where);
						free((*whereTmp));
						return -1;
					}
					tmpMsg[0] = '\0';
					j = 0;
					findType = 2;
				}

			break;
			//�� 
			case 2 :
				//ó���� ������ ������ ���� 
				if (tmpMsg[0] == '\0' && msg[i] == 32) {
					break;
				}
				//���Ŀ� ������ ������ �� ��
				else if (msg[i] == 32) {
					tmpMsg[j] = '\0';
					strcpy((*whereTmp)->value, tmpMsg);
					tmpMsg[0] = '\0';
					j = 0;
					findType = 3;
					tmpI = i;
				}
				//������ �ƴϸ� ���� ���� 
				else if(msg[i] != 34 && msg[i] != 39) {
					tmpMsg[j] = msg[i];
					j++;
				}
			break;

			//and �Ǵ� or�� �����°�� 
			case 3:
				
				//ó���� ������ ������ ���� 
				if (tmpMsg[0] == '\0' && msg[i] == 32) {
					break;
				}				
				//���Ŀ� ������ ������ �� ��
				else if (msg[i] == 32) {
					tmpMsg[j] = '\0';
					// and�� �����°��, or�� �����°�� �Ѵ� �ƴѰ�� 
					if ( strcmp(tmpMsg, "and" ) == 0   || strcmp(tmpMsg, "AND") == 0 ) {
						(*whereTmp)->afterType = 2;
					}
					else if (strcmp(tmpMsg, "or") == 0 || strcmp(tmpMsg, "OR") == 0 ) {
						(*whereTmp)->afterType = 1;
					}
					else {
						(*whereTmp)->afterType = 0;
						i = tmpI;
						bk = 1;
						break;
					}

					//������ �ϳ��� ������ ó�� ������ �Ѵٿ� �ְ� ������ ������ ���ǿ� �ִ´� 
					if ((*where) == NULL) {
						(*where) = (*whereTmp);
						(*whereEnd) = (*whereTmp);
					}
					else {
						(*whereEnd)->after = (*whereTmp);
						(*whereEnd) = (*whereTmp);

					}
					k = 1;


					tmpMsg[0] = '\0';
					j = 0;
					findType = 0;
					(*whereTmp) = NULL;
				}
				//������ �ƴϸ� ���� ���� 
				else {
					tmpMsg[j] = msg[i];
					j++;
				}
			break;
		}

		if (bk == 1) {
			break;
		}

		i++;
	}

	//����� ���� �ʾҴµ� ������ ������� �־���
	if (k == 0) {



		

		//Ÿ���� ������ ��������
		if ((*whereTmp)->type == 0) {
			freeWhereCondition(where);
			free((*whereTmp));
			return -1;
		}

		//�ʵ��̸� ������ �������� 
		if ((*whereTmp)->field[0] =='\0') {
			freeWhereCondition(where);
			free((*whereTmp));
			return -1;
		}


		if (tmpMsg[0] != '\0') {

			tmpMsg[j] = '\0';
			strcpy((*whereTmp)->value, tmpMsg);
		}



		//�񱳰��� ������ �������� 
		if ((*whereTmp)->value[0] == '\0') {
			freeWhereCondition(where);
			free((*whereTmp));
			return -1;
		}

		//������ �ϳ��� ������ ó�� ������ �Ѵٿ� �ְ� ������ ������ ���ǿ� �ִ´� 
		if ((*where) == NULL) {
			(*where) = (*whereTmp);
			(*whereEnd) = (*whereTmp);
		}
		else {
			(*whereEnd)->after = (*whereTmp);
			(*whereEnd) = (*whereTmp);

		}
	}

	return i;


}




//������ ���ö����� �ܾ �������ش� �Լ� 
int beforeSpace(char msg[], char returnMsg[], int start) {

	int i = start;
	int j = 0;
	returnMsg[0] = '\0';
	//�ܾ� �ݺ��� ������ ó�� ������ �����ϰ� ������ ���ö����� �ܾ �߰��Ѵ� 
	while (msg[i] != '\0') {
		if (returnMsg[0] == '\0' && msg[i] == 32) {
			i++;
			
			continue;
		}else if (msg[i] == 32) {
			i++;

			return i;
		}
		else {
			returnMsg[j] = msg[i];
			j++;
			i++;
			returnMsg[j] = '\0';
		}
	}

	//������� �°Ÿ� ������ ���°ſ��� -1���� 
	return -1;
}



//�޽��� �ȿ��� ������ ã�� ã�� ��ġ�� �����ϴ� �Լ� ������ -1 
int findMsg(char msg[], char findMsg[] , char findMsg2[] , int start) {

	int i = start;
	int j = 0;
	char tmpMsg[100];

	tmpMsg[0] = '\0';
	while (msg[i] != '\0') {
	
		//������ ������ ���� �ʱ�ȭ 
		if (msg[i] == 32) {
			i++;
			tmpMsg[0] = '\0';
			j = 0;
			
			continue;
		}
		//���� �ƴϸ� ���ڸ� ���� 
		else {
			tmpMsg[j] = msg[i];
			j++;
			tmpMsg[j] = '\0';

		}


		i++;

		//������ ������ i�� ���� 
		if (strcmp(findMsg, tmpMsg) == 0) {
			return i;

		}
		//������ ������ i�� ���� 
		if (strcmp(findMsg2, tmpMsg) == 0) {
			return i;

		}


	}
	return -1;
}



//���ڷ� ���� ���ڸ� ���ڷ� �ٲ��ִ� �Լ� 
int atoi2(char msg[]) {



	int i = 0;
	char tmp[100];
	int number = 0;

	for (i = 0; msg[i] != '\0'; i++) {

		//�������� 
		if (msg[i] == 32) {
			continue;
		}

		if (number == 0) {
			number = msg[i] - 48;
		}
		else {

			number *= 10;
			number += msg[i] - 48;
		}


	}


	return number;
}

//���ڸ� float������ ����� �Լ� 
float atof2(char msg[]) {

	float f = 0;
	char tmp[100];
	int i = 0;
	int j = 0;
	int isDot = 0;
	float number = 0;;
	float nunber2 = 0;
	float div = 10;
	while (msg[i] != '\0'){


		if (isDot == 0) {

			if (msg[i] == 46) {

				isDot = 1;
				i++;
				continue;

			}
			//�������� 
			if (msg[i] == 32) {
				i++;
				continue;
			}

			if (f == 0) {
				f = msg[i] - 48;
			}
			else {
				f *= 10;
				f += msg[i] - 48;
			}

		}
		else {			
			f += (msg[i] - 48) / div;
			div *= 10;
		}

		i++;
	}


	return f;
}

// ���۰�ȣ üũ�ϴ� �Լ� 
int openCheck(char msg[], char tmp[], char tmp2[]) {

	int i = 0;
	int j = 0;
	int k = 0;
	int isOpen = 0;

	//�޽����� ���������� �ݺ��ؼ� ��ȣ������ �������� tmp�� ���ڸ� ���� ��ȣ�� ������ tmp2�� ���ڸ� ���� 
	while (msg[i] != '\0') {



		if (msg[i] == 40) {
			i++;
			isOpen = 1; 
			continue;
		}


		if (isOpen == 0) {
			tmp[j++] = msg[i++];

		}
		else {

			tmp2[k++] = msg[i++];

		}
	}
	//�ݺ��� ������ \0�� �ٿ��� ���ڿ� ���� ��Ÿ�� 
	tmp[j] = '\0';
	tmp2[k] = '\0';



}

int closeCheck(char msg[], char tmp[], char tmp2[], int *isClose, int* isComma) {

	int i = 0;
	int j = 0;
	int k = 0;
	while (msg[i] != '\0') {
		//��ǥ�� �����°�� 
		if (msg[i] == 44) {
			i++;
			*isComma = 1;
			continue;
		}
		//��ȣ �ݴ°� ������ ���̻� �� �ʿ䰡 ����� break;
		else if (msg[i] == 41) {
			*isClose = 1;
			break;
		}

		//��ǥ�� �������� tmp2�� ���� 
		if (*isComma == 0) {
			tmp[j++] = msg[i++];

		}
		else {

			tmp2[k++] = msg[i++];

		}



	}


	tmp[j] = '\0';
	tmp2[k] = '\0';


}


// ���꽺Ʈ�� �Լ� 
void substring(char msg[], int start, int number) {

	char tmpMsg[100];
	int i = 0;
	int j = 0;


	//���̸� ���� ��� 
	if (number != 0) {
		for (i = start; i < start + number; i++) {
			tmpMsg[j] = msg[i];
			j++;
			if (msg[i] == '\0') {
				break;
			}
		}

	}
	//���̸� ���� ���� ��쿡�� ���� ������ �ִ´� 
	else {
		i = start;
		do {
			tmpMsg[j++] = msg[i++];
		} while (msg[i] != '\0');
		

	}
	strcpy(msg, tmpMsg);


}

// ���͸� �����ϸ� �ȿ� �ִ� ����/��������� �����ϴ� �Լ� 
int rmdirs(char* path, int force)
{
	DIR* dir_ptr = NULL;
	struct dirent* file = NULL;
	struct stat   buf;
	char   filename[1024];
	char pathTmp[1024];




	//���� �ȿ� ������ ������ ���� ���� 
	if ((dir_ptr = opendir(path)) == NULL) {
		return unlink(path);
	}

	//���� �ȿ� �����̳� ������ �ִ°�� �ϳ��� �о�� 
	while ((file = readdir(dir_ptr)) != NULL) {

		//printf("file -> %s \n ", file->d_name);
		// readdir ������ ���ϸ� �߿� ���� ���丮�� ��Ÿ�״� . �� ���ԵǾ� �����Ƿ� 
		// ���� �ݺ��� ������ �������� ���ϸ��� . �̸� skip �ؾ� ��
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
			continue;
		}

		sprintf(filename, "%s/%s", path, file->d_name);
		//printf("%s \n", file->d_name);
		/* ������ �Ӽ�(������ ����, ũ��, ����/���� �ð� ���� ��� ���Ͽ� */
		if (lstat(filename, &buf) == -1) {
			continue;
		}

		//printf("fileName :  %s \n", filename);

		//strcpy(pathTmp, path);
		//strcat(pathTmp, "/");
		//strcat(pathTmp, filename);
		//printf("pathTmp : %s \n", pathTmp);
		if (S_ISDIR(buf.st_mode)) { // �˻��� �̸��� �Ӽ��� ���丮�̸�
			/* �˻��� ������ directory�̸� ���ȣ��� ���� ���丮�� �ٽ� �˻� */
			if (rmdirs(filename, force) == -1 && !force) {
				return -1;
			}
		}
		else if (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) { // �Ϲ����� �Ǵ� symbolic link �̸�
			if (unlink(filename) == -1 && !force) {
				return -1;
			}
		}
	}

	/* open�� directory ������ close �մϴ�. */
	closedir(dir_ptr);

	return rmdir(path);
}



//���;��� Ű �Է¹����� ������ �����ϴ� �Լ� 
char getch(void)
{
	int ch;
	// �͹̳� io�� ������ �ΰ� ����
	struct termios old;
	struct termios new;

	// ���� �͹̳� io���� ������ 
	tcgetattr(0, &old);

	// ���ο���� ������ �������� �����ͼ� �ٲ��ִ� ���� 
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;

	//���� ������ ������ �͹̳� io���� �ٲ��� 
	tcsetattr(0, TCSAFLUSH, &new);
	// ���ڸ� �޾ƿ����� �ٽ� �������� �ٲ��� 
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &old);

	return ch;
}



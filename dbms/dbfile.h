#include "dbfunction.h"



//select뒤에 *가 나오는지 체크하는 함수 
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



//select값을 가져오는 함수 
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


	//set부분 순서  필드이름 -> = -> 값 -> , ->끝이거나 where
	while (msg[i] != '\0') {
		//조건 구조체가 없으면 생성해줌 
		if ((*selectTmp) == NULL) {

			(*selectTmp) = (struct whereCondition*)malloc(sizeof(struct whereCondition));
			(*selectTmp)->type = 0;
			(*selectTmp)->field[0] = '\0';
			(*selectTmp)->value[0] = '\0';
			(*selectTmp)->afterType = 0;
			(*selectTmp)->after = NULL;
			//set 시작부분체크
			k = 0;
		}

		//처음에 나오는 공백은 무시 
		if (tmpMsg[0] == '\0' && msg[i] == 32) {
			break;
		}			//이후에 ,연산자가 나오면 값 끝 
		else if (msg[i] == 44) {
			tmpMsg[j] = '\0';
			strcpy((*selectTmp)->field, tmpMsg);

			//조건이 하나도 없으면 처음 마지막 둘다에 넣고 있으면 마지막 조건에 넣는다 
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
		//여기서 공백이 나오면 값이 있고 공백이라 그 뒤에 쉼표가 나오거나 끝이 나오거나 where이 나옴  
		else if (msg[i] == 32) {
			isSpace = 1;
		}
		else if (msg[i] != 34 && msg[i] != 39) {

			//공백이 나온다음에 또 문자가 나오면 break;
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
		//필드이름 없으면 문법오류 
		if ((*selectTmp)->field[0] == '\0') {
			freeWhereCondition(select);
			free((*selectTmp));
			return -1;
		}


		if (tmpMsg[0] != '\0') {

			tmpMsg[j] = '\0';
			strcpy((*selectTmp)->value, tmpMsg);
		}




		//비교값이 없으면 문법오류 
		if ((*selectTmp)->value[0] == '\0') {
			freeWhereCondition(select);
			free((*selectTmp));
			return -1;
		}


		//조건이 하나도 없으면 처음 마지막 둘다에 넣고 있으면 마지막 조건에 넣는다 
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


//update값을 가져오는 함수 
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


	//set부분 순서  필드이름 -> = -> 값 -> , ->끝이거나 where
	while (msg[i] != '\0') {
		//조건 구조체가 없으면 생성해줌 
		if ((*changeTmp) == NULL) {

			(*changeTmp) = (struct whereCondition*)malloc(sizeof(struct whereCondition));
			(*changeTmp)->type = 0;
			(*changeTmp)->field[0] = '\0';
			(*changeTmp)->value[0] = '\0';
			(*changeTmp)->afterType = 0;
			(*changeTmp)->after = NULL;
			//set 시작부분체크
			k = 0;
		}


		switch (findType) {
		//필드이름 
		case 0:
			// = 연산자가 나옴
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
			//연산자가 아니면 필드이름을 추가 
			else if (msg[i] != 32) {
				tmpMsg[j] = msg[i];
				j++;
			}



			break;
			//값 
		case 1:
			//처음에 나오는 공백은 무시 
			if (tmpMsg[0] == '\0' && msg[i] == 32) {
				break;
			}
			//이후에 ,연산자가 나오면 값 끝 
			else if (msg[i] == 44) {
				tmpMsg[j] = '\0';
				strcpy((*changeTmp)->value, tmpMsg);

				//조건이 하나도 없으면 처음 마지막 둘다에 넣고 있으면 마지막 조건에 넣는다 
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
			//여기서 공백이 나오면 값이 있고 공백이라 그 뒤에 쉼표가 나오거나 끝이 나오거나 where이 나옴  
			else if (msg[i] == 32) {
				isSpace = 1;
			}
			else if(msg[i] != 34 && msg[i] != 39) {

				//공백이 나온다음에 또 문자가 나오면 break;
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
		//필드이름 없으면 문법오류 
		if ((*changeTmp)->field[0] == '\0') {
			freeWhereCondition(change);
			free((*changeTmp));
			return -1;
		}


		if (tmpMsg[0] != '\0') {

			tmpMsg[j] = '\0';
			strcpy((*changeTmp)->value, tmpMsg);
		}




		//비교값이 없으면 문법오류 
		if ((*changeTmp)->value[0] == '\0') {
			freeWhereCondition(change);
			free((*changeTmp));
			return -1;
		}


		//조건이 하나도 없으면 처음 마지막 둘다에 넣고 있으면 마지막 조건에 넣는다 
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
//튜플을 다시 쓰는 함수 
int writeTuple(struct tuple** tmpTuple, char filePath[]) {


	FILE* userFile = fopen(filePath, "w");


	struct field* fieldTmp = NULL;
	struct data* dataTmp = NULL;
	while ((*tmpTuple) != NULL) {

		//필드가 NULL이면 튜플의 처음 필드를 줌 
		if (fieldTmp == NULL) {
			fieldTmp = (*tmpTuple)->firstField;
			dataTmp = (*tmpTuple)->firstData;
		}


		// 데이터타입에 따라 파일에 입력
		if (strcmp(fieldTmp->dataType, "CHAR") == 0 || strcmp(fieldTmp->dataType, "char") == 0 || strcmp(fieldTmp->dataType, "varchar") == 0 || strcmp(fieldTmp->dataType, "VARCHAR") == 0) {
			fprintf(userFile, "%s ", dataTmp->cdata);

		}
		else if (strcmp(fieldTmp->dataType, "float") == 0 || strcmp(fieldTmp->dataType, "FLOAT") == 0) {
			fprintf(userFile, "%f ", dataTmp->fdata);

		}
		else if (strcmp(fieldTmp->dataType, "int") == 0 || strcmp(fieldTmp->dataType, "INT") == 0) {
			fprintf(userFile, "%d ", dataTmp->idata);

		}

		//다음 필드로 이동 
		if (fieldTmp->after != NULL) {
			fieldTmp = fieldTmp->after;
			dataTmp = dataTmp->after;
		}
		//다음 필드가 없으면 다음 튜플로 이동
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


//조건에 맞는 튜플인지 체크하는 함수 
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
	//조건이 없을때까지 체크 
	while ( (*whereTmp) != NULL) {


		//필드 이름이 같은경우 
		if (strcmp((*whereTmp)->field, tmpField->name) == 0) {

			con1 = 1;
			//필드가 문자인경우 
			if (strcmp(tmpField->dataType, "CHAR") == 0 || strcmp(tmpField->dataType, "char") == 0 || strcmp(tmpField->dataType, "varchar") == 0 || strcmp(tmpField->dataType, "VARCHAR") == 0) {
				i = strcmp(tmpData->cdata, (*whereTmp)->value);
				switch ((*whereTmp)->type) {
					//같은경우 
					case 1 :
						//같지 않으면 조건 실패 
						if (i != 0) {
							con1 = 0;
						}
					break;
					//같지 않은 경우 
					case 2:
						//같은경우 실패 
						if (i == 0) {
							con1 = 0;
						}
					break;
					//값이 큰경우 
					case 3:

						if (i >= 0) {
							con1 = 0;
						}
					break;
					//값이 크거나 같은경우 
					case 4:
						if (i > 0) {
							con1 = 0;
						}
					break;
					//필드가 큰경우 
					case 5:
						if (i <= 0) {
							con1 = 0;
						}
					break;
					//필드가 크거나 같은경우 
					case 6:
						if (i < 0) {
							con1 = 0;
						}
					break;
				}


			}
			//필드가 float인경우 
			else if (strcmp(tmpField->dataType, "float") == 0 || strcmp(tmpField->dataType, "FLOAT") == 0) {


				fTmp = atof2((*whereTmp)->value);
				switch ((*whereTmp)->type) {
					//같은경우 
				case 1:
					//같지 않으면 조건 실패 
					if (tmpData->fdata != fTmp) {
						con1 = 0;
					}
					break;
					//같지 않은 경우 
				case 2:
					//같은경우 실패 
					if (tmpData->fdata == fTmp) {
						con1 = 0;
					}
					break;
					//값이 큰경우 
				case 3:

					if (tmpData->fdata >= fTmp) {
						con1 = 0;
					}
					break;
					//값이 크거나 같은경우 
				case 4:
					if (tmpData->fdata > fTmp) {
						con1 = 0;
					}
					break;
					//필드가 큰경우 
				case 5:
					if (tmpData->fdata <= fTmp) {
						con1 = 0;
					}
					break;
					//필드가 크거나 같은경우 
				case 6:
					if (tmpData->fdata < fTmp) {
						con1 = 0;
					}
					break;
				}
			}
			//필드가 int인경우 
			else if (strcmp(tmpField->dataType, "int") == 0 || strcmp(tmpField->dataType, "INT") == 0) {
				iTmp = atoi2((*whereTmp)->value);
				switch ((*whereTmp)->type) {
					//같은경우 
				case 1:
					//같지 않으면 조건 실패 
					if (tmpData->idata != iTmp) {
						con1 = 0;
					}
					break;
					//같지 않은 경우 
				case 2:
					//같은경우 실패 
					if (tmpData->idata == iTmp) {
						con1 = 0;
					}
					break;
					//값이 큰경우 
				case 3:

					if (tmpData->idata >= iTmp) {
						con1 = 0;
					}
					break;
					//값이 크거나 같은경우 
				case 4:
					if (tmpData->idata > iTmp) {
						con1 = 0;
					}
					break;
					//필드가 큰경우 
				case 5:
					if (tmpData->idata <= iTmp) {
						con1 = 0;
					}
					break;
					//필드가 크거나 같은경우 
				case 6:
					if (tmpData->idata < iTmp ) {
						con1 = 0;
					}
					break;
				}
			}



			printf("con1 : %d\n", con1);
			//이전 타입이 or인경우 
			if (beforeType == 1) {

				//둘다 1이 아니면 조건에 맞지 않음
				if (!(con1 || conTmp)) {
					return 0;
				}

				//연산자 우선순위가 왼쪽에서 오른쪽이라 지금 나온게 0이여도 or로 1이되서 그 다음꺼는 1이랑 연산을 함 
				conTmp = 1;
			}
			//이전타입이 and인경우 
			else if (beforeType == 2) {
				//둘중 하나라도 1이 아니면 조건에 맞지 않음
				if (!(con1 && conTmp)) {
					return 0;
				}

				conTmp = 1;
			}
			else {
				conTmp = con1;
			}



			beforeType = (*whereTmp)->afterType;
			//다음 연산자가 없는데 이전값이 0이면 조건에 맞지 않음 
			if (beforeType == 0 && conTmp == 0) {
				return 0;
			}


			//다음조건으로 넘어감 
			tmpField = (*tmpTuple)->firstField;
			tmpData = (*tmpTuple)->firstData;
			(*whereTmp) = (*whereTmp)->after;

		}

		//필드이름이 다르면 다음 필드로 넘어감 
		else {

			//체크해야하는 다음 필드가 있으면 다음 필드로 넘어가고 
			if (tmpField->after != NULL) {
				tmpField = tmpField->after;
				tmpData = tmpData->after;
				
			}
			//조건에 맞는 필드가 없으면 where절이 잘못된거여서 문법 오류  
			else {
				freeWhereCondition(where);
				return -1;
			}


		}




	}

	return 1;
}


//테이블을 찾는 함수 
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


//where절의 조건을 찾는 함수 
int findCondition(char msg[], struct whereCondition **where , struct whereCondition **whereTmp, struct whereCondition **whereEnd ,  int start) {

	int i = start;
	int tmpI = 0;
	int j = 0;
	char tmpMsg[100];
	int k = 0;
	int findType = 0;
	int bk = 0;

	//where절 순서   필드이름-> 연산자 -> 값  -> and 또는 or   -> .... 
	tmpMsg[0] = '\0';
	while (msg[i] != '\0') {
		
		//조건 구조체가 없으면 생성해줌 
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
			//필드이름 
			case 0:
				//연산자가 나옴
				if (msg[i] == 33 || msg[i] == 60 || msg[i] == 61 || msg[i] == 62) {
					tmpMsg[j] = '\0';
					strcpy((*whereTmp)->field, tmpMsg);
					j = 0;

					tmpMsg[j] = msg[i];
					findType = 1;
					j++;
				}
				//연산자가 아니면 필드이름을 추가 
				else if(msg[i] != 32){
					tmpMsg[j] = msg[i];
					j++;
				}



			break;
			//연산자 
			case 1:

				//연산자가 나오는경우 연산자 문자를 추가한다 
				if (msg[i] == 33 || msg[i] == 60 || msg[i] == 61 || msg[i] == 62) {
					tmpMsg[j] = msg[i];
					j++;

				}
				//연산자가 안나오는 경우  연산자 끝
				else {
					tmpMsg[j] = '\0';
					
					//= 이면 1   != 면 2  <면 3  <=면 4   >면5 >=면 6
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
					//연산자가 아니면 명령 실패 
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
			//값 
			case 2 :
				//처음에 나오는 공백은 무시 
				if (tmpMsg[0] == '\0' && msg[i] == 32) {
					break;
				}
				//이후에 공백이 나오면 값 끝
				else if (msg[i] == 32) {
					tmpMsg[j] = '\0';
					strcpy((*whereTmp)->value, tmpMsg);
					tmpMsg[0] = '\0';
					j = 0;
					findType = 3;
					tmpI = i;
				}
				//공백이 아니면 값을 넣음 
				else if(msg[i] != 34 && msg[i] != 39) {
					tmpMsg[j] = msg[i];
					j++;
				}
			break;

			//and 또는 or이 나오는경우 
			case 3:
				
				//처음에 나오는 공백은 무시 
				if (tmpMsg[0] == '\0' && msg[i] == 32) {
					break;
				}				
				//이후에 공백이 나오면 값 끝
				else if (msg[i] == 32) {
					tmpMsg[j] = '\0';
					// and가 나오는경우, or이 나오는경우 둘다 아닌경우 
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

					//조건이 하나도 없으면 처음 마지막 둘다에 넣고 있으면 마지막 조건에 넣는다 
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
				//공백이 아니면 값을 넣음 
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

	//만들고 넣지 않았는데 문장이 끝난경우 넣어줌
	if (k == 0) {



		

		//타입이 없으면 문법오류
		if ((*whereTmp)->type == 0) {
			freeWhereCondition(where);
			free((*whereTmp));
			return -1;
		}

		//필드이름 없으면 문법오류 
		if ((*whereTmp)->field[0] =='\0') {
			freeWhereCondition(where);
			free((*whereTmp));
			return -1;
		}


		if (tmpMsg[0] != '\0') {

			tmpMsg[j] = '\0';
			strcpy((*whereTmp)->value, tmpMsg);
		}



		//비교값이 없으면 문법오류 
		if ((*whereTmp)->value[0] == '\0') {
			freeWhereCondition(where);
			free((*whereTmp));
			return -1;
		}

		//조건이 하나도 없으면 처음 마지막 둘다에 넣고 있으면 마지막 조건에 넣는다 
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




//공백이 나올때까지 단어를 리턴해준는 함수 
int beforeSpace(char msg[], char returnMsg[], int start) {

	int i = start;
	int j = 0;
	returnMsg[0] = '\0';
	//단어 반복을 돌려서 처음 공백은 무시하고 공백이 나올때까지 단어를 추가한다 
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

	//여기까지 온거면 공백은 없는거여서 -1리턴 
	return -1;
}



//메시지 안에서 문장을 찾고 찾은 위치를 리턴하는 함수 없으면 -1 
int findMsg(char msg[], char findMsg[] , char findMsg2[] , int start) {

	int i = start;
	int j = 0;
	char tmpMsg[100];

	tmpMsg[0] = '\0';
	while (msg[i] != '\0') {
	
		//공백이 나오면 문자 초기화 
		if (msg[i] == 32) {
			i++;
			tmpMsg[0] = '\0';
			j = 0;
			
			continue;
		}
		//공백 아니면 문자를 넣음 
		else {
			tmpMsg[j] = msg[i];
			j++;
			tmpMsg[j] = '\0';

		}


		i++;

		//문장이 같으면 i를 리턴 
		if (strcmp(findMsg, tmpMsg) == 0) {
			return i;

		}
		//문장이 같으면 i를 리턴 
		if (strcmp(findMsg2, tmpMsg) == 0) {
			return i;

		}


	}
	return -1;
}



//문자로 받은 숫자를 숫자로 바꿔주는 함수 
int atoi2(char msg[]) {



	int i = 0;
	char tmp[100];
	int number = 0;

	for (i = 0; msg[i] != '\0'; i++) {

		//공백제거 
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

//문자를 float형으로 만드는 함수 
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
			//공백제거 
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

// 시작괄호 체크하는 함수 
int openCheck(char msg[], char tmp[], char tmp2[]) {

	int i = 0;
	int j = 0;
	int k = 0;
	int isOpen = 0;

	//메시지가 끝날떄까지 반복해서 괄호나오기 전까지는 tmp에 문자를 저장 괄호가 나오면 tmp2에 문자를 저장 
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
	//반복이 끝나면 \0을 붙여서 문자열 끝을 나타냄 
	tmp[j] = '\0';
	tmp2[k] = '\0';



}

int closeCheck(char msg[], char tmp[], char tmp2[], int *isClose, int* isComma) {

	int i = 0;
	int j = 0;
	int k = 0;
	while (msg[i] != '\0') {
		//쉼표가 나오는경우 
		if (msg[i] == 44) {
			i++;
			*isComma = 1;
			continue;
		}
		//괄호 닫는게 나오면 더이상 볼 필요가 없어사 break;
		else if (msg[i] == 41) {
			*isClose = 1;
			break;
		}

		//쉼표가 나왔으면 tmp2에 넣음 
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


// 서브스트링 함수 
void substring(char msg[], int start, int number) {

	char tmpMsg[100];
	int i = 0;
	int j = 0;


	//길이를 받은 경우 
	if (number != 0) {
		for (i = start; i < start + number; i++) {
			tmpMsg[j] = msg[i];
			j++;
			if (msg[i] == '\0') {
				break;
			}
		}

	}
	//길이를 받지 않은 경우에는 문자 끝까지 넣는다 
	else {
		i = start;
		do {
			tmpMsg[j++] = msg[i++];
		} while (msg[i] != '\0');
		

	}
	strcpy(msg, tmpMsg);


}

// 디렉터리 삭제하면 안에 있는 파일/폴더들까지 삭제하는 함수 
int rmdirs(char* path, int force)
{
	DIR* dir_ptr = NULL;
	struct dirent* file = NULL;
	struct stat   buf;
	char   filename[1024];
	char pathTmp[1024];




	//폴더 안에 파일이 없으면 폴더 삭제 
	if ((dir_ptr = opendir(path)) == NULL) {
		return unlink(path);
	}

	//폴더 안에 파일이나 폴더가 있는경우 하나씩 읽어옴 
	while ((file = readdir(dir_ptr)) != NULL) {

		//printf("file -> %s \n ", file->d_name);
		// readdir 읽혀진 파일명 중에 현재 디렉토리를 나타네는 . 도 포함되어 있으므로 
		// 무한 반복에 빠지지 않으려면 파일명이 . 이면 skip 해야 함
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
			continue;
		}

		sprintf(filename, "%s/%s", path, file->d_name);
		//printf("%s \n", file->d_name);
		/* 파일의 속성(파일의 유형, 크기, 생성/변경 시간 등을 얻기 위하여 */
		if (lstat(filename, &buf) == -1) {
			continue;
		}

		//printf("fileName :  %s \n", filename);

		//strcpy(pathTmp, path);
		//strcat(pathTmp, "/");
		//strcat(pathTmp, filename);
		//printf("pathTmp : %s \n", pathTmp);
		if (S_ISDIR(buf.st_mode)) { // 검색된 이름의 속성이 디렉토리이면
			/* 검색된 파일이 directory이면 재귀호출로 하위 디렉토리를 다시 검색 */
			if (rmdirs(filename, force) == -1 && !force) {
				return -1;
			}
		}
		else if (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) { // 일반파일 또는 symbolic link 이면
			if (unlink(filename) == -1 && !force) {
				return -1;
			}
		}
	}

	/* open된 directory 정보를 close 합니다. */
	closedir(dir_ptr);

	return rmdir(path);
}



//엔터없이 키 입력받으면 데이터 저장하는 함수 
char getch(void)
{
	int ch;
	// 터미널 io값 변수를 두개 만듬
	struct termios old;
	struct termios new;

	// 기존 터미널 io값을 가져옴 
	tcgetattr(0, &old);

	// 새로운것의 설정도 기존값을 가져와서 바꿔주는 형식 
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;

	//새로 설정된 값으로 터미널 io값을 바꿔줌 
	tcsetattr(0, TCSAFLUSH, &new);
	// 문자를 받아오고나서 다시 원래껄로 바꿔줌 
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &old);

	return ch;
}



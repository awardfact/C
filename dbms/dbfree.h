#include "dbfunction.h"

//필드 메모리 할당 해제 함수 
void freeField(struct field** freeF) {

	struct field* tmpF = (*freeF);

	while (1) {

		//널이 아닌경우 freeF는 다음으로 보내고 tmpF는 할당 해제 
		if (tmpF != NULL) {


			(*freeF) = (*freeF)->after;
			free(tmpF);
			tmpF = (*freeF);
		}
		else {
			break;
		}


	}
}



//데이터  메모리 할당 해제 함수 
void freeData(struct data** freeF) {

	struct data* tmpF = (*freeF);

	while (1) {

		//널이 아닌경우 freeF는 다음으로 보내고 tmpF는 할당 해제 
		if (tmpF != NULL) {


			(*freeF) = (*freeF)->after;
			free(tmpF);
			tmpF = (*freeF);
		}
		else {
			break;
		}


	}
}


// 메시지 메모리 할당 해제 함수 
void freeMsg(struct dividedMsg** freeD) {

	struct dividedMsg* tmpF = (*freeD);
	while (1) {

		//널이 아닌경우 freeF는 다음으로 보내고 tmpF는 할당 해제 
		if (tmpF != NULL) {
			(*freeD) = (*freeD)->after;
			free(tmpF);
			tmpF = (*freeD);
		}
		else {
			break;
		}


	}




}


//구조체 메모리 해제 함수 
void freeWhereCondition(struct whereCondition** freeW) {


	struct whereCondition* tmpW = (*freeW);
	while (1) {

		//널이 아닌경우 freeF는 다음으로 보내고 tmpF는 할당 해제 
		if (tmpW != NULL) {
			(*freeW) = (*freeW)->after;
			free(tmpW);
			tmpW = (*freeW);
		}
		else {
			break;
		}
	}
}
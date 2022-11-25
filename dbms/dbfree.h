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
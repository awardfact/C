#include "dbfunction.h"

//�ʵ� �޸� �Ҵ� ���� �Լ� 
void freeField(struct field** freeF) {

	struct field* tmpF = (*freeF);

	while (1) {

		//���� �ƴѰ�� freeF�� �������� ������ tmpF�� �Ҵ� ���� 
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
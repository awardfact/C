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



//������  �޸� �Ҵ� ���� �Լ� 
void freeData(struct data** freeF) {

	struct data* tmpF = (*freeF);

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


// �޽��� �޸� �Ҵ� ���� �Լ� 
void freeMsg(struct dividedMsg** freeD) {

	struct dividedMsg* tmpF = (*freeD);
	while (1) {

		//���� �ƴѰ�� freeF�� �������� ������ tmpF�� �Ҵ� ���� 
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


//����ü �޸� ���� �Լ� 
void freeWhereCondition(struct whereCondition** freeW) {


	struct whereCondition* tmpW = (*freeW);
	while (1) {

		//���� �ƴѰ�� freeF�� �������� ������ tmpF�� �Ҵ� ���� 
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
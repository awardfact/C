

//����ȭ�� �����ִ� �Լ� 
void showMain(char mMsg[], struct sockS* sockTmp) {

	int i = 0;
	printf("============================================================\n");
	printf("=                                                          =\n");
	printf("=                                                          =\n");


	//����ȭ������ �Ë� Ư�� �޽����� �ִ°�� �޽������ 
	printf("=");
	for (i = (56 - strlen(mMsg)) / 2; i >= 0; i--) printf(" ");
	printf("%s", mMsg);
	for (i = (56 - strlen(mMsg)) / 2; i >= 0; i--) printf(" ");
	if (strlen(mMsg) % 2 != 0) printf(" ");
	printf("=\n");
	//printf("=                                                          =\n")l
	printf("=                       Hello SQL!                         =\n");
	printf("=                   Exit Program Input 0                   =\n");
	printf("=                      Login Input 1                       =\n");
	printf("=                     Sign up Input 2                      =\n");
	printf("=                     Sign out Input 3                     =\n");
	printf("=                                                          =\n");
	printf("=                                                          =\n");
	printf("============================================================\n");

}


void showLogin(char mMsg[] , char loginId[], struct sockS* sockTmp) {

	int i = 0;
	printf("============================================================\n");
	printf("=                                                          =\n");
	printf("=                                                          =\n");


	//����ȭ������ �Ë� Ư�� �޽����� �ִ°�� �޽������ 
	printf("=");
	for (i = (56 - strlen(mMsg)) / 2; i >= 0; i--) printf(" ");
	printf("%s", mMsg);
	for (i = (56 - strlen(mMsg)) / 2; i >= 0; i--) printf(" ");
	if (strlen(mMsg) % 2 != 0) printf(" ");
	printf("=\n");

	//���� �α������� ���� ���̵� ��� 
	printf("=");
	for (i = (50 - strlen(loginId)) / 2; i >= 0; i--) printf(" ");
	printf("Hello ");
	printf("%s", loginId);
	for (i = (50 - strlen(loginId)) / 2; i >= 0; i--) printf(" ");
	if (strlen(loginId) % 2 != 0) printf(" ");
	printf("=\n");


	printf("=                   Exit Program Input 0                   =\n");
	printf("=                    input Order input 1                   =\n");
	printf("=                     logOut Input 2                       =\n");
	printf("=                                                          =\n");
	printf("=                                                          =\n");
	printf("============================================================\n");



}




void showDb(char mMsg[], char loginId[] , char dbName[], struct sockS* sockTmp) {

	int i = 0;
	printf("============================================================\n");
	printf("=                                                          =\n");
	printf("=                                                          =\n");


	//����ȭ������ �Ë� Ư�� �޽����� �ִ°�� �޽������ 
	printf("=");
	for (i = (56 - strlen(mMsg)) / 2; i >= 0; i--) printf(" ");
	printf("%s", mMsg);
	for (i = (56 - strlen(mMsg)) / 2; i >= 0; i--) printf(" ");
	if (strlen(mMsg) % 2 != 0) printf(" ");
	printf("=\n");

	//���� �α������� ���� ���̵� ��� 
	printf("=");
	for (i = (50 - strlen(loginId)) / 2; i >= 0; i--) printf(" ");
	printf("Hello ");
	printf("%s", loginId);
	for (i = (50 - strlen(loginId)) / 2; i >= 0; i--) printf(" ");
	if (strlen(loginId) % 2 != 0) printf(" ");
	printf("=\n");



	//���� �α������� ���� ���̵� ��� 
	printf("=");
	for (i = (50 - strlen(dbName)) / 2; i >= 0; i--) printf(" ");
	printf(" DB : ");
	printf("%s", dbName);
	for (i = (50 - strlen(dbName)) / 2; i >= 0; i--) printf(" ");
	if (strlen(dbName) % 2 != 0) printf(" ");
	printf("=\n");


	printf("=                   Exit Program Input 0                   =\n");
	printf("=                    input Order input 1                   =\n");
	printf("=                      DBOut Input 2                       =\n");
	printf("=                                                          =\n");
	printf("=                                                          =\n");
	printf("============================================================\n");
}
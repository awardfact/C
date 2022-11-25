



//dbshow ��� �Լ� 
void showMain();   // ����ȭ�� ��� �Լ� 
void showDb(char mMsg[], char loginId[], char dbName[]); // ��� ȭ�� ��� �Լ� 
void showLogin();   // �α����Լ� ��� �Լ� 
//dbshow ��� �Լ� 

//dbfile��� �Լ� 
int rmdirs(char* path, int force); // ���͸� �ȿ��ִ� ����/���� ���� �����ϴ� �Լ� 
char getch();   // Ű�� ������ �ԷµǴ� �Լ� 
void substring(char msg[], int start, int end);  //substriong �Լ� 
int openCheck(char msg[], char tmp[], char tmp2[]); // ���۰�ȣ üũ�ϴ� �Լ� 
//dbfile��� �Լ� 


//dborder ��� �Լ� 
char* divide(char msg[], struct dividedMsg** dorderF, struct dividedMsg** dorderL, struct dividedMsg** dorderT);  // ����� �޾Ƽ� ����� �������� ������ �Լ� 
int userOrder(struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);  // �α��� ���¿��� �����ͺ��̽� ��ɾ� ��� �Լ�   CREATE , DROP   ->database 
int dbOrder(struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable); // db��� �Լ� 
//dborder ��� �Լ� 




//dbuser��� �Լ� 
int addUser(char id[], char password[], struct user** firstUser, struct user** lastUser, struct user** tmpUser);   //���̵�� �н����带 �ް� ������ �����ϴ� �Լ�
void getUser();   // ������ �� ȸ�� ���� ���Ͽ��� �ҷ����� �Լ� 
int deleteUser(char id[], char password[], struct user** firstUser, struct user** lastUser, struct user** tmpUser); // ����� ���� �Լ� 
int login(char id[], char passwd[], struct user** firstUser, struct user** selectedUser, struct user** tmpUser);   // �α����ϴ� �Լ� 
int signUp(struct user** firstUser, struct user** lastUser, struct user** tmpUser);   // ȸ������ �Լ� 
int signOut(struct user** firstUser, struct user** lastUser, struct user** tmpUser);   //ȸ��Ż�� �Լ� 
int logOut(struct user** selectedUser);  //�α׾ƿ� �Լ� 
//dbuser��� �Լ� 




//dbdb ��� �Լ� 
int createDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb); // ��� ���� �Լ�
int dropDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb);   // ��� �����Լ�
int useDb(struct dividedMsg** order, struct db** firstDb, struct db** tmpDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);// ��� ��� �Լ� 
void getDb(struct user** tmpUser);  //�������� ��� ������ �������� �Լ� 
//dbdb ��� �Լ� 


//dbTable��� �Լ� 
int createTable(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);//CREATE TABLE ���̺��̸� (�ʵ��̸�1 Ÿ���̸�, �ʵ��̸�2 Ÿ���̸�2 .....);
//dbTable��� �Լ� 












int deleteTuple(struct tuple* deleteTuple); // Ʃ�� ���� �Լ� 

int dropTable(struct dividedMsg*);  //DROP TABLE ���̺��̸�;
int insertTable(struct dividedMsg*);// INSERT INTO ���̺��̸�(�ʵ��̸�1, �ʵ��̸�2, �ʵ��̸�3 ...)VALUES(�����Ͱ�1,�����Ͱ�2,�����Ͱ�3....)  OR  INSERT INTO ���̺��̸�VALUES(�����Ͱ�1, �����Ͱ�2,�����Ͱ�3.....)
int updateTable(struct dividedMsg*); //UPDATE ���̺��̸� SET �ʵ��̸�1=�����Ͱ�1, �ʵ��̸�2=�����Ͱ�2..... where ����
int deleteTable(struct dividedMsg*); //DELETE FROM ���̺��̸� where ����
int selectTable(struct dividedMsg*); //SELECT �ʵ��̸�1 , �ʵ��̸�2 .... from ���̺��̸� where ����  group by �ʵ�


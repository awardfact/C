



//dbshow ��� �Լ� 
void showMain(char mMsg[], struct sockS* sockTmp);   // ����ȭ�� ��� �Լ� 
void showDb(char mMsg[], char loginId[], char dbName[], struct sockS* sockTmp); // ��� ȭ�� ��� �Լ� 
void showLogin(char mMsg[], char loginId[], struct sockS* sockTmp);   // �α����Լ� ��� �Լ� 
//dbshow ��� �Լ� 

//dbfile��� �Լ� 
int rmdirs(char* path, int force); // ���͸� �ȿ��ִ� ����/���� ���� �����ϴ� �Լ� 
char getch();   // Ű�� ������ �ԷµǴ� �Լ� 
void substring(char msg[], int start, int end);  //substriong �Լ� 
int openCheck(char msg[], char tmp[], char tmp2[]); // ���۰�ȣ üũ�ϴ� �Լ� 
int atoi2(char msg[]); // ���ڷ� �̷���� ���ڸ� ���������� �ٲٴ� �Լ� 
float atof2(char msg[]); //���ڷ� �̷����� �Ǽ��� �Ǽ��� �ٲٴ� �Լ� 
int findMsg(char msg[], char findMsg[], char findMsg2[], int start); // ���ڸ� ã�� ��ġ�� �����ϴ� �Լ� 
int findCondition(char msg[], struct whereCondition** where, struct whereCondition** whereTmp, struct whereCondition** whereEnd, int start); // where���� ������ ã�� �Լ� 
int findTable(struct table** firstTable, struct table** lastTable, struct table** tmpTable, char tableName[]); //���̺��� ã�� �Լ� 
int conditionCheck(struct whereCondition** where, struct whereCondition** whereTmp, struct tuple** tmpTuple); //������ �´� Ʃ������ üũ�ϴ� �Լ� 
int findUpdate(char msg[], struct whereCondition** change, struct whereCondition** changeTmp, struct whereCondition** changeEnd, int start); //update������ �������� �Լ� 
int findSelect(char msg[], struct whereCondition** select, struct whereCondition** selectTmp, struct whereCondition** selectEnd, int start); //select������ �������� �Լ� 
int findStar(char msg[]); //*�� ã�� �Լ� 
//dbfile��� �Լ� 


//dborder ��� �Լ� 
void divide(char msg[], char msgTmp[], struct dividedMsg** dorderF, struct dividedMsg** dorderL, struct dividedMsg** dorderT);  // ����� �޾Ƽ� ����� �������� ������ �Լ� 
int userOrder(struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct sockS* sockTmp);  // �α��� ���¿��� �����ͺ��̽� ��ɾ� ��� �Լ�   CREATE , DROP   ->database 
int dbOrder(struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** lastField, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData, struct sockS* sockTmp); // db��� �Լ� 
//dborder ��� �Լ� 




//dbuser��� �Լ� 
int addUser(char id[], char password[], struct user** firstUser, struct user** lastUser, struct user** tmpUser);   //���̵�� �н����带 �ް� ������ �����ϴ� �Լ�
void getUser(struct user** firstUser, struct user** lastUser, struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);   // ������ �� ȸ�� ���� ���Ͽ��� �ҷ����� �Լ� 
int deleteUser(char id[], char password[], struct user** firstUser, struct user** lastUser, struct user** tmpUser); // ����� ���� �Լ� 
int login(char id[], char passwd[], struct user** firstUser, struct user** selectedUser, struct user** tmpUser, struct sockS* sockTmp);   // �α����ϴ� �Լ� 
int signUp(struct user** firstUser, struct user** lastUser, struct user** tmpUser, struct sockS* sockTmp);   // ȸ������ �Լ� 
int signOut(struct user** firstUser, struct user** lastUser, struct user** tmpUser, struct sockS* sockTmp);   //ȸ��Ż�� �Լ� 
int logOut(struct user** selectedUser);  //�α׾ƿ� �Լ� 
//dbuser��� �Լ� 




//dbdb ��� �Լ� 
int createDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb); // ��� ���� �Լ�
int dropDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb);   // ��� �����Լ�
int useDb(struct dividedMsg** order, struct db** firstDb, struct db** tmpDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);// ��� ��� �Լ� 
int showDb2(struct dividedMsg** order, struct db** firstDb, struct db** tmpDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);// ��� show �Լ� 
void getDb(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);  //�������� ��� ������ �������� �Լ� 
//dbdb ��� �Լ� 


//dbTable��� �Լ� 
int createTable(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** lastField, struct field** tmpField);//CREATE TABLE ���̺��̸� (�ʵ��̸�1 Ÿ���̸�, �ʵ��̸�2 Ÿ���̸�2 .....);
int dropTable(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField); //drop table ���̤����̸�
void getTable(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);// ���̺��� �������� �Լ� 
//dbTable��� �Լ� 



//dbfree ��� �Լ� 
void freeField(struct field** freeField);
void freeMsg(struct dividedMsg** freeD);
void freeData(struct data** freeF);
void freeWhereCondition(struct whereCondition** freeW);
//dbfree ��� �Լ� 



//dbTuple ��� �Լ� 
int insertTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);   //insert��ɾ� 
int deleteTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);   //delete ��ɾ�
int updateTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData); //update��� ���� 
int selectTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData, struct sockS* sockTmp); //select��� ���� 
void getTuple(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** getData); //  Ʃ���� �������� �Լ� 
//dbTuple ��� �Լ� 





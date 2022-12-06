



//dbshow 헤더 함수 
void showMain(char mMsg[], struct sockS* sockTmp);   // 메인화면 출력 함수 
void showDb(char mMsg[], char loginId[], char dbName[], struct sockS* sockTmp); // 디비 화면 출력 함수 
void showLogin(char mMsg[], char loginId[], struct sockS* sockTmp);   // 로그인함수 출력 함수 
//dbshow 헤더 함수 

//dbfile헤더 함수 
int rmdirs(char* path, int force); // 디렉터리 안에있는 폴더/파일 전부 삭제하는 함수 
char getch();   // 키만 누르면 입력되는 함수 
void substring(char msg[], int start, int end);  //substriong 함수 
int openCheck(char msg[], char tmp[], char tmp2[]); // 시작괄호 체크하는 함수 
int atoi2(char msg[]); // 문자로 이루어진 숫자를 정수형으로 바꾸는 함수 
float atof2(char msg[]); //문자로 이루저진 실수를 실수로 바꾸는 함수 
int findMsg(char msg[], char findMsg[], char findMsg2[], int start); // 문자를 찾고 위치를 리턴하는 함수 
int findCondition(char msg[], struct whereCondition** where, struct whereCondition** whereTmp, struct whereCondition** whereEnd, int start); // where절의 조건을 찾는 함수 
int findTable(struct table** firstTable, struct table** lastTable, struct table** tmpTable, char tableName[]); //테이블을 찾는 함수 
int conditionCheck(struct whereCondition** where, struct whereCondition** whereTmp, struct tuple** tmpTuple); //조건이 맞는 튜플인지 체크하는 함수 
int findUpdate(char msg[], struct whereCondition** change, struct whereCondition** changeTmp, struct whereCondition** changeEnd, int start); //update조건을 가져오는 함수 
int findSelect(char msg[], struct whereCondition** select, struct whereCondition** selectTmp, struct whereCondition** selectEnd, int start); //select조건을 가져오는 함수 
int findStar(char msg[]); //*을 찾는 함수 
//dbfile헤더 함수 


//dborder 헤더 함수 
void divide(char msg[], char msgTmp[], struct dividedMsg** dorderF, struct dividedMsg** dorderL, struct dividedMsg** dorderT);  // 명령을 받아서 명령을 공백으로 나누는 함수 
int userOrder(struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct sockS* sockTmp);  // 로그인 상태에서 데이터베이스 명령어 사용 함수   CREATE , DROP   ->database 
int dbOrder(struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** lastField, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData, struct sockS* sockTmp); // db명령 함수 
//dborder 헤더 함수 




//dbuser헤더 함수 
int addUser(char id[], char password[], struct user** firstUser, struct user** lastUser, struct user** tmpUser);   //아이디와 패스워드를 받고 계정을 생성하는 함수
void getUser(struct user** firstUser, struct user** lastUser, struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);   // 시작할 때 회원 정보 파일에서 불러오는 함수 
int deleteUser(char id[], char password[], struct user** firstUser, struct user** lastUser, struct user** tmpUser); // 사용자 삭제 함수 
int login(char id[], char passwd[], struct user** firstUser, struct user** selectedUser, struct user** tmpUser, struct sockS* sockTmp);   // 로그인하는 함수 
int signUp(struct user** firstUser, struct user** lastUser, struct user** tmpUser, struct sockS* sockTmp);   // 회원가입 함수 
int signOut(struct user** firstUser, struct user** lastUser, struct user** tmpUser, struct sockS* sockTmp);   //회원탈퇴 함수 
int logOut(struct user** selectedUser);  //로그아웃 함수 
//dbuser헤더 함수 




//dbdb 헤더 함수 
int createDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb); // 디비 생성 함수
int dropDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb);   // 디비 삭제함수
int useDb(struct dividedMsg** order, struct db** firstDb, struct db** tmpDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);// 디비 사용 함수 
int showDb2(struct dividedMsg** order, struct db** firstDb, struct db** tmpDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);// 디비 show 함수 
void getDb(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);  //유저들의 디비 정보를 가져오는 함수 
//dbdb 헤더 함수 


//dbTable헤더 함수 
int createTable(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** lastField, struct field** tmpField);//CREATE TABLE 테이블이름 (필드이름1 타입이름, 필드이릅2 타입이름2 .....);
int dropTable(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField); //drop table 테이ㅁ블이름
void getTable(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);// 테이블을 가져오는 함수 
//dbTable헤더 함수 



//dbfree 헤더 함수 
void freeField(struct field** freeField);
void freeMsg(struct dividedMsg** freeD);
void freeData(struct data** freeF);
void freeWhereCondition(struct whereCondition** freeW);
//dbfree 헤더 함수 



//dbTuple 헤더 함수 
int insertTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);   //insert명령어 
int deleteTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData);   //delete 명령어
int updateTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData); //update명령 실행 
int selectTuple(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** tmpData, struct sockS* sockTmp); //select명령 실행 
void getTuple(struct user** tmpUser, struct db** tmpDb, struct table** tmpTable, struct field** tmpField, struct field** tmpField2, struct tuple** tmpTuple, struct data** getData); //  튜플을 가져오는 함수 
//dbTuple 헤더 함수 





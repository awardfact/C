



//dbshow 헤더 함수 
void showMain();   // 메인화면 출력 함수 
void showDb(char mMsg[], char loginId[], char dbName[]); // 디비 화면 출력 함수 
void showLogin();   // 로그인함수 출력 함수 
//dbshow 헤더 함수 

//dbfile헤더 함수 
int rmdirs(char* path, int force); // 디렉터리 안에있는 폴더/파일 전부 삭제하는 함수 
char getch();   // 키만 누르면 입력되는 함수 
void substring(char msg[], int start, int end);  //substriong 함수 
int openCheck(char msg[], char tmp[], char tmp2[]); // 시작괄호 체크하는 함수 
//dbfile헤더 함수 


//dborder 헤더 함수 
char* divide(char msg[], struct dividedMsg** dorderF, struct dividedMsg** dorderL, struct dividedMsg** dorderT);  // 명령을 받아서 명령을 공백으로 나누는 함수 
int userOrder(struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);  // 로그인 상태에서 데이터베이스 명령어 사용 함수   CREATE , DROP   ->database 
int dbOrder(struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable); // db명령 함수 
//dborder 헤더 함수 




//dbuser헤더 함수 
int addUser(char id[], char password[], struct user** firstUser, struct user** lastUser, struct user** tmpUser);   //아이디와 패스워드를 받고 계정을 생성하는 함수
void getUser();   // 시작할 때 회원 정보 파일에서 불러오는 함수 
int deleteUser(char id[], char password[], struct user** firstUser, struct user** lastUser, struct user** tmpUser); // 사용자 삭제 함수 
int login(char id[], char passwd[], struct user** firstUser, struct user** selectedUser, struct user** tmpUser);   // 로그인하는 함수 
int signUp(struct user** firstUser, struct user** lastUser, struct user** tmpUser);   // 회원가입 함수 
int signOut(struct user** firstUser, struct user** lastUser, struct user** tmpUser);   //회원탈퇴 함수 
int logOut(struct user** selectedUser);  //로그아웃 함수 
//dbuser헤더 함수 




//dbdb 헤더 함수 
int createDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb); // 디비 생성 함수
int dropDb(struct dividedMsg** order, struct user** selectedUser, struct db** tmpDb, struct db** firstDb, struct db** lastDb);   // 디비 삭제함수
int useDb(struct dividedMsg** order, struct db** firstDb, struct db** tmpDb, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);// 디비 사용 함수 
void getDb(struct user** tmpUser);  //유저들의 디비 정보를 가져오는 함수 
//dbdb 헤더 함수 


//dbTable헤더 함수 
int createTable(struct dividedMsg** order, char afterOrder[], struct user** selectedUser, struct db** selectedDb, struct table** firstTable, struct table** lastTable, struct table** tmpTable);//CREATE TABLE 테이블이름 (필드이름1 타입이름, 필드이릅2 타입이름2 .....);
//dbTable헤더 함수 












int deleteTuple(struct tuple* deleteTuple); // 튜플 삭제 함수 

int dropTable(struct dividedMsg*);  //DROP TABLE 테이블이름;
int insertTable(struct dividedMsg*);// INSERT INTO 테이블이름(필드이름1, 필드이름2, 필드이름3 ...)VALUES(데이터값1,데이터값2,데이터값3....)  OR  INSERT INTO 테이블이름VALUES(데이터값1, 데이터값2,데이터값3.....)
int updateTable(struct dividedMsg*); //UPDATE 테이블이름 SET 필드이름1=데이터값1, 필드이름2=데이터값2..... where 조건
int deleteTable(struct dividedMsg*); //DELETE FROM 테이블이름 where 조건
int selectTable(struct dividedMsg*); //SELECT 필드이름1 , 필드이름2 .... from 테이블이름 where 조건  group by 필드


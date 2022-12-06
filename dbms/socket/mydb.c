#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
//#include <mongoc.h>
//#include  "cJSON/cJSON.h"
//#include <bson.h>
#include <pthread.h>
//#include <winsock.h>
#include <errno.h>

#include <stdio.h>
#include <termios.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "dbstruct.h"
#include "dbfunction.h"
#include "dbfile.h"
#include "dbshow.h"
#include "dbuser.h"
#include "dborder.h"
#include "dbdb.h"
#include "dbtable.h"
#include "dbfree.h"
#include "dbtuple.h"
#include <time.h>

//#pragma comment(lib, "ws2_32.lib")




// 현재 연결중인 


//처음 가리키는 포인터 
struct sockS* firstClient = NULL;

// 이전 현재 이후 가리키는 포인터 
struct sockS* beforeClient = NULL;
struct sockS* currentClient = NULL;
struct sockS* nextClient = NULL;
struct sockS* clientTmp = NULL;




time_t t;

char* str;


//클라이언트 추가 함수
void client_add(int so, int sock);
//클라이언트 삭제 함수 
void client_del(struct sockS* client);
//클라이언트에게 메시지 전송 함수 
void write_to_client(char* buf, char* name);

void* connect_client(void* soTmp);
/*
소켓 연결
1. 소켓 생성
2. bind
3. listen
4. accept
5. send / recv
6. close

*/
int main(int argc, char* argv[])
{


    //소켓 주소 
    struct sockaddr_in address;

    int client_addr_size = sizeof(address);
    // 클라이언트 쓰레드 

    // 클라이언트 배열 번호 
    int client_i = 0;
    //클라이언트 소켓 번호 
    int cs = 0;
    int status;

    // 소켓 주소 크기
    int addresslen = sizeof(address);
    //서버  소켓 번호 
    int sSock;
    int cSock;



    //ipv4
    address.sin_family = AF_INET;
    // 서버 아이피 
    address.sin_addr.s_addr = inet_addr(argv[1]);
    //포트번호 
    address.sin_port = htons(atoi(argv[2]));

    int result;

    // 1.소켓을 만든다 
    if ((sSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket fail.");
    }

    // 2. bind
    if (bind(sSock, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("bind fail  %d.\n", errno);
    }

    // 3.listen 
    if (listen(sSock, 5) < 0) {
        printf("listen fail.");
    }

   // printf("socket  : %d\n ", sSock);
    while (1) {
        // 4. accept
        if ((cSock = accept(sSock, (struct sockaddr*)&address, &client_addr_size)) < 0) {
            printf("accept");
        }
        else {

            // 클라이언트가 연결하면 쓰레드를 만들고 송수신하는  쓰레드 함수 실행 
            client_add(cSock, sSock);
            //cs = pthread_create(&client[client_i], NULL, connect_client,&so[client_i]);

           // printf("sSock : %d   cSock : %d     \n", currentClient->sSock, currentClient->cSock);
            cs = pthread_create(&currentClient->client, NULL, connect_client, currentClient);

        }

    }


    /*

        while (1) {


        result = read(so, buf, 1024);
        if (result < 1) {

            printf("fail recv\n");
        }
        printf("read Data :  %s\n", buf);

    }
    */


    close(sSock);

    return 0;
}

//클라이언트 추가 함수 클라이언트는 연결 리스트로 관리 
void client_add(int cSock, int sSock) {

    // printf("sScok : %d   cScok :  %d\n", sSock, cSock);
     //메모리 할당 한 후 클라이언트 소켓, 서버 소켓을 클라이언트 구조체에 넣는다 
    clientTmp = (struct sockS*)malloc(sizeof(struct sockS));
    clientTmp->sSock = sSock;
    clientTmp->cSock = cSock;

    // 생성한 클라이언트의 다음은 없어서 null
    clientTmp->next = NULL;
    clientTmp->before = NULL;

    //현재 클라이언트가 있으면 생성 클라이언트 전 클라이언트는 생성한 클라이언트가 된다 
    if (currentClient != NULL) {
        clientTmp->before = currentClient;
        currentClient->next = clientTmp;

        beforeClient = currentClient;
    }

    // 현재 클라이언트의 다음은 생성한 클라이언트

    printf("sScok : %d   cScok :  %d\n", clientTmp->sSock, clientTmp->cSock);

    //처음 클라이언트가 없으면 생성 클라이언트가 현재 클라이언트가 된다 
    if (firstClient == NULL) {
        firstClient = clientTmp;
    }

    //현재 클라이언트는 생성한 클라이언트 

    currentClient = clientTmp;


    return;

}

//클라이언트 삭제 함수 
void client_del(struct sockS* client) {

    //삭제 클라이언트가 현재 클라이언트고 이전 클라이언트가 있는 경우 
    if (client == currentClient && beforeClient != NULL) {
        // 현재 클라이언트는 이전 클라이언트가 된다 (다음 클라이언트는 어차피 null)
        currentClient = beforeClient;

        //전(현재)클라이언트의 이전 클라이언트가 있으면 이전 클라이언트는 전(현재)클라이언트의 이전 클라이언트가 된다 
        if (currentClient->before != NULL) {
            beforeClient = currentClient->before;
        }
    }
    // 이전 클라이언트가 없으면 현재 클라이언트를 null로 바꾼다  
    else if (client == currentClient) {
        currentClient = NULL;
        firstClient = NULL;
    }

    // 삭제 클라이언트의 다음 클라이언트가 있는 경우 
    if (client->next != NULL) {
        // 삭제 클라이언트의 이전 클라이언트도 있으면 다음클라이언트의 이전은 삭제클라이언트의 이전 , 삭젴클라이언트 이전 클라이언트의 다음은 삭제 클라이언트 다음 클라이언트 
        if (client->before != NULL) {
            client->next->before = client->before;
            client->before->next = client->next;
        }
        // 삭제클라이언트 이전 클라이언트가 없으면 다음 클라이언트의 이전 클라이언트는 null이된다 
        else {
            client->next->before = NULL;
        }
    }
    // 삭제 클라이언트 다음 클라이언트가 없고이전 클라이언트가 있으면 이전 클라이언트의 다음 클라이언트는 null이 된다 
    else {
        if (client->before != NULL) {
            client->before->next = NULL;
        }
    }

    //클라이언트 소켓 닫기 
    close(client->cSock);
    //삭제 클라이언트 메모리 할당 해제 
    free(client);


    printf("client connect exit\n");
}

// 클라이언트들에게 메시지를 보내는 함수 
void write_to_client(char* buf, char* name) {

    int wresult;
    //처음 클라이언트를 가리키는 임시 클라이언트 변수 생성
    struct sockS* clientFunctionTmp = firstClient;
    //처음 클라이언트가 없으면 연결된 클라이언트가 없음을 출력 




    if (clientFunctionTmp == NULL) {

        printf("no connected Client\n");
    }
    else {
        //클라이언트가 있으면 클라이언트의 다음이 null이될때까지 클라이언트에게 메시지 전송 
        do {


            //printf("next : %d \n ", clientFunctionTmp->next);
            wresult = write(clientFunctionTmp->cSock, name, 1024);
            wresult = write(clientFunctionTmp->cSock, buf, 1024);
            if (wresult < 0) {
                printf("send fail\n");
            }
            clientFunctionTmp = clientFunctionTmp->next;
        } while (clientFunctionTmp != NULL);


    }

}


// 클라이언트와 송수신하는 쓰레드 함수 
void* connect_client(void* sockSTmp) {

    // 인수로 받은 값을 구조체로 변환

    printf("0\n");
    struct sockS* sockTmp = (struct sockS*)sockSTmp;
    // struct sockS* sockTmp = NULL;
    // sockTmp->sSock =sockSTmp->sSock;
    // sockTmp->cSock = sockSTmp->cSock;
    // sockTmp->client = sockSTmp->client;
     // 서버 소켓 번호와 클라이언트 소켓 번호를 변수에 저장 

    int userCount;

    // 사용자 전역변수 
    struct user* tmpUser = NULL;  // 임시로 user를 다룰 때 사용하는 임시 user변수
    struct user* firstUser = NULL;  // 첫번쨰 user를 가리키는 user포인터
    struct user* lastUser = NULL;   // 현재 user를 가리키는 user포인터(가장 마지막에 생성된 user)
    struct user* selectedUser = NULL;   // 현재 접속중인 user를 가리키는 user 포인터 

    // 디비 전역변수 
    struct db* tmpDb = NULL; // 임시로 db를 다룰 때 사용하는 임시 db포인터변수
    struct db* firstDb = NULL;  // 현재 유저가 가지고 있는 첫번쨰 db  로그인중이 아니라면 null   user의 firstDb가 이것이 된다
    struct db* lastDb = NULL; // 현재 유저가 가지고 있는 마지막 db 로그인이 아니면 null user의 firstDb의 before가 있으면 그것이 되고    없다면 fristDb도 없으면 null
    struct db* selectedDb = NULL;   //현재 유저가 선택한 db

    //테이블 전역변수 
    struct table* tmpTable = NULL; // 임시로 테이블을 다룰 때 사용하는 테이블포인터변수  
    struct table* firstTable = NULL; //  DB를 선택했을 때 DB의 첫번째 테이블 
    struct table* lastTable = NULL;  // DB를 선택했을 때 DB의 마지막 테이블 
    struct talbe* selectedTable = NULL;   //  현재 선택중인 테이블 


    //튜플 전역변수 
    struct tuple* tmpTuple = NULL;
    struct tuple* firstTuple = NULL;
    struct tuple* lastTuple = NULL;


    //필드 전역변수 
    struct field* tmpField = NULL;  // 임시로 필드를 다룰 때 사용하는 필드
    struct field* tmpField2 = NULL;  // 임시로 필드를 다룰 때 사용하는 필드
    struct field* firstField = NULL;  // 선택한 테이블의 첫번째 필드
    struct field* lastField = NULL;  // 선택한 테이블의 마지막 필드 
    struct field* selectedField = NULL; // 현재 선택중인 필드 (사용할지는 모르겠음)



    //데이터 전역변수 
    struct data* tmpData = NULL;  // 임시로 데이터를 다룰 때 사용하는 데이터
    struct data* firstData = NULL; // 현재 튜플의 첫 데이터
    struct data* lastData = NULL; // 현재 튜플의 마지막 데이터

    char id[1024];
    char passwd[1024];
    char passwdRe[1024];
    char mainMsg[1024];
    struct user* headerTest;

    int i = 0;
    char c;

    int monitor = 0;
    int monitor2 = 0;
    char order[1024];
    char getKey[1024];
    int exit = 0;
    int tmp = 0;

    //printf("sSock : %d   cSock : %d     \n", currentClient->sSock, currentClient->cSock);
    //printf("sSock : %d  and cSock  : %d  before :  %d    after  :  %d \n", sockTmp->(int)sSock, sockTmp->(int)cSock , sockTmp->(int)before, sockTmp->(int)next);
    //전송 수신 성공했는지 여부를 받는 변수 선언 
    int result = 0;
    int wresult = 0;

    char* str;
    char name[1024] = "client";
    //메시지를 담는 변수 선언 
    char buf[1024] = "input your name : \0";



    getKey[0] = '\0';
    //클라이언트에 환영 메시지를 보냄
    wresult = write(sockTmp->cSock, "hello Server", 1024);

    getUser(&firstUser, &lastUser, &tmpUser, &tmpDb, &tmpTable, &tmpField, &tmpField2, &tmpTuple, &tmpData);
    while (1) {




        if (monitor2 != 0) {


            result = read(sockTmp->cSock, getKey, 1024);
            monitor2 = 0;


        }


        // monitor가 0이면 메인화면 1이면 로그인화면 2면 디비화면을 출력한다 
        switch (monitor) {

            //메인화면 
        case 0:
            //getKEy가 Null이면 입력을 메인화면에서 입력을 받는다 
            if (getKey[0] == '\0') {

                showMain(mainMsg , sockTmp);
                // 클라이언트에게 메시지를 받음 
                result = read(sockTmp->cSock, getKey, 1024);

                // 메시지 받는게 실패하면 해당 클라이언트 소켓 종료 
                if (result < 1) {

                    client_del((struct sockS*)sockSTmp);
                    //printf("connect fail ");
                    break;
                }



            }

            //printf("%d \n ", &sockTmp);
            // 0을 입력했으면 프로그램 종료 
            if (getKey[0] == 48) {
                exit = 1;
            }
            //1을 입력했으면 로그인 화면으로 이동
            else if (getKey[0] == 49) {
                if (login(id, passwd, &firstUser, &selectedUser, &tmpUser ,sockTmp) == 1) {
                    firstDb = selectedUser->firstDB;
                    lastDb = selectedUser->currentDB;
                    monitor = 1;
                    strcpy(mainMsg, "login success.\0");
                    getKey[0] = '\0';
                }
                else {
                    strcpy(mainMsg, "login fail.\0");
                    getKey[0] = '\0';

                }

            }
            //2를 입력했으면 회원가입 화면으로 이동 
            else if (getKey[0] == 50) {
                // 회원가입 성공하면 메인페이지로 이동하고 성공 출력 
                if (signUp(&firstUser, &lastUser, &tmpUser , sockTmp) == 1) {
                    strcpy(mainMsg, "sign up success!\0");
                    getKey[0] = '\0';
                }
                // 회원가입 실패하면 메인페이지로 이동하고 실패 출력 
                else {
                    strcpy(mainMsg, "sign up fail.\0");
                    getKey[0] = '\0';
                }
                //3을 입력하면 회원 탈퇴 화면으로 이동
            }
            else if (getKey[0] == 51) {
                // 회원탈퇴 성공하면 메인페이지로 이동하고 성공 출력 
                if (signOut(&firstUser, &lastUser, &tmpUser , sockTmp ) == 1) {
                    strcpy(mainMsg, "sign out success!\0");
                    getKey[0] = '\0';


                }
                // 회원탈퇴 실패하면 메인페이지로 이동하고 성공 출력 
                else {
                    strcpy(mainMsg, "sign out fail.\0");
                    getKey[0] = '\0';
                }

            }

            else {
                getKey[0] = '\0';

            }


            break;
            //로그인 화면 
        case 1:
            //getKEy가 Null이면 입력을 메인화면에서 입력을 받는다 
            if (getKey[0] == '\0') {
                //	printf("%s \n", selectedUser->id);
                showLogin(mainMsg, selectedUser->id , sockTmp);
                result = read(sockTmp->cSock, getKey, 1024);
                if (result < 1) {
                    client_del((struct sockS*)sockSTmp);
                    break;
                }


            }

            // 0을 입력했으면 프로그램 종료 
            if (getKey[0] == 48) {
                exit = 1;
            }
            else if (getKey[0] == 49) {
                tmp = userOrder(&selectedUser, &tmpDb, &firstDb, &lastDb, &selectedDb, &firstTable, &lastTable, &tmpTable , sockTmp);
                if (tmp == 0) {
                    strcpy(mainMsg, "order process fail \0");

                }
                else if (tmp == 1) {
                    strcpy(mainMsg, "create db success! \0");
                }
                else if (tmp == 2) {
                    strcpy(mainMsg, "drop db success! \0");
                }
                else if (tmp == 3) {
                    strcpy(mainMsg, "use db success! \0");
                    monitor = 2;

                }
                else if (tmp == 4) {
                    //   printf("Press any key to return. \n");
                    //   getKey[0] = getch();
                    monitor2 = 1;
                }
                getKey[0] = '\0';

            }
            // 2를 입력하면 로그아웃 
            else if (getKey[0] == 50) {
                selectedUser = NULL;
                monitor = 0;
                strcpy(mainMsg, "logout success!\0");
                getKey[0] = '\0';

            }
            else {
                getKey[0] = '\0';

            }



            break;
        case 2:
            if (getKey[0] == '\0') {
                //showDb(mainMsg, selectedUser->id, selectedDb->name , sockTmp);
                result = read(sockTmp->cSock, getKey, 1024);
                if (result < 1) {
                    client_del((struct sockS*)sockSTmp);
                    break;
                }


            }


            if (getKey[0] == 48) {
                exit;
            }
            else if (getKey[0] == 49) {

                tmp = dbOrder(&selectedUser, &selectedDb, &firstTable, &lastTable, &tmpTable, &lastField, &tmpField, &tmpField2, &tmpTuple, &tmpData , sockTmp );

                //	printf("tmp : :  %d\n", tmp);
                if (tmp == 0) {
                    strcpy(mainMsg, "order process fail \0");

                }
                else if (tmp == 1) {
                    strcpy(mainMsg, "create table success! \0");
                }
                else if (tmp == 2) {
                    strcpy(mainMsg, "drop table success! \0");
                }
                else if (tmp == 3) {
                    strcpy(mainMsg, "insert tuple success! \0");


                }
                else if (tmp == 4) {
                    strcpy(mainMsg, "delete tuple success! \0");
                }
                else if (tmp == 5) {
                    strcpy(mainMsg, "upadate tuple success! \0");

                }
                else if (tmp == 6) {
                    //select 
                //    printf("Press any key to return. \n");
                  //  result = read(sockTmp->cSock, getKey, 1024);
                  //  monitor2 = 1;
                    //getKey = '\0';
                    strcpy(mainMsg, "select tuple success! \0");
                }
                getKey[0] = '\0';

            }
            else if (getKey[0] == 50) {
                selectedDb = NULL;
                monitor = 1;
                strcpy(mainMsg, "logout success!\0");
                getKey[0] = '\0';
            }
            else {
                getKey[0] = '\0';
            }

            printf("tmpOut\n");
            break;

        }



        if (exit == 1) {
            break;
        }








        //printf("read Data : %s \n", buf);
        //// 클라이언트들에게 메시지를 보내고 몽고디비에 데이터를 넣음 넣는 데이터는 내용, 시간, 이름
        //write_to_client(buf, name);
        //bson = bson_new();
        //t = time(NULL);
        //BSON_APPEND_UTF8(bson, "chat", buf);
        //BSON_APPEND_UTF8(bson, "name", name);
        //BSON_APPEND_UTF8(bson, "time", ctime(&t));

        //if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, bson, NULL, &error)) {
        //    fprintf(stderr, "%s\n", error.message);
        //}
        //bson_free(bson);
        ////받은 메시지를 클라이언트에게 보냄 
        ////wresult = send(cSock, buf, 1024, 1);
        //if (wresult < 1) {
        //    client_del((struct sockS*)sockSTmp);
        //    //printf("connect fail ");
        //    break;
        //}

    }


}
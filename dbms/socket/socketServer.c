#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

//#include <bson.h>
#include <pthread.h>
//#include <winsock.h>
#include <errno.h>


#include <time.h>
//#pragma comment(lib, "ws2_32.lib")

struct chatt {
    char chat[1024];
    char name[1024];
    char time[1024];

};



// 쓰레드 인수로 보낼 구조체 
struct sockS {
    int sSock;
    int cSock;
    pthread_t client;
    struct sockS* before;
    struct sockS* next;
};


// 현재 연결중인 


//처음 가리키는 포인터 
struct sockS* firstClient = NULL;

// 이전 현재 이후 가리키는 포인터 
struct sockS* beforeClient = NULL;
struct sockS* currentClient = NULL;
struct sockS* nextClient = NULL;
struct sockS* clientTmp = NULL;
struct chatt chatMsg[20];
int chatCnt = 0;
int sock2;
int dbGet = 0;
time_t t;


//클라이언트 추가 함수
void client_add(int so, int sock);
//클라이언트 삭제 함수 
void client_del(struct sockS* client);
//클라이언트에게 메시지 전송 함수 
void write_to_client(char* buf, char* name);

void* connect_client(void* soTmp);
void* write_msg(void* soTmp);
void* read_msg(void* soTmp);

void write_db(char name[], char msg[], char time[]);
void get_db();
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
    struct sockaddr_in address2;
    int client_addr_size = sizeof(address);
    int client_addr_size2 = sizeof(address2);
    // 클라이언트 쓰레드 

    // 클라이언트 배열 번호 
    int client_i = 0;
    //클라이언트 소켓 번호 
    int cs = 0;
    int status;

    // 소켓 주소 크기
    int addresslen = sizeof(address);
    int addresslen2 = sizeof(address2);
    //서버  소켓 번호 
    int sSock;
    int cSock;


    // 클라이언트 쓰레드 
    pthread_t client[2];

    //송 /수신 쓰레드 번호
    int readT;
    int writeT;

    int result;
    int result2;
    int optval = 1;


    //ipv4
    address.sin_family = AF_INET;
    // 서버 아이피 
    address.sin_addr.s_addr = inet_addr(argv[1]);
    //포트번호 
    address.sin_port = htons(atoi(argv[2]));



    //먼저 디비 서버와 연결을 한다 
    //ipv4
    address2.sin_family = AF_INET;
    // 서버 아이피 
    address2.sin_addr.s_addr = inet_addr(argv[1]);
    //포트번호 
    address2.sin_port = htons(atoi(argv[3]));

    //1. 소켓 생성
    if ((sock2 = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket fail.");
    }

    // 2. 소켓 연결 
    if (connect(sock2, (struct sockaddr*)&address2, sizeof(address2)) < 0) {
        printf("Connect fail\n");
    }

    //디비 서버와 송수신하는 쓰레드 생성 
    //연결한 소켓 번호를 송수신 쓰레드에 넣고 쓰레드 함수 실행 
    readT = pthread_create(&client[0], NULL, write_msg, &sock2);
    writeT = pthread_create(&client[1], NULL, read_msg, &sock2);


    





    // 1.소켓을 만든다 
    if ((sSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket fail.");
    }
    printf("2222");
    // 2. bind
    if (bind(sSock, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("bind fail  %d.\n", errno);
    }
    printf("3333");
    // 3.listen 
    if (listen(sSock, 5) < 0) {
        printf("listen fail.");
    }

    setsockopt(sSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    printf("4444");
    printf("socket  : %d\n ", sSock);
    while (1) {
        // 4. accept
        if ((cSock = accept(sSock, (struct sockaddr*)&address, &client_addr_size)) < 0) {
            printf("accept");
        }
        else {
            setsockopt(cSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
            printf("5555");
            // 클라이언트가 연결하면 쓰레드를 만들고 송수신하는  쓰레드 함수 실행 
            client_add(cSock, sSock);
            //cs = pthread_create(&client[client_i], NULL, connect_client,&so[client_i]);
            printf("6666");
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
    //pthread_join(currentClient->client, NULL);

    pthread_join(client[0], NULL);
    pthread_join(client[1], NULL);
    close(sSock);
    close(sock2);

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


    printf("입장  sdasdasdasdasdasdasd\n");
    // 인수로 받은 값을 구조체로 변환

    struct sockS* sockTmp = (struct sockS*)sockSTmp;
    // struct sockS* sockTmp = NULL;
    // sockTmp->sSock =sockSTmp->sSock;
    // sockTmp->cSock = sockSTmp->cSock;
    // sockTmp->client = sockSTmp->client;
     // 서버 소켓 번호와 클라이언트 소켓 번호를 변수에 저장 


    //printf("sSock : %d   cSock : %d     \n", currentClient->sSock, currentClient->cSock);
    //printf("sSock : %d  and cSock  : %d  before :  %d    after  :  %d \n", sockTmp->(int)sSock, sockTmp->(int)cSock , sockTmp->(int)before, sockTmp->(int)next);
    //전송 수신 성공했는지 여부를 받는 변수 선언 
    int result = 0;
    int wresult = 0;
    int i = 0;
    char* str;
    char name[100] = "client";
    //메시지를 담는 변수 선언 
    char buf[1024] = "hello Server";

    
    //클라이언트에 환영 메시지를 보냄
    wresult = write(sockTmp->cSock, buf, 1024);
    //    wresult = write(sockTmp->cSock, buf, 1024);
    //이름 입력하라고 클라이언트에게 보냄 
    strcpy(buf, "input your name : \0");

    wresult = write(sockTmp->cSock, buf, 1024);
    //write(sock, buf, 1024);
    //클라이언트의 이름을 받아옴 
    result = read(sockTmp->cSock, name, 1024);
    printf("%s's join \n", name);


  //  printf("%s's 22222222222222222222222222222222222222 \n", name);


    //디비 서버에서 최신 문자 10개를 받아오게 설정하는 함수 
    get_db();
    dbGet = 1;

    //받아오는데 시간이 조금 필요해서 0.5초정도 슬립 
    usleep(50000);

 //   printf("chatCnt : %d \n ", chatCnt);
//

    //역순으로 출력 
    for (i = chatCnt -1; i >=0; i--) {

        printf("name : %s   chat : %s \n ", chatMsg[i].name, chatMsg[i].chat);
        strcpy(buf, chatMsg[i].name);
        wresult = write(sockTmp->cSock, buf, 1024);
        strcpy(buf, chatMsg[i].chat);
        wresult = write(sockTmp->cSock, buf, 1024);

        //wresult = write(sockTmp->cSock, chatMsg[i].chat, 1024);
        //wresult = write(sockTmp->cSock, chatMsg[i].name, 1024);
   //     printf("writeEnd \n ", chatMsg[i].name, chatMsg[i].chat);

    }









   // printf("ccccccccc\n");
    // 변수 초기화할떄 \0으로 초기화되는거 같아서 자동으로 빈 데이터를 보내는거 같아서 그것을 읽는 코드 추가 
   // result = read(sockTmp->cSock, buf, 1024);

    while (1) {
       // printf("readWhilew \n ", chatCnt);
        // 클라이언트에게 메시지를 받음 
        result = read(sockTmp->cSock, buf, 1024);
        printf("read Data : %s \n", buf);
        // 메시지 받는게 실패하면 해당 클라이언트 소켓 종료 
        if (result < 1) {
            client_del((struct sockS*)sockSTmp);
            //printf("connect fail ");
            break;
        }


       //받아온 데이터를 클라이언트들에게 보내고 디비 서버에 insert문 실행 
        t = time(NULL);
        write_to_client(buf, name);
        write_db(name, buf, ctime(&t));


        //받은 메시지를 클라이언트에게 보냄 
      //  wresult = send(cSock, buf, 1024, 1);


    }


}





// 디비 서버 수신 쓰레드 
void* read_msg(void* soTmp) {

    //소켓 번호 변수 생성
    int* so = (int*)soTmp;


    int result = 0;
    char buf[1024];
    int type = 0;
    while (1) {

        //클라이언트가 접속해서 최신 데이터 10개를 받는 상태 
        if (dbGet == 1) {

            // 서버에게서 데이터 수신 
            result = read(*so, buf, 1024);

            if (strcmp(buf, "!end!") == 0) {
                dbGet = 1;
                // break;
            }
            printf("read Data : %s \n", buf);
            //데이터를 받아서 배열에 넣는다 
            if (type == 0) {
                strcpy(chatMsg[chatCnt].chat, buf);
                type = 1;
            }
            else if (type == 1) {
                strcpy(chatMsg[chatCnt].name, buf);
                type =0;
                chatCnt++;
            }




        }
        // 기본 상태 
        else {



            // 서버에게서 데이터 수신 
            result = read(*so, buf, 1024);


            if (dbGet == 1) {

                // 서버에게서 데이터 수신 
                result = read(*so, buf, 1024);

                if (strcmp(buf, "!end!") == 0) {
                    dbGet = 1;
                    // break;
                }
                printf("read Data : %s \n", buf);

                if (type == 0) {
                    strcpy(chatMsg[chatCnt].chat, buf);
                    type = 1;
                }
                else if (type == 1) {
                    strcpy(chatMsg[chatCnt].name, buf);
                    type = 0;
                    chatCnt++;
                }



            }


            //수신이 실패하면 소켓 종료 
            if (result < 1) {
                printf("connect fail ");
                close(*so);
                break;
            }

            //수신한 데이터 출력 
            printf("%s : ", buf);



        }


    }


}

//디비 서버에 메시지를 보내는 함수 
void* write_msg(void* soTmp) {



    //소켓 번호 변수 생성
    int* so = (int*)soTmp;

    int result = 0;

    char buf[1024];
    int i = 0;



    while (1) {
        //printf("%d socket send : ", sock);
    //rewind(stdin);
        //송신한 데이터를 사용자에게 입력받음 
//        fgets(buf,1024,stdin);

        i = 0;
        //scanf로 데이터를 보내면 공백이 문제가 되고 scanf에서 공백을 포함하여 데이터를 넣으면 무한루프 나와서 getchar를 통해 데이터를 받아옴 
       //디비 명령어를 받아서 디비에 보냄 
        while (buf[i] = getchar()) {
            if (buf[i] == '\n') {
                break;
            }
            else {
                i++;
            }
        }
        buf[i] = '\0';

        //        scanf("%[^\n]s", buf);
                // 서버에게서 데이터 송신 
        result = write(*so, buf, strlen(buf) + 1);

        //rewind(stdin);

        if (result < 1) {
            printf("connect fail ");
            close(*so);
            break;
        }
        //printf("write result = %d \n", result);
        // printf("%s", buf);
         //send(sock, buf, strlen(buf));
    }





}




///디비 서버에  데이터 입력함수  
void write_db(char name[], char msg[], char time[]) {


   // printf("name :  %s \n", name);
   // printf("msg :  %s \n", msg);
  //  printf("time :  %s \n", time);
    //소켓 번호 변수 생성
   // int* so = (int*)soTmp;

    int result = 0;

    char buf[1024];
    int i = 0;
    // 1번을 입력해야 명령어 입력창이 떠서 먼저 1을 입력 
    result = write(sock2, "1", 1024);
    if (result < 1) {
        printf("connect fail ");
        close(sock2);
        return;
    }
    // 받아온 데이터를 이용해서 insert문을 만들어서 디비 서버에 보낸다 
    strcpy(buf, "INSERT INTO chat VALUES(");
    strcat(buf, msg);
    strcat(buf, ",");
    strcat(buf, name);

    strcat(buf, ")");
    result = write(sock2, buf, 1024);


    if (result < 1) {
        printf("connect fail ");
        close(sock2);
        return;
    }




}


///디비 서버에  입력 함수 

void get_db() {



    printf("1\n");
    //소켓 번호 변수 생성
    //int* so = (int*)soTmp;
    char tmpMsg[1024];
    printf("getDb 1 \n");
    int result = 0;

    char buf[1024];
    int i = 0;
    int j = 0;
    result = write(sock2, "1", 1024);
    if (result < 1) {
        printf("connect fail ");
        close(sock2);
        return;
    }
    result = write(sock2, "SELECT * from chat", 1024);
    printf("getDb 2 \n");
    chatCnt = 0;

    dbGet = 1;

    return;

}
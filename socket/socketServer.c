#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <mongoc.h>
#include  "cJSON/cJSON.h"
//#include <bson.h>
#include <pthread.h>
//#include <winsock.h>
#include <errno.h>


#include <time.h>
//#pragma comment(lib, "ws2_32.lib")




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



// 몽고디비 연결에 필요한 정보 선언 
mongoc_database_t* database;
mongoc_collection_t* collection;
bson_error_t error;
bson_t* command, * bson, * insert;
const char* uri_string = "mongodb://awardfact:aldksgkek12@ac-sog0qbo-shard-00-00.zo1c6o4.mongodb.net:27017,ac-sog0qbo-shard-00-01.zo1c6o4.mongodb.net:27017,ac-sog0qbo-shard-00-02.zo1c6o4.mongodb.net:27017/?ssl=true&replicaSet=atlas-4w3b9t-shard-0&authSource=admin&retryWrites=true&w=majority";
mongoc_uri_t* uri;
mongoc_client_t* client;

time_t t;

char* str;
bool retval;

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



    mongoc_init();
    //몽고디비 연결 
    uri = mongoc_uri_new(uri_string);
    if (!uri) {
        fprintf(stderr,
            "failed to parse URI: %s\n"
            "error message:       %s\n",
            uri_string,
            error.message);
        return EXIT_FAILURE;
    }

    client = mongoc_client_new_from_uri(uri);
    if (!client) {
        return EXIT_FAILURE;
    }
    mongoc_client_set_appname(client, "connect-example");

    /*
     * Get a handle on the database "db_name" and collection "coll_name"
     */
     //몽고디비 데이터베이스랑 테이블 가져옴 
    database = mongoc_client_get_database(client, "chatting");
    collection = mongoc_client_get_collection(client, "chatting", "chatlog");

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

    printf("socket  : %d\n ", sSock);
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

    //몽고디비 할당 해제 
    mongoc_collection_destroy(collection);
    mongoc_database_destroy(database);
    mongoc_uri_destroy(uri);
    mongoc_client_destroy(client);
    mongoc_cleanup();


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
    char buf[1024] = "input your name : \0";

    //클라이언트에 환영 메시지를 보냄
    wresult = write(sockTmp->cSock, "hello Server", 1024);
    //    wresult = write(sockTmp->cSock, buf, 1024);

    //이름 입력하라고 클라이언트에게 보냄 
    wresult = write(sockTmp->cSock, buf, 1024);
    //write(sock, buf, 1024);
    //클라이언트의 이름을 받아옴 
    result = read(sockTmp->cSock, name, 1024);
    printf("%s's join \n", name);

    //몽고디비에 들어가있는 데이터를 최신순으로 10개를 가져옴 
    const bson_t* doc;
    cJSON* json = NULL;
    cJSON* jsonItem = NULL;
    bson_t* filter, * opts;
    mongoc_cursor_t* cursor;
    filter = bson_new();
    opts = BCON_NEW("limit", BCON_INT32(10), "sort", "{", "time", BCON_INT32(-1), "}");


    cursor = mongoc_collection_find_with_opts(collection, filter, opts, NULL);
    //채팅 로그를 담을 배열 
    char dbname[10][1024] = { '\0', };
    char dbchat[10][1024] = { '\0', };


    // 최신순으로 데이터를 가져오면 가장 최신 데이터가 가장 위로 들어가기 때문에 바로 보내지 않고 배열에 데이터를 저장했다가 배열의 역순으로 데이터 전송
    while (mongoc_cursor_next(cursor, &doc)) {
        printf("cursor \n");
        //디비 데이터를 json으로 변경 
        str = bson_as_canonical_extended_json(doc, NULL);
        // json 파싱
        json = cJSON_Parse(str);
        // name데이터를 가져옴 
        jsonItem = cJSON_GetObjectItem(json, "name");
        //name 값을 배열에 넣음
        strcpy(dbname[i], jsonItem->valuestring);
        // chat데이터를 가져옴 
        jsonItem = cJSON_GetObjectItem(json, "chat");
        //chat데이터를 배열에 넣음
        strcpy(dbchat[i], jsonItem->valuestring);
        i++;
        // wresult = write(sockTmp->cSock, str, 1024);
 //    	printf("name : %s \n " , jsonItem->valuestring);

        bson_free(str);
    }
    //역순으로 디비에 있는 데이터를 클라이언트에 전송 
    for (i = 9; i >= 0; i--) {
        if (dbname[i][0] != '\0') {
            wresult = write(sockTmp->cSock, dbname[i], 1024);
            wresult = write(sockTmp->cSock, dbchat[i], 1024);
        }
    }

    // 변수 초기화할떄 \0으로 초기화되는거 같아서 자동으로 빈 데이터를 보내는거 같아서 그것을 읽는 코드 추가 
    result = read(sockTmp->cSock, buf, 1024);

    while (1) {

        // 클라이언트에게 메시지를 받음 
        result = read(sockTmp->cSock, buf, 1024);

        // 메시지 받는게 실패하면 해당 클라이언트 소켓 종료 
        if (result < 1) {

            client_del((struct sockS*)sockSTmp);
            //printf("connect fail ");
            break;
        }

        printf("read Data : %s \n", buf);
        // 클라이언트들에게 메시지를 보내고 몽고디비에 데이터를 넣음 넣는 데이터는 내용, 시간, 이름
        write_to_client(buf, name);
        bson = bson_new();
        t = time(NULL);
        BSON_APPEND_UTF8(bson, "chat", buf);
        BSON_APPEND_UTF8(bson, "name", name);
        BSON_APPEND_UTF8(bson, "time", ctime(&t));

        if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, bson, NULL, &error)) {
            fprintf(stderr, "%s\n", error.message);
        }
        bson_free(bson);
        //받은 메시지를 클라이언트에게 보냄 
        //wresult = send(cSock, buf, 1024, 1);
        if (wresult < 1) {
            client_del((struct sockS*)sockSTmp);
            //printf("connect fail ");
            break;
        }

    }


}
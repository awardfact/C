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




// ������ �μ��� ���� ����ü 
struct sockS {
    int sSock;
    int cSock;
    pthread_t client;
    struct sockS* before;
    struct sockS* next;
};


// ���� �������� 


//ó�� ����Ű�� ������ 
struct sockS* firstClient = NULL;

// ���� ���� ���� ����Ű�� ������ 
struct sockS* beforeClient = NULL;
struct sockS* currentClient = NULL;
struct sockS* nextClient = NULL;
struct sockS* clientTmp = NULL;



// ������ ���ῡ �ʿ��� ���� ���� 
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

//Ŭ���̾�Ʈ �߰� �Լ�
void client_add(int so, int sock);
//Ŭ���̾�Ʈ ���� �Լ� 
void client_del(struct sockS* client);
//Ŭ���̾�Ʈ���� �޽��� ���� �Լ� 
void write_to_client(char* buf, char* name);

void* connect_client(void* soTmp);
/*
���� ����
1. ���� ����
2. bind
3. listen
4. accept
5. send / recv
6. close

*/
int main(int argc, char* argv[])
{


    //���� �ּ� 
    struct sockaddr_in address;

    int client_addr_size = sizeof(address);
    // Ŭ���̾�Ʈ ������ 

    // Ŭ���̾�Ʈ �迭 ��ȣ 
    int client_i = 0;
    //Ŭ���̾�Ʈ ���� ��ȣ 
    int cs = 0;
    int status;

    // ���� �ּ� ũ��
    int addresslen = sizeof(address);
    //����  ���� ��ȣ 
    int sSock;
    int cSock;



    mongoc_init();
    //������ ���� 
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
     //������ �����ͺ��̽��� ���̺� ������ 
    database = mongoc_client_get_database(client, "chatting");
    collection = mongoc_client_get_collection(client, "chatting", "chatlog");

    //ipv4
    address.sin_family = AF_INET;
    // ���� ������ 
    address.sin_addr.s_addr = inet_addr(argv[1]);
    //��Ʈ��ȣ 
    address.sin_port = htons(atoi(argv[2]));

    int result;

    // 1.������ ����� 
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
            // Ŭ���̾�Ʈ�� �����ϸ� �����带 ����� �ۼ����ϴ�  ������ �Լ� ���� 
            client_add(cSock, sSock);
            //cs = pthread_create(&client[client_i], NULL, connect_client,&so[client_i]);

           // printf("sSock : %d   cSock : %d     \n", currentClient->sSock, currentClient->cSock);
            cs = pthread_create(&currentClient->client, NULL, connect_client, currentClient);

        }

    }

    //������ �Ҵ� ���� 
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

//Ŭ���̾�Ʈ �߰� �Լ� Ŭ���̾�Ʈ�� ���� ����Ʈ�� ���� 
void client_add(int cSock, int sSock) {

    // printf("sScok : %d   cScok :  %d\n", sSock, cSock);
     //�޸� �Ҵ� �� �� Ŭ���̾�Ʈ ����, ���� ������ Ŭ���̾�Ʈ ����ü�� �ִ´� 
    clientTmp = (struct sockS*)malloc(sizeof(struct sockS));
    clientTmp->sSock = sSock;
    clientTmp->cSock = cSock;

    // ������ Ŭ���̾�Ʈ�� ������ ��� null
    clientTmp->next = NULL;
    clientTmp->before = NULL;

    //���� Ŭ���̾�Ʈ�� ������ ���� Ŭ���̾�Ʈ �� Ŭ���̾�Ʈ�� ������ Ŭ���̾�Ʈ�� �ȴ� 
    if (currentClient != NULL) {
        clientTmp->before = currentClient;
        currentClient->next = clientTmp;

        beforeClient = currentClient;
    }

    // ���� Ŭ���̾�Ʈ�� ������ ������ Ŭ���̾�Ʈ

    printf("sScok : %d   cScok :  %d\n", clientTmp->sSock, clientTmp->cSock);

    //ó�� Ŭ���̾�Ʈ�� ������ ���� Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� �ȴ� 
    if (firstClient == NULL) {
        firstClient = clientTmp;
    }

    //���� Ŭ���̾�Ʈ�� ������ Ŭ���̾�Ʈ 

    currentClient = clientTmp;


    return;

}

//Ŭ���̾�Ʈ ���� �Լ� 
void client_del(struct sockS* client) {

    //���� Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� �ִ� ��� 
    if (client == currentClient && beforeClient != NULL) {
        // ���� Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� �ȴ� (���� Ŭ���̾�Ʈ�� ������ null)
        currentClient = beforeClient;

        //��(����)Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� ������ ���� Ŭ���̾�Ʈ�� ��(����)Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� �ȴ� 
        if (currentClient->before != NULL) {
            beforeClient = currentClient->before;
        }
    }
    // ���� Ŭ���̾�Ʈ�� ������ ���� Ŭ���̾�Ʈ�� null�� �ٲ۴�  
    else if (client == currentClient) {
        currentClient = NULL;
        firstClient = NULL;
    }

    // ���� Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� �ִ� ��� 
    if (client->next != NULL) {
        // ���� Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� ������ ����Ŭ���̾�Ʈ�� ������ ����Ŭ���̾�Ʈ�� ���� , �蠨Ŭ���̾�Ʈ ���� Ŭ���̾�Ʈ�� ������ ���� Ŭ���̾�Ʈ ���� Ŭ���̾�Ʈ 
        if (client->before != NULL) {
            client->next->before = client->before;
            client->before->next = client->next;
        }
        // ����Ŭ���̾�Ʈ ���� Ŭ���̾�Ʈ�� ������ ���� Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� null�̵ȴ� 
        else {
            client->next->before = NULL;
        }
    }
    // ���� Ŭ���̾�Ʈ ���� Ŭ���̾�Ʈ�� �������� Ŭ���̾�Ʈ�� ������ ���� Ŭ���̾�Ʈ�� ���� Ŭ���̾�Ʈ�� null�� �ȴ� 
    else {
        if (client->before != NULL) {
            client->before->next = NULL;
        }
    }

    //Ŭ���̾�Ʈ ���� �ݱ� 
    close(client->cSock);
    //���� Ŭ���̾�Ʈ �޸� �Ҵ� ���� 
    free(client);


    printf("client connect exit\n");
}

// Ŭ���̾�Ʈ�鿡�� �޽����� ������ �Լ� 
void write_to_client(char* buf, char* name) {

    int wresult;
    //ó�� Ŭ���̾�Ʈ�� ����Ű�� �ӽ� Ŭ���̾�Ʈ ���� ����
    struct sockS* clientFunctionTmp = firstClient;
    //ó�� Ŭ���̾�Ʈ�� ������ ����� Ŭ���̾�Ʈ�� ������ ��� 




    if (clientFunctionTmp == NULL) {

        printf("no connected Client\n");
    }
    else {
        //Ŭ���̾�Ʈ�� ������ Ŭ���̾�Ʈ�� ������ null�̵ɶ����� Ŭ���̾�Ʈ���� �޽��� ���� 
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


// Ŭ���̾�Ʈ�� �ۼ����ϴ� ������ �Լ� 
void* connect_client(void* sockSTmp) {

    // �μ��� ���� ���� ����ü�� ��ȯ

    struct sockS* sockTmp = (struct sockS*)sockSTmp;
    // struct sockS* sockTmp = NULL;
    // sockTmp->sSock =sockSTmp->sSock;
    // sockTmp->cSock = sockSTmp->cSock;
    // sockTmp->client = sockSTmp->client;
     // ���� ���� ��ȣ�� Ŭ���̾�Ʈ ���� ��ȣ�� ������ ���� 


    //printf("sSock : %d   cSock : %d     \n", currentClient->sSock, currentClient->cSock);
    //printf("sSock : %d  and cSock  : %d  before :  %d    after  :  %d \n", sockTmp->(int)sSock, sockTmp->(int)cSock , sockTmp->(int)before, sockTmp->(int)next);
    //���� ���� �����ߴ��� ���θ� �޴� ���� ���� 
    int result = 0;
    int wresult = 0;
    int i = 0;
    char* str;
    char name[100] = "client";
    //�޽����� ��� ���� ���� 
    char buf[1024] = "input your name : \0";

    //Ŭ���̾�Ʈ�� ȯ�� �޽����� ����
    wresult = write(sockTmp->cSock, "hello Server", 1024);
    //    wresult = write(sockTmp->cSock, buf, 1024);

    //�̸� �Է��϶�� Ŭ���̾�Ʈ���� ���� 
    wresult = write(sockTmp->cSock, buf, 1024);
    //write(sock, buf, 1024);
    //Ŭ���̾�Ʈ�� �̸��� �޾ƿ� 
    result = read(sockTmp->cSock, name, 1024);
    printf("%s's join \n", name);

    //������ ���ִ� �����͸� �ֽż����� 10���� ������ 
    const bson_t* doc;
    cJSON* json = NULL;
    cJSON* jsonItem = NULL;
    bson_t* filter, * opts;
    mongoc_cursor_t* cursor;
    filter = bson_new();
    opts = BCON_NEW("limit", BCON_INT32(10), "sort", "{", "time", BCON_INT32(-1), "}");


    cursor = mongoc_collection_find_with_opts(collection, filter, opts, NULL);
    //ä�� �α׸� ���� �迭 
    char dbname[10][1024] = { '\0', };
    char dbchat[10][1024] = { '\0', };


    // �ֽż����� �����͸� �������� ���� �ֽ� �����Ͱ� ���� ���� ���� ������ �ٷ� ������ �ʰ� �迭�� �����͸� �����ߴٰ� �迭�� �������� ������ ����
    while (mongoc_cursor_next(cursor, &doc)) {
        printf("cursor \n");
        //��� �����͸� json���� ���� 
        str = bson_as_canonical_extended_json(doc, NULL);
        // json �Ľ�
        json = cJSON_Parse(str);
        // name�����͸� ������ 
        jsonItem = cJSON_GetObjectItem(json, "name");
        //name ���� �迭�� ����
        strcpy(dbname[i], jsonItem->valuestring);
        // chat�����͸� ������ 
        jsonItem = cJSON_GetObjectItem(json, "chat");
        //chat�����͸� �迭�� ����
        strcpy(dbchat[i], jsonItem->valuestring);
        i++;
        // wresult = write(sockTmp->cSock, str, 1024);
 //    	printf("name : %s \n " , jsonItem->valuestring);

        bson_free(str);
    }
    //�������� ��� �ִ� �����͸� Ŭ���̾�Ʈ�� ���� 
    for (i = 9; i >= 0; i--) {
        if (dbname[i][0] != '\0') {
            wresult = write(sockTmp->cSock, dbname[i], 1024);
            wresult = write(sockTmp->cSock, dbchat[i], 1024);
        }
    }

    // ���� �ʱ�ȭ�ҋ� \0���� �ʱ�ȭ�Ǵ°� ���Ƽ� �ڵ����� �� �����͸� �����°� ���Ƽ� �װ��� �д� �ڵ� �߰� 
    result = read(sockTmp->cSock, buf, 1024);

    while (1) {

        // Ŭ���̾�Ʈ���� �޽����� ���� 
        result = read(sockTmp->cSock, buf, 1024);

        // �޽��� �޴°� �����ϸ� �ش� Ŭ���̾�Ʈ ���� ���� 
        if (result < 1) {

            client_del((struct sockS*)sockSTmp);
            //printf("connect fail ");
            break;
        }

        printf("read Data : %s \n", buf);
        // Ŭ���̾�Ʈ�鿡�� �޽����� ������ ������ �����͸� ���� �ִ� �����ʹ� ����, �ð�, �̸�
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
        //���� �޽����� Ŭ���̾�Ʈ���� ���� 
        //wresult = send(cSock, buf, 1024, 1);
        if (wresult < 1) {
            client_del((struct sockS*)sockSTmp);
            //printf("connect fail ");
            break;
        }

    }


}
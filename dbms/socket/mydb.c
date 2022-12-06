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




// ���� �������� 


//ó�� ����Ű�� ������ 
struct sockS* firstClient = NULL;

// ���� ���� ���� ����Ű�� ������ 
struct sockS* beforeClient = NULL;
struct sockS* currentClient = NULL;
struct sockS* nextClient = NULL;
struct sockS* clientTmp = NULL;




time_t t;

char* str;


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

   // printf("socket  : %d\n ", sSock);
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

    printf("0\n");
    struct sockS* sockTmp = (struct sockS*)sockSTmp;
    // struct sockS* sockTmp = NULL;
    // sockTmp->sSock =sockSTmp->sSock;
    // sockTmp->cSock = sockSTmp->cSock;
    // sockTmp->client = sockSTmp->client;
     // ���� ���� ��ȣ�� Ŭ���̾�Ʈ ���� ��ȣ�� ������ ���� 

    int userCount;

    // ����� �������� 
    struct user* tmpUser = NULL;  // �ӽ÷� user�� �ٷ� �� ����ϴ� �ӽ� user����
    struct user* firstUser = NULL;  // ù���� user�� ����Ű�� user������
    struct user* lastUser = NULL;   // ���� user�� ����Ű�� user������(���� �������� ������ user)
    struct user* selectedUser = NULL;   // ���� �������� user�� ����Ű�� user ������ 

    // ��� �������� 
    struct db* tmpDb = NULL; // �ӽ÷� db�� �ٷ� �� ����ϴ� �ӽ� db�����ͺ���
    struct db* firstDb = NULL;  // ���� ������ ������ �ִ� ù���� db  �α������� �ƴ϶�� null   user�� firstDb�� �̰��� �ȴ�
    struct db* lastDb = NULL; // ���� ������ ������ �ִ� ������ db �α����� �ƴϸ� null user�� firstDb�� before�� ������ �װ��� �ǰ�    ���ٸ� fristDb�� ������ null
    struct db* selectedDb = NULL;   //���� ������ ������ db

    //���̺� �������� 
    struct table* tmpTable = NULL; // �ӽ÷� ���̺��� �ٷ� �� ����ϴ� ���̺������ͺ���  
    struct table* firstTable = NULL; //  DB�� �������� �� DB�� ù��° ���̺� 
    struct table* lastTable = NULL;  // DB�� �������� �� DB�� ������ ���̺� 
    struct talbe* selectedTable = NULL;   //  ���� �������� ���̺� 


    //Ʃ�� �������� 
    struct tuple* tmpTuple = NULL;
    struct tuple* firstTuple = NULL;
    struct tuple* lastTuple = NULL;


    //�ʵ� �������� 
    struct field* tmpField = NULL;  // �ӽ÷� �ʵ带 �ٷ� �� ����ϴ� �ʵ�
    struct field* tmpField2 = NULL;  // �ӽ÷� �ʵ带 �ٷ� �� ����ϴ� �ʵ�
    struct field* firstField = NULL;  // ������ ���̺��� ù��° �ʵ�
    struct field* lastField = NULL;  // ������ ���̺��� ������ �ʵ� 
    struct field* selectedField = NULL; // ���� �������� �ʵ� (��������� �𸣰���)



    //������ �������� 
    struct data* tmpData = NULL;  // �ӽ÷� �����͸� �ٷ� �� ����ϴ� ������
    struct data* firstData = NULL; // ���� Ʃ���� ù ������
    struct data* lastData = NULL; // ���� Ʃ���� ������ ������

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
    //���� ���� �����ߴ��� ���θ� �޴� ���� ���� 
    int result = 0;
    int wresult = 0;

    char* str;
    char name[1024] = "client";
    //�޽����� ��� ���� ���� 
    char buf[1024] = "input your name : \0";



    getKey[0] = '\0';
    //Ŭ���̾�Ʈ�� ȯ�� �޽����� ����
    wresult = write(sockTmp->cSock, "hello Server", 1024);

    getUser(&firstUser, &lastUser, &tmpUser, &tmpDb, &tmpTable, &tmpField, &tmpField2, &tmpTuple, &tmpData);
    while (1) {




        if (monitor2 != 0) {


            result = read(sockTmp->cSock, getKey, 1024);
            monitor2 = 0;


        }


        // monitor�� 0�̸� ����ȭ�� 1�̸� �α���ȭ�� 2�� ���ȭ���� ����Ѵ� 
        switch (monitor) {

            //����ȭ�� 
        case 0:
            //getKEy�� Null�̸� �Է��� ����ȭ�鿡�� �Է��� �޴´� 
            if (getKey[0] == '\0') {

                showMain(mainMsg , sockTmp);
                // Ŭ���̾�Ʈ���� �޽����� ���� 
                result = read(sockTmp->cSock, getKey, 1024);

                // �޽��� �޴°� �����ϸ� �ش� Ŭ���̾�Ʈ ���� ���� 
                if (result < 1) {

                    client_del((struct sockS*)sockSTmp);
                    //printf("connect fail ");
                    break;
                }



            }

            //printf("%d \n ", &sockTmp);
            // 0�� �Է������� ���α׷� ���� 
            if (getKey[0] == 48) {
                exit = 1;
            }
            //1�� �Է������� �α��� ȭ������ �̵�
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
            //2�� �Է������� ȸ������ ȭ������ �̵� 
            else if (getKey[0] == 50) {
                // ȸ������ �����ϸ� ������������ �̵��ϰ� ���� ��� 
                if (signUp(&firstUser, &lastUser, &tmpUser , sockTmp) == 1) {
                    strcpy(mainMsg, "sign up success!\0");
                    getKey[0] = '\0';
                }
                // ȸ������ �����ϸ� ������������ �̵��ϰ� ���� ��� 
                else {
                    strcpy(mainMsg, "sign up fail.\0");
                    getKey[0] = '\0';
                }
                //3�� �Է��ϸ� ȸ�� Ż�� ȭ������ �̵�
            }
            else if (getKey[0] == 51) {
                // ȸ��Ż�� �����ϸ� ������������ �̵��ϰ� ���� ��� 
                if (signOut(&firstUser, &lastUser, &tmpUser , sockTmp ) == 1) {
                    strcpy(mainMsg, "sign out success!\0");
                    getKey[0] = '\0';


                }
                // ȸ��Ż�� �����ϸ� ������������ �̵��ϰ� ���� ��� 
                else {
                    strcpy(mainMsg, "sign out fail.\0");
                    getKey[0] = '\0';
                }

            }

            else {
                getKey[0] = '\0';

            }


            break;
            //�α��� ȭ�� 
        case 1:
            //getKEy�� Null�̸� �Է��� ����ȭ�鿡�� �Է��� �޴´� 
            if (getKey[0] == '\0') {
                //	printf("%s \n", selectedUser->id);
                showLogin(mainMsg, selectedUser->id , sockTmp);
                result = read(sockTmp->cSock, getKey, 1024);
                if (result < 1) {
                    client_del((struct sockS*)sockSTmp);
                    break;
                }


            }

            // 0�� �Է������� ���α׷� ���� 
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
            // 2�� �Է��ϸ� �α׾ƿ� 
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
        //// Ŭ���̾�Ʈ�鿡�� �޽����� ������ ������ �����͸� ���� �ִ� �����ʹ� ����, �ð�, �̸�
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
        ////���� �޽����� Ŭ���̾�Ʈ���� ���� 
        ////wresult = send(cSock, buf, 1024, 1);
        //if (wresult < 1) {
        //    client_del((struct sockS*)sockSTmp);
        //    //printf("connect fail ");
        //    break;
        //}

    }


}
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
struct chatt chatMsg[20];
int chatCnt = 0;
int sock2;
int dbGet = 0;
time_t t;


//Ŭ���̾�Ʈ �߰� �Լ�
void client_add(int so, int sock);
//Ŭ���̾�Ʈ ���� �Լ� 
void client_del(struct sockS* client);
//Ŭ���̾�Ʈ���� �޽��� ���� �Լ� 
void write_to_client(char* buf, char* name);

void* connect_client(void* soTmp);
void* write_msg(void* soTmp);
void* read_msg(void* soTmp);

void write_db(char name[], char msg[], char time[]);
void get_db();
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
    struct sockaddr_in address2;
    int client_addr_size = sizeof(address);
    int client_addr_size2 = sizeof(address2);
    // Ŭ���̾�Ʈ ������ 

    // Ŭ���̾�Ʈ �迭 ��ȣ 
    int client_i = 0;
    //Ŭ���̾�Ʈ ���� ��ȣ 
    int cs = 0;
    int status;

    // ���� �ּ� ũ��
    int addresslen = sizeof(address);
    int addresslen2 = sizeof(address2);
    //����  ���� ��ȣ 
    int sSock;
    int cSock;


    // Ŭ���̾�Ʈ ������ 
    pthread_t client[2];

    //�� /���� ������ ��ȣ
    int readT;
    int writeT;

    int result;
    int result2;
    int optval = 1;


    //ipv4
    address.sin_family = AF_INET;
    // ���� ������ 
    address.sin_addr.s_addr = inet_addr(argv[1]);
    //��Ʈ��ȣ 
    address.sin_port = htons(atoi(argv[2]));



    //���� ��� ������ ������ �Ѵ� 
    //ipv4
    address2.sin_family = AF_INET;
    // ���� ������ 
    address2.sin_addr.s_addr = inet_addr(argv[1]);
    //��Ʈ��ȣ 
    address2.sin_port = htons(atoi(argv[3]));

    //1. ���� ����
    if ((sock2 = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket fail.");
    }

    // 2. ���� ���� 
    if (connect(sock2, (struct sockaddr*)&address2, sizeof(address2)) < 0) {
        printf("Connect fail\n");
    }

    //��� ������ �ۼ����ϴ� ������ ���� 
    //������ ���� ��ȣ�� �ۼ��� �����忡 �ְ� ������ �Լ� ���� 
    readT = pthread_create(&client[0], NULL, write_msg, &sock2);
    writeT = pthread_create(&client[1], NULL, read_msg, &sock2);


    





    // 1.������ ����� 
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
            // Ŭ���̾�Ʈ�� �����ϸ� �����带 ����� �ۼ����ϴ�  ������ �Լ� ���� 
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


    printf("����  sdasdasdasdasdasdasd\n");
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
    char buf[1024] = "hello Server";

    
    //Ŭ���̾�Ʈ�� ȯ�� �޽����� ����
    wresult = write(sockTmp->cSock, buf, 1024);
    //    wresult = write(sockTmp->cSock, buf, 1024);
    //�̸� �Է��϶�� Ŭ���̾�Ʈ���� ���� 
    strcpy(buf, "input your name : \0");

    wresult = write(sockTmp->cSock, buf, 1024);
    //write(sock, buf, 1024);
    //Ŭ���̾�Ʈ�� �̸��� �޾ƿ� 
    result = read(sockTmp->cSock, name, 1024);
    printf("%s's join \n", name);


  //  printf("%s's 22222222222222222222222222222222222222 \n", name);


    //��� �������� �ֽ� ���� 10���� �޾ƿ��� �����ϴ� �Լ� 
    get_db();
    dbGet = 1;

    //�޾ƿ��µ� �ð��� ���� �ʿ��ؼ� 0.5������ ���� 
    usleep(50000);

 //   printf("chatCnt : %d \n ", chatCnt);
//

    //�������� ��� 
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
    // ���� �ʱ�ȭ�ҋ� \0���� �ʱ�ȭ�Ǵ°� ���Ƽ� �ڵ����� �� �����͸� �����°� ���Ƽ� �װ��� �д� �ڵ� �߰� 
   // result = read(sockTmp->cSock, buf, 1024);

    while (1) {
       // printf("readWhilew \n ", chatCnt);
        // Ŭ���̾�Ʈ���� �޽����� ���� 
        result = read(sockTmp->cSock, buf, 1024);
        printf("read Data : %s \n", buf);
        // �޽��� �޴°� �����ϸ� �ش� Ŭ���̾�Ʈ ���� ���� 
        if (result < 1) {
            client_del((struct sockS*)sockSTmp);
            //printf("connect fail ");
            break;
        }


       //�޾ƿ� �����͸� Ŭ���̾�Ʈ�鿡�� ������ ��� ������ insert�� ���� 
        t = time(NULL);
        write_to_client(buf, name);
        write_db(name, buf, ctime(&t));


        //���� �޽����� Ŭ���̾�Ʈ���� ���� 
      //  wresult = send(cSock, buf, 1024, 1);


    }


}





// ��� ���� ���� ������ 
void* read_msg(void* soTmp) {

    //���� ��ȣ ���� ����
    int* so = (int*)soTmp;


    int result = 0;
    char buf[1024];
    int type = 0;
    while (1) {

        //Ŭ���̾�Ʈ�� �����ؼ� �ֽ� ������ 10���� �޴� ���� 
        if (dbGet == 1) {

            // �������Լ� ������ ���� 
            result = read(*so, buf, 1024);

            if (strcmp(buf, "!end!") == 0) {
                dbGet = 1;
                // break;
            }
            printf("read Data : %s \n", buf);
            //�����͸� �޾Ƽ� �迭�� �ִ´� 
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
        // �⺻ ���� 
        else {



            // �������Լ� ������ ���� 
            result = read(*so, buf, 1024);


            if (dbGet == 1) {

                // �������Լ� ������ ���� 
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


            //������ �����ϸ� ���� ���� 
            if (result < 1) {
                printf("connect fail ");
                close(*so);
                break;
            }

            //������ ������ ��� 
            printf("%s : ", buf);



        }


    }


}

//��� ������ �޽����� ������ �Լ� 
void* write_msg(void* soTmp) {



    //���� ��ȣ ���� ����
    int* so = (int*)soTmp;

    int result = 0;

    char buf[1024];
    int i = 0;



    while (1) {
        //printf("%d socket send : ", sock);
    //rewind(stdin);
        //�۽��� �����͸� ����ڿ��� �Է¹��� 
//        fgets(buf,1024,stdin);

        i = 0;
        //scanf�� �����͸� ������ ������ ������ �ǰ� scanf���� ������ �����Ͽ� �����͸� ������ ���ѷ��� ���ͼ� getchar�� ���� �����͸� �޾ƿ� 
       //��� ��ɾ �޾Ƽ� ��� ���� 
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
                // �������Լ� ������ �۽� 
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




///��� ������  ������ �Է��Լ�  
void write_db(char name[], char msg[], char time[]) {


   // printf("name :  %s \n", name);
   // printf("msg :  %s \n", msg);
  //  printf("time :  %s \n", time);
    //���� ��ȣ ���� ����
   // int* so = (int*)soTmp;

    int result = 0;

    char buf[1024];
    int i = 0;
    // 1���� �Է��ؾ� ��ɾ� �Է�â�� ���� ���� 1�� �Է� 
    result = write(sock2, "1", 1024);
    if (result < 1) {
        printf("connect fail ");
        close(sock2);
        return;
    }
    // �޾ƿ� �����͸� �̿��ؼ� insert���� ���� ��� ������ ������ 
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


///��� ������  �Է� �Լ� 

void get_db() {



    printf("1\n");
    //���� ��ȣ ���� ����
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
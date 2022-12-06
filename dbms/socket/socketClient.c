#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#define PORT = 8000;


void* write_msg(void* soTmp);
void* read_msg(void* soTmp);
pthread_t client[2];
int sock;

/*
���� Ŭ���̾�Ʈ
1. ���� ����
2. ������ ����
3. �ۼ���
4. close
*/
int main(int argc, char* argv[])
{


    int optval = 1;

    char buf[1024];

    // Ŭ���̾�Ʈ ������ 


    //�� /���� ������ ��ȣ
    int readT;
    int writeT;

    // ���� �ּ� 
    struct sockaddr_in address;

    //���Ϲ�ȣ 


    //ipv4
    address.sin_family = AF_INET;
    // ���� ������ 
    address.sin_addr.s_addr = inet_addr(argv[1]);
    //��Ʈ��ȣ 
    address.sin_port = htons(atoi(argv[2]));


    int result = 0;


    //1. ���� ����
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket fail.");
    }

    // 2. ���� ���� 
    if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("Connect fail\n");
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    printf("so = %d \n", sock);
    printf("sock : %d", sock);
    result = read(sock, buf, 1024);
    printf("%s \n", buf);
    result = read(sock, buf, 1024);
    printf("%s", buf);
    scanf("%s", buf);
    result = write(sock, buf, strlen(buf) + 1);
    //if (result < 1) {
    //    printf("connect fail ");
    //    close(sock);
    //    return 0;
    //}

    //������ ���� ��ȣ�� �ۼ��� �����忡 �ְ� ������ �Լ� ���� 
    readT = pthread_create(&client[0], NULL, write_msg, &sock);
    writeT = pthread_create(&client[1], NULL, read_msg, &sock);


    /*
     while (1) {
         printf("%d socket send : ",sock);
         scanf("%s", buf);
         result = write(sock, buf, 1024);


         printf("write result = %d \n", result);
        // printf("%s", buf);
         //send(sock, buf, strlen(buf));
     }


    */


    pthread_join(client[0], NULL);
    pthread_join(client[1], NULL);

    return 0;

}



// ���� ������ 
void* read_msg(void* soTmp) {

    //���� ��ȣ ���� ����
    int* so = (int*)soTmp;


    int result = 0;
    char buf[1024];

    printf("so = %d \n", sock);
    printf("read Start \n");
    while (1) {

        // �������Լ� ������ ���� 
        result = read(sock, buf, 1024);

  //      printf("read mid \n");
        //������ �����ϸ� ���� ���� 
        if (result < 1) {
            printf("connect fail ");
            close(sock);
            break;
        }

        //������ ������ ��� 
        printf("%s : ", buf);


        // �������Լ� ������ ���� 
        result = read(sock, buf, 1024);


        //������ �����ϸ� ���� ���� 
        if (result < 1) {
            printf("connect fail ");
            close(sock);
            break;
        }

        //������ ������ ��� 
        printf("%s\n", buf);

    }
  //  printf("read end \n");
    close(sock);
}

//�۽� ������ 
void* write_msg(void* soTmp) {
    //���� ��ȣ ���� ����
    int* so = (int*)soTmp;

    int result = 0;

    char buf[1024];
    int i = 0;

    printf("so = %d \n", sock);
    printf("write Start \n");

    while (1) {
        //printf("%d socket send : ", sock);
    //rewind(stdin);
        //�۽��� �����͸� ����ڿ��� �Է¹��� 
//        fgets(buf,1024,stdin);

        i = 0;
        buf[0] = '\0';
        //scanf�� �����͸� ������ ������ ������ �ǰ� scanf���� ������ �����Ͽ� �����͸� ������ ���ѷ��� ���ͼ� getchar�� ���� �����͸� �޾ƿ� 
        //while (buf[i] = getchar()) {
        //    if (buf[i] == '\n') {
        //        break;
        //    }
        //    else {
        //        i++;
        //    }
        //}
//        buf[i] = '\0';
      //  printf("write mid  %s \n");
        //        scanf("%[^\n]s", buf);
                // �������Լ� ������ �۽� 


        scanf("%s", buf);
        if (buf[0] != '\0') {

            result = write(sock, buf, strlen(buf) + 1);

            //rewind(stdin);

            if (result < 1) {
                printf("connect fail ");
                close(sock);
                break;
            }

        }

        //printf("write result = %d \n", result);
        // printf("%s", buf);
         //send(sock, buf, strlen(buf));
    }
    printf("write end \n");


    close(sock);

}

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#define PORT = 8000;


void* write_msg(void* soTmp);
void* read_msg(void* soTmp);

/*
���� Ŭ���̾�Ʈ
1. ���� ����
2. ������ ����
3. �ۼ���
4. close
*/
int main(int argc, char* argv[])
{



    char buf[1024];

    // Ŭ���̾�Ʈ ������ 
    pthread_t client[2];

    //�� /���� ������ ��ȣ
    int readT;
    int writeT;

    // ���� �ּ� 
    struct sockaddr_in address;

    //���Ϲ�ȣ 
    int sock;


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



    //printf("sock : %d", sock);
    result = read(sock, buf, 1024);
    printf("%s \n", buf);
    result = read(sock, buf, 1024);
    printf("%s", buf);
    scanf("%s", buf);
    result = write(sock, buf, 1024);
    if (result < 1) {
        printf("connect fail ");
        close(sock);
        return 0;
    }

    //������ ���� ��ȣ�� �ۼ��� �����忡 �ְ� ������ �Լ� ���� 
    readT = pthread_create(&client[0], NULL, write_msg, &sock);
    writeT = pthread_create(&client[1], NULL, read_msg, &sock);

    pthread_join(client[0], NULL);
    pthread_join(client[1], NULL);

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

    // close(sock);


    return 0;

}



// ���� ������ 
void* read_msg(void* soTmp) {

    //���� ��ȣ ���� ����
    int* so = (int*)soTmp;


    int result = 0;
    char buf[1024];

    while (1) {

        // �������Լ� ������ ���� 
        result = read(*so, buf, 1024);


        //������ �����ϸ� ���� ���� 
        if (result < 1) {
            printf("connect fail ");
            close(*so);
            break;
        }

        //������ ������ ��� 
        printf("%s : ", buf);


        // �������Լ� ������ ���� 
        result = read(*so, buf, 1024);


        //������ �����ϸ� ���� ���� 
        if (result < 1) {
            printf("connect fail ");
            close(*so);
            break;
        }

        //������ ������ ��� 
        printf("%s\n", buf);

    }


}

//�۽� ������ 
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
        result = write(*so, buf, 1024);

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

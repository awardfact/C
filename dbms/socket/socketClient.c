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
소켓 클라이언트
1. 소켓 생성
2. 서버에 연결
3. 송수신
4. close
*/
int main(int argc, char* argv[])
{


    int optval = 1;

    char buf[1024];

    // 클라이언트 쓰레드 


    //송 /수신 쓰레드 번호
    int readT;
    int writeT;

    // 소켓 주소 
    struct sockaddr_in address;

    //소켓번호 


    //ipv4
    address.sin_family = AF_INET;
    // 서버 아이피 
    address.sin_addr.s_addr = inet_addr(argv[1]);
    //포트번호 
    address.sin_port = htons(atoi(argv[2]));


    int result = 0;


    //1. 소켓 생성
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket fail.");
    }

    // 2. 소켓 연결 
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

    //연결한 소켓 번호를 송수신 쓰레드에 넣고 쓰레드 함수 실행 
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



// 수신 쓰레드 
void* read_msg(void* soTmp) {

    //소켓 번호 변수 생성
    int* so = (int*)soTmp;


    int result = 0;
    char buf[1024];

    printf("so = %d \n", sock);
    printf("read Start \n");
    while (1) {

        // 서버에게서 데이터 수신 
        result = read(sock, buf, 1024);

  //      printf("read mid \n");
        //수신이 실패하면 소켓 종료 
        if (result < 1) {
            printf("connect fail ");
            close(sock);
            break;
        }

        //수신한 데이터 출력 
        printf("%s : ", buf);


        // 서버에게서 데이터 수신 
        result = read(sock, buf, 1024);


        //수신이 실패하면 소켓 종료 
        if (result < 1) {
            printf("connect fail ");
            close(sock);
            break;
        }

        //수신한 데이터 출력 
        printf("%s\n", buf);

    }
  //  printf("read end \n");
    close(sock);
}

//송신 쓰레드 
void* write_msg(void* soTmp) {
    //소켓 번호 변수 생성
    int* so = (int*)soTmp;

    int result = 0;

    char buf[1024];
    int i = 0;

    printf("so = %d \n", sock);
    printf("write Start \n");

    while (1) {
        //printf("%d socket send : ", sock);
    //rewind(stdin);
        //송신한 데이터를 사용자에게 입력받음 
//        fgets(buf,1024,stdin);

        i = 0;
        buf[0] = '\0';
        //scanf로 데이터를 보내면 공백이 문제가 되고 scanf에서 공백을 포함하여 데이터를 넣으면 무한루프 나와서 getchar를 통해 데이터를 받아옴 
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
                // 서버에게서 데이터 송신 


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

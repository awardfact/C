#include <stdio.h>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <winsock.h>
#include <errno.h>
//#pragma comment(lib, "ws2_32.lib")

#define PORT = 8000;

//소켓 통신 서버 
int main(int argc, char* argv[])
{


    int port = 8001;
    char buf[1024];
    struct sockaddr_in address;

    int addresslen = sizeof(address);
    int sock;
    socklen_t clnt_addr_size;

    int so;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("10.0.2.15");
    address.sin_port = htons(port);


    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {


        printf("소켓 연결에 실패하였습니다.");
    }


    printf("scoket : %d \n ", sock);


    if (bind(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
        printf("bind 실패하였습니다  %d.\n", errno);
    }

    if (listen(sock, 5) < 0) {
        printf("listen 실패하였습니다.");
    }


    if ((so = accept(sock, (struct sockaddr*)&address, &clnt_addr_size)) < 0) {
        printf("accept");
    }


    read(sock, buf, 1024);
    printf("%s", buf);


    return 0;
}

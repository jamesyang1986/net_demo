//
// Created by wan on 23-12-19.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void send_by_stdin(int sock, char *buffer);

int main(int argc, char *argv[]) {
    char *ip = "127.0.0.1";
    int port = 9527;

    if (argc >= 2) {
        ip = argv[1];
    }

    if (argc >= 3) {
        port = atoi(argv[2]);
    }

    printf("start to connect the server, the ip is:%s, the port is:%d \n", ip, port);

    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock <= 0) {
        printf("fail to create client socket");
        exit(1);
    }

    printf("create socket success!\n");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {//连接
        printf("connect failed!!!\n");
        return 0;
    }
    printf("connect success!!!\n");
    char buffer[100];
    sleep(5);

    for (int i = 0; i < 100; i++) {
//        send_by_stdin(sock, buffer);
        char buff[100];
        sprintf(buff, "i am from client:%d\n ", i);
        send(sock, buff, sizeof(buff), 0);

        memset(buffer, 0, sizeof(buffer));
        sleep(5);
        int len = recv(sock, buffer, sizeof(buffer), 0);
        if (len == 0) {
            printf("server socket is closed.");
        } else {
            printf("Message form server: %s\n", buffer);
        }

        if (i == 15) {
            break;
        }
    }
    printf("start to close the client socket. \n");
    sleep(5);
    close(sock);
    return 0;
}

void send_by_stdin(int sock, char *buffer) {
    memset(buffer, 0, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strlen(buffer) - 1] = '\0';
    send(sock, buffer, sizeof(buffer), 0);
    sleep(1);
}

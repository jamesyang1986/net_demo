#include <stdio.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>

void set_socket_opt(int sock);
void handle_client(int clnt_sock);

void *handle_by_thread(void *arg){
    int fd = *((int *)arg);
    handle_client(fd);
}

void handle_client(int clnt_sock) {//
    int len;
    char buff[100];
    printf("a new client connected!!!!!\n");
    while (1) {
        if ((len = recv(clnt_sock, buff, sizeof(buff), 0)) > 0) {
            if (strcmp(buff, "exit") == 0) {
                printf("client exit\n");
                break;
            }

            time_t cur_time = time(NULL);
            printf("tid:%lu, time:%s, Message form client: %s\n", pthread_self(), ctime(&cur_time), buff);

            int size = send(clnt_sock, buff, sizeof(buff), 0);
            if (size == 0) {
                printf("client closed..");
                break;
            } else if (size < 0) {
                printf("socket send error, error:%s\n", strerror(errno));
                break;
            }
            memset(buff, 0, sizeof(buff));
        } else if (len == 0) {
            printf("client closed\n");
            break;
        } else {
            printf("socket read error, error:%s\n", strerror(errno));
        }
    }
    //关闭套接字
    close(clnt_sock);
    printf("clnt_socket closed\n");
}

int main() {
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("create socket success!\n");
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");  //具体的IP地址
    serv_addr.sin_port = htons(9527);  //端口


    bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));//绑定
    printf("bind success!!\n");
    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    printf("listening......\n");
    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    while (1) {
        int clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr,
                               &clnt_addr_size);//接收客户端的连接请求，如果没有客户端连接，则程序会在这里阻塞，直到有客户端的连接到来
        set_socket_opt(clnt_sock);
//        handle_client(clnt_sock);//为客户端的服务函数
        pthread_t t;
        if(pthread_create(&t, NULL, handle_by_thread, &clnt_sock) ){
            printf("fail to create pthread...");
            continue;
        }
    }

    sleep(100000);
    printf("start to close the tcp server...");
    close(serv_sock);
    return 0;
}

void set_socket_opt(int sock) {
    struct timeval ts;
    ts.tv_sec = 1;
    ts.tv_usec = 0;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &ts, sizeof(ts)) < 0) {
        printf("set socket rcv timeout error. close the socket.");
        close(sock);
        exit(1);
    }

    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &ts, sizeof(ts)) < 0) {
        printf("set socket rcv timeout error. close the socket.");
        close(sock);
        exit(1);
    }
}

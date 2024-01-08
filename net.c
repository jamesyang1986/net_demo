//
// Created by wan on 23-12-19.
//
#include "net.h"
#include <errno.h>

#define LISTEN_NUM 20
#define EPOLL_MAX_SIZE 1024
#define EPOLL_EVENT 100

int bind_socket(char *ip, int port) {
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock < 0) {
        printf("fail to create server socket\n");
        return -1;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);
    if (!bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        printf("fail to bind socket!!\n");
        return -1;
    }
    return serv_sock;
}

int listen_socket(int server_sock) {
    if(!listen_socket(server_sock, LISTEN_NUM)){
        printf("fail to listen the server socket, the fd is %d\n", server_sock);
        return -1;
    }
    return 0;
}

void do_epoll(int server_sock) {
     int epoll_fd =  epoll_create(EPOLL_MAX_SIZE);
     struct epoll_event events[EPOLL_EVENT];
     for(;;){
        int client_fd =  epoll_wait(epoll_fd,events, 100, 100);
     }

}

void handle_net_events(int epoll_fd, struct epoll_event *events, int num, int client_fd, char *buf) {

}

void do_accept(int epoll_fd, int client_fd) {

}

void do_read(int epoll_fd, int client_fd, char *buf) {

}

void do_write(int epoll_fd, int client_fd, char *buf) {

}

void add_net_event(int epoll_fd, int client_fd, int state) {

}

void remove_net_event(int epoll_fd, int client_fd, int state) {

}

void del_net_event(int epoll_fd, int client_fd, int state) {

}
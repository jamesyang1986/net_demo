//
// Created by wan on 23-12-19.
//
#include "net.h"
#include <errno.h>

#define LISTEN_NUM 20
#define EPOLL_MAX_SIZE 1024
#define EPOLL_EVENT 100
#define  MAX_SIZE 1024

int bind_socket(char *ip, int port) {
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock < 0) {
        perror("fail to create server socket");
        return -1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);
    if (!bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) {
        perror("fail to bind socket!!");
        return -1;
    }
    return serv_sock;
}

int listen_socket(int server_sock) {
    if (!listen_socket(server_sock, LISTEN_NUM)) {
        printf("fail to listen the server socket, the fd is %d\n", server_sock);
        return -1;
    }
    return 0;
}

void do_epoll(int server_sock) {
    int epoll_fd = epoll_create(EPOLL_MAX_SIZE);
    add_net_event(epoll_fd, server_sock, EPOLLIN);
    struct epoll_event events[EPOLL_EVENT];
    int num = 100;
    char *buf[1024];
    for (;;) {
        int size = epoll_wait(epoll_fd, events, num, 100);
        for (int i = 0; i < size; i++) {
            int fd = events[i].data.fd;
            if (fd == server_sock && (events[i].events & EPOLLIN)) {
                do_accept(epoll_fd, fd);
            } else if ((events[i].events & EPOLLIN)) {
                do_read(epoll_fd, fd, buf);
            } else {
                do_write(epoll_fd, fd, buf);
            }
        }

    }

}

void do_accept(int epoll_fd, int serv_sock) {
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int client_fd = accept(serv_sock, (struct sockaddr *) &clnt_addr,
                           &clnt_addr_size);
    if (client_fd == -1) {
        perror("fail to  accept socket.")
        return;
    }
    set_socket_opt(client_fd);
    add_net_event(epoll_fd, client_fd, EPOLLIN);
}

void do_read(int epoll_fd, int client_fd, char *buf, int num) {
    int nread = 0;
    int ret = 0;
    while (num > 0) {
        nread = read(client_fd, buf, num);
        if (nread == -1) {
            printf("socket read error: %s\n", strerror(errno));
            if (errno == EAGAIN) {
                continue;
            }

            remove_net_event(epoll_fd, client_fd, EPOLLIN);
        } else if (nread == 0) {
            perror("socket closed.")
            remove_net_event(epoll_fd, client_fd, EPOLLIN);
            continue;
        }
        num -= nread;
    }
    mod_net_event(epoll_fd, client_fd, EPOLLOUT);
}

void do_write(int epoll_fd, int client_fd, char *buf) {
    int len = strlen(buf);
    int n_write = write(client_fd, buf, strlen(buf));
    if (n_write == -1) {
        printf("fail to write the socket , the err:%s\n", strerror(errno));
        remove_net_event(epoll_fd, client_fd, EPOLLOUT);
        close(client_fd);
    } else {
        mod_net_event(epoll_fd, client_fd, EPOLLIN);
    }
}

void add_net_event(int epoll_fd, int client_fd, int state) {
    struct epoll_event ev;
    ev.data.fd = client_fd;
    ev.events = state;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
        perror("add epoll event failed.");
    }

}

void remove_net_event(int epoll_fd, int client_fd, int state) {
    struct epoll_event ev;
    ev.data.fd = client_fd;
    ev.events = state;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, &ev) < 0) {
        perror("del epoll event failed.");
    }

}

void mod_net_event(int epoll_fd, int client_fd, int state) {
    struct epoll_event ev;
    ev.data.fd = client_fd;
    ev.events = state;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_fd, &ev) < 0) {
        perror("mod epoll event failed.");
    }

}

void set_socket_opt(int sock) {
    struct timeval ts;
    ts.tv_sec = 1;
    ts.tv_usec = 0;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &ts, sizeof(ts)) < 0) {
        perror("set socket rcv timeout error. close the socket.");
        close(sock);
        exit(1);
    }

    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &ts, sizeof(ts)) < 0) {
        perror("set socket rcv timeout error. close the socket.");
        close(sock);
        exit(1);
    }

}
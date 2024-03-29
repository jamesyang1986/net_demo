//
// Created by wan on 23-12-19.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <string.h>

#ifndef NET_DEMO_NET_H
#define NET_DEMO_NET_H

int bind_socket(char *ip, int port);

int listen_socket(int server_sock);

void do_epoll(int server_sock);

void do_accept(int epoll_fd, int client_fd);

void do_read(int epoll_fd, int client_fd, char *buf);

void do_write(int epoll_fd, int client_fd, char *buf);

void add_net_event(int epoll_fd, int client_fd, int state);

void remove_net_event(int epoll_fd, int client_fd, int state);

void mod_net_event(int epoll_fd, int client_fd, int state);

void set_socket_opt(int sock);

#endif //NET_DEMO_NET_H

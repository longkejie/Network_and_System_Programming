/*************************************************************************
	> File Name: 1.server.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 26 Jan 2021 08:51:02 PM CST
 ************************************************************************/

#include "head.h"


#define MAX_EVENTS 10
struct epoll_event ev, events[MAX_EVENTS];
int nfds, epollfd;

int main (int argc, char **argv) {
    int server_listen, sockfd, port;
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port!\n",argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server_listen;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev) == -1) {
        perror("epoll_ctl : listen_sock");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    while (1) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_listen) {
                sockfd = accept(server_listen,(struct sockaddr *) &client, &len);
                if (sockfd == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                printf("<%s> is online\n",inet_ntoa(client.sin_addr));
                make_nonblock(sockfd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = sockfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
                    perror("epoll_ctl : sockfd");
                    exit(EXIT_FAILURE);
                }
            }else {
                sockfd = events[i].data.fd;
                char buff[512] = {0};
                char tobuff[1024] = {0};
                getpeername(sockfd, (struct sockaddr *)&client, &len);
                size_t ret = recv(sockfd, buff, sizeof(buff), 0);
                if (ret <= 0) {
                    printf("<%s> is offline\n",inet_ntoa(client.sin_addr));
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, NULL);
                    close(sockfd);
                }else {
                   printf("<%s> : %s\n", inet_ntoa(client.sin_addr), buff);
                   sprintf(tobuff, "I've recved your message: %s",buff);
                   send(sockfd, tobuff, strlen(tobuff),0); 
                }
                
            }
        } 
    }
}

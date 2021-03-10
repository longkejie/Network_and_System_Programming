/*************************************************************************
	> File Name: 1.server.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 26 Jan 2021 08:51:02 PM CST
 ************************************************************************/

#include "head.h"
#include "cmd.h"

#define MAX_EVENTS 10
#define MAX_CLIENTS 100

struct Client {
    char name[20];
    int fd;
    int online;
};


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
    
    struct Client *users = (struct Client*)malloc(sizeof(struct Client) * MAX_CLIENTS);


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
                
                char name[20] = {0};
                if (sockfd == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                printf("<%s> is online\n",inet_ntoa(client.sin_addr));
                make_nonblock(sockfd);
                if (recv_str_nonblock(sockfd, name, sizeof(name), 10000) != 0) {
                    printf("<LoginError : %s did not send his name...deleted...\n", inet_ntoa(client.sin_addr));
                    close(sockfd);
                    continue;
                }
                send(sockfd, "Welcome!", sizeof("Welcome!"), 0);
                users[sockfd].fd = sockfd;
                strcpy(users[sockfd].name, name);
                users[sockfd].online = 1;
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = sockfd;
                make_block(sockfd);
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
                    perror("epoll_ctl : sockfd");
                    exit(EXIT_FAILURE);
                }
                printf("<LoginSuccess> %s login on %s\n",name, inet_ntoa(client.sin_addr));
            }else {
                int flag = 0;
                sockfd = events[i].data.fd;
                char buff[1024] = {0};
                getpeername(sockfd, (struct sockaddr *)&client, &len);
                size_t ret = recv(sockfd, (char *)&flag, sizeof(flag), 0);
                if (ret <= 0) {
                    printf("<%s> is offline\n",inet_ntoa(client.sin_addr));
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, NULL);
                    close(sockfd);
                }else {
                    if (flag & SEND) {
                        char name[512] = {0}, filepath[1024] = "0";
                        printf("Client send data...\n");
                        ssize_t size = 0, sum = 0;
                        int ack = 1;
                        recv(sockfd, (void *)&size, sizeof(size), 0);
                        recv(sockfd, name, sizeof(name), 0);
                        send(sockfd, (char *)&ack, sizeof(int), 0);
                        printf("filename = %s\n", name);
                        sprintf(filepath, "./data/%s",name);
                        FILE *fp = fopen(filepath, "w");
                        if (fp == NULL) {
                            perror("fopen()");
                            exit(1);
                        }
                        while (1) {
                            ssize_t tmp = recv(sockfd, buff, sizeof(buff), 0);
                            if (tmp <= 0) {
                                perror("recv");
                                exit(1);
                            }
                            fwrite(buff,1,tmp,fp);
                            sum += tmp;
                            if (sum >= size) {
                                printf("File transfer finished!\n");
                                break;
                            }
                        }
                        fclose(fp);
                    }else if (flag & RECV) {
                        printf("Client want recv data...\n");
                    }else if (flag & NORMAL){
                        recv(sockfd, buff, sizeof(buff), 0);
                        printf("<%s> : %s\n", users[sockfd].name, buff);
                    }
                }
                
            }
        } 
    }
}
(1);
                   

/*************************************************************************
	> File Name: 1.server.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 26 Jan 2021 08:51:02 PM CST
 ************************************************************************/

#include "head.h"

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
    while (1) {
        if ((sockfd = accept(server_listen, NULL, NULL)) < 0) {
            perror("accept");
            exit(1);
        }
        printf("Something is online");
        pid_t pid;
        if ((pid = fork()) < 0) {
            exit(1);
        }
        if (pid == 0) {
            while (1) {
                char buff[512] = {0};
                recv(sockfd, buff, sizeof(buff), 0);
                printf("recv : %s\n", buff);
            }
        }
    }
}

/*************************************************************************
	> File Name: 2.server.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Sun 14 Mar 2021 11:06:17 AM CST
 ************************************************************************/

#include "head.h"


int main (int argc, char **argv) {

    if (argc < 2 && argc > 2) {
        printf("Usage : ./server port\n");
        exit(1);
    }
    int listen, sockfd;
    int port = atoi(argv[1]);
    if ((listen = socket_create(port)) < 0) {
        perror("socket_create\n");
        exit(1);
    }
    if ((sockfd = accept(listen, NULL, NULL)) < 0) {
        perror("accept");
        exit(1);
    }
    int size;
    int ack = 1;
    char name[50] = {0}, path[50] = {0}, buffer[1024] = {0};
    recv(sockfd, &name, sizeof(name), 0);
    send(sockfd, &ack, sizeof(int), 0);
    printf("file name = %s\n",name);
    recv(sockfd, &size, sizeof(int), 0);
    printf("file size = %d\n", size);
    sprintf(path,"../data/%s",name);
    printf("data path = %s\n", name);
    int fd = open(path, O_CREAT|O_WRONLY, S_IRWXU | S_IRWXG);
    while (1) {
        int rsize = recv(sockfd, &buffer, sizeof(buffer), 0);
        size -= rsize;
        if (rsize < 0) {
            perror("recv");
            close(sockfd);
            exit(1);
        }
        write(fd, &buffer, rsize);
        if (size == 0) {
            printf("recv success!\n");
            close(sockfd);
            break;
        }
    }
    return 0;
}

/*************************************************************************
	> File Name: common.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Sat 13 Mar 2021 10:34:10 AM CST
 ************************************************************************/

#include "head.h"

int make_nonblock(int fd) {
    int flag;
    if (( flag = fcntl(fd, F_GETFL) ) < 0) {
        perror("fcntl");
        return -1;
    }
    return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

int make_block(int fd) {
    int flag;
    if (( flag = fcntl(fd, F_GETFL) ) < 0) {
        perror("fcntl");
        return -1;
    }
    flag &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL,flag);
}


int socket_connect(char *ip, int port) {
    int sockfd;
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    socklen_t len = sizeof(addr);
    if ((connect(sockfd,(struct sockaddr *)&addr, len)) < 0) {
        return -1;
    } 
    return sockfd;
}


int socket_create(int port) {
    int sockfd;
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    socklen_t len = sizeof(addr);
    if (bind(sockfd, (struct sockaddr *)&addr, len) < 0) {
        return -1; 
    }
    if (listen(sockfd, 20) < 0 ) {
        return -1;
    }
    return sockfd;

}

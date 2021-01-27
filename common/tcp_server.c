/*************************************************************************
	> File Name: tcp_server.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 26 Jan 2021 08:38:10 PM CST
 ************************************************************************/
#include "head.h"

int socket_create(int port) {
    int sockfd;
    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    if (listen(sockfd,20) < 0) {
        return -1;
    }
    return sockfd;
}


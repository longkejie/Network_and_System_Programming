/*************************************************************************
	> File Name: common.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 07 Jan 2021 02:58:13 PM CST
 ************************************************************************/

#include "head.h" 

int make_nonblock(int fd) {
    int flag;
    if ((flag = fcntl(fd,F_GETFL)) < 0) {
        return -1;
    }
    return fcntl(fd,F_SETFL,(flag | O_NONBLOCK) );
}

int make_block(int fd) {
    int flag;
    if ((flag = fcntl(fd,F_GETFL)) < 0) {
        return -1;
    }
    return fcntl(fd,F_SETFL,(flag & ~O_NONBLOCK) );
}


int recv_str_nonblock(int sockfd, char *buff, long size, int timeout) {
    fd_set rdfds;
    struct timeval tv;
    FD_ZERO(&rdfds);
    FD_SET(sockfd, &rdfds);
    tv.tv_sec = 0;
    tv.tv_usec = timeout;
    int ret = select(sockfd + 1, &rdfds, NULL, NULL, &tv);
    if (ret <= 0) {
        return -1;
    }
    int rsize = recv(sockfd, buff, size, 0);
    if (rsize <= 0) {
        return -1;
    }
    return 0;
}

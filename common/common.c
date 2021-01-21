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

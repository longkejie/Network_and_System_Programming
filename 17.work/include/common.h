/*************************************************************************
	> File Name: common.h
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Sat 13 Mar 2021 10:30:04 AM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

int make_nonblock(int fd);
int make_block(int fd); 

int socket_connect(char *,int);

int socket_create(int port);

#endif

/*************************************************************************
	> File Name: common.h
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 07 Jan 2021 02:57:27 PM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

int make_nonblock(int);

int make_block(int);

int recv_str_nonblock(int sockfd, char *buff, long size, int timeout);

#endif

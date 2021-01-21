/*************************************************************************
	> File Name: chat.h
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 21 Jan 2021 06:37:11 PM CST
 ************************************************************************/

#ifndef _CHAT_H
#define _CHAT_H
struct Msg {
    char name[20];
    char msg[1024];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
#endif

/*************************************************************************
	> File Name: 1.thread.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 21 Jan 2021 07:45:18 PM CST
 ************************************************************************/

#include "head.h"

struct MyArg {
    char name[20];
    int age;
};


void *print(void *arg) {
    struct MyArg *in;
    in = (struct MyArg *)arg;
    printf("In Thread!\n");
    printf("%s is %d years old\n",in->name, in->age);
    printf("lkj\n"); 
}

int main () {
    struct MyArg arg;
    pthread_t thread;
    int age = 20;
    arg.age = age;
    char name[20];
    strcpy(arg.name, "longkejie");
    pthread_create(&thread,NULL,print,&arg);
    
    usleep(1);



    return 0;
}

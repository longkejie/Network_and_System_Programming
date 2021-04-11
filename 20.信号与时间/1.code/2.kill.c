/*************************************************************************
	> File Name: 2.kill.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 23 Mar 2021 10:43:39 PM CST
 ************************************************************************/

#include "head.h"
void print () {
    printf("\n");
}

int main () {
    signal(SIGINT, SIG_DFL);
    int age;
    scanf("%d",&age);
    printf("longkejie is %d years old!\n", age);
    return 0;
}

/*************************************************************************
	> File Name: 3.fifo.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Fri 19 Mar 2021 09:41:22 PM CST
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>

int main () {

    if (mkfifo("fifo",0666) < 0) {
        perror("fifo");
        exit(1);
    }
    
    return 0;
}

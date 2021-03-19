/*************************************************************************
	> File Name: test.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 16 Mar 2021 10:54:22 PM CST
 ************************************************************************/

#include "head.h"
int errno ;
int main () {

    FILE* fp = my_popen("cat test.c","r");
    char buff[1024] = {0};
    while (fread(&buff, 1, 1024, fp) != 0) {
        printf("%s",buff);
    }

    my_pclose(fp);
    return 0;
}

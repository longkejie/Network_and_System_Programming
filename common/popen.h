/*************************************************************************
	> File Name: popen.h
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 16 Mar 2021 07:55:20 PM CST
 ************************************************************************/

#ifndef _POPEN_H
#define _POPEN_H
FILE *my_popen(const char *cmd, const char *type);
int my_pclose(FILE *fp);
#endif

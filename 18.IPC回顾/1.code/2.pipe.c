/*************************************************************************
	> File Name: 2.pipe.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Fri 19 Mar 2021 07:44:53 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>

int main (int argc, char **argv) {
    int pipfd[2];
    char buff;
    pid_t pid;
    if (pipe(pipfd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(pipfd[1]);

        while (read(pipfd[0], &buff, 1) > 0) {
            write(STDOUT_FILENO, &buff, 1);
        }

        write(STDOUT_FILENO, "\n", 1);
        close(pipfd[0]);
        _exit(EXIT_SUCCESS);
    }else {
        close(pipfd[0]);

        write(pipfd[1], argv[1], strlen(argv[1]));
        close(pipfd[1]);
        wait(NULL);
    }
    

    return 0;
}

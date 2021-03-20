/*************************************************************************
	> File Name: 1.popen.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 16 Mar 2021 07:50:28 PM CST
 ************************************************************************/

#include "head.h"

static pid_t *childpid = NULL;
static int maxsize = 0;

FILE *my_popen(const char *cmd, const char *type) {    
    int pfd[2];
    FILE *fp;
    pid_t pid;
    if ((type[0] != 'r' && type[0] != 'w') || (type[1] != '\0')) {
        //errno = EINVAL;
        return NULL;
    }
    if (childpid == NULL) {
        maxsize = sysconf(_SC_OPEN_MAX);
    }
    if ((childpid = (pid_t *)calloc(maxsize, sizeof(pid_t))) == NULL) {
        return NULL;
    }
    if (pipe(pfd) < 0) {
        return NULL;
    }
    if ((pid = fork()) < 0) {
        return NULL;
    }
    if (pid == 0) {
        if (type[0] == 'r') {
            close(pfd[0]);
            if (pfd[1] != STDOUT_FILENO) {
                dup2(pfd[1], STDOUT_FILENO); 
                close(pfd[1]);
            }
        }else {
            close(pfd[1]);
            if (pfd[0] != STDIN_FILENO) {
                dup2(pfd[0], STDIN_FILENO);
                close(pfd[0]);
            }
        }
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        exit(0);
    }

    if (type[0] == 'r') {
        close(pfd[1]);
        if ((fp = fdopen(pfd[0], type))== NULL) {
            return NULL;
        }
    }else {
        close(pfd[0]);
        if ((fp = fdopen(pfd[1], type)) == NULL) {
            return NULL;
        }
    }
    childpid[fileno(fp)] = pid;
    //pipe();
    //fork();
    //shell
    return fp;
}


int my_pclose(FILE *fp) {
    int status, fd;
    fd = fileno(fp);
    pid_t pid = childpid[fd];
    if (pid == 0) {
        //errno = EINVAL;
        return -1;
    }
    fflush(fp);
    close(fd);
    waitpid(pid, &status, 0);
    return status;
}



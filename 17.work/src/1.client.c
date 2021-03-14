/*************************************************************************
	> File Name: 1.client.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Sat 13 Mar 2021 12:09:34 PM CST
 ************************************************************************/

#include "head.h"
/*struct File {
    char name[50];
    uint64_t size;
    char buff[4096];
};*/

void get_name(char *name, char *filepath) {
    char * p = strrchr(filepath, '/');
    if (p == NULL) {
        strcpy(name, filepath);
        return ;
    }
    strcpy(name, p + 1);
    return;
}


int main (int argc, char **argv) {
    if (argc < 3 || argc > 3) {
        printf("Usage : checkme ip port");
        exit(1);
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int sockfd;
    char filepath[50] = {0}, name[50] = {0};
    if ((sockfd = socket_connect(ip, port)) < 0) {
        perror("socket_connect");
        exit(1);
    }
    printf("connect success ! please input your filepath\n");
    scanf("%s",filepath);
    int fd = open(filepath, O_RDONLY);
    int size = lseek(fd, 0, SEEK_END);
    int ack = 0;
    printf("%d\n",size);
    lseek(fd, 0, SEEK_SET);
    get_name(name, filepath);
    printf("name = %s\n", name);
    send(sockfd, &name, strlen(name), 0);
    recv(sockfd, &ack, sizeof(int), 0);
    send(sockfd, &size, sizeof(int), 0);
    char buff[1024] = {0};
    int sum  = 0;
    while (ack && 1) {
        int ssize = read(fd, (void *)&buff, sizeof(buff));
        sum += ssize;
        if (ssize < 0) {
            perror("read");
            close(sockfd);
            exit(1);
        }
        send(sockfd, &buff, ssize, 0);
        if (sum == size) {
            printf("send file success!\n");
            close(sockfd);
            break;
        }
    }
    return 0;
}

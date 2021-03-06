/*************************************************************************
	> File Name: 1.client.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 26 Jan 2021 08:57:06 PM CST
 ************************************************************************/

#include "head.h"
int sockfd;
void logout(int signum) {
    close(sockfd);
    printf("Bye bye\n");
}

int main (int argc, char **argv) {
    int  port, sock_listen;
    char buff[512] = {0}, ip[20] = {0};
    if (argc != 3) {
        fprintf(stderr, "Usage : %s ip port\n",argv[0]);
        exit(1);
    }
    strcpy(ip, argv[1]);
    port = atoi(argv[2]);
    signal(SIGINT, logout);
    if ((sockfd = socket_connect(ip,port)) < 0) {
        perror("socket_connect");
        exit(1);
    }
    
    while (1) {
        printf("<input> : ");
        scanf("%[^\n]s",buff);
        getchar();
        if (!strlen(buff)) continue;
        printf("sending %s...\n",buff);
        send(sockfd,buff,strlen(buff),0);
        bzero(buff,sizeof(buff));
        recv(sockfd, buff,sizeof(buff),0);
        printf("<Sever> : %s\n",buff);
        bzero(buff,sizeof(buff));
    }

}

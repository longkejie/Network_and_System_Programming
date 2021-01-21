/*************************************************************************
	> File Name: ls.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Wed 13 Jan 2021 03:14:49 PM CST
 ************************************************************************/
#include<sys/ioctl.h>
#include<math.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>
#include<grp.h>
#include<sys/stat.h>
#include<pwd.h>
#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
int flag;
int max_gid1;
int max_uid1;
int max_gid;
int max_uid;
int max_digit;
int max_link;
char name[10000][10000];
int cnt = 0;
int max_size;
int max_digit1;
int num = 0;
char path[10000][10000];

int terminalWidth;


int get_digit(int x) {
    return ((int)fabs(log10(x)) + 1);
}

void getwidth() {
    struct winsize wbuf; 
    terminalWidth = 80;
    if (isatty(STDOUT_FILENO)) {
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &wbuf) == -1 || wbuf.ws_col == 0 ){
            char *tp;
            if (tp = getenv("COLUMS")) {
                terminalWidth = atoi(tp);
            }
        }else {
            terminalWidth = wbuf.ws_col;
        }
    }
}

#define PRINT(a,b) printf(a,b)
#define OUTPUT(s,b) PRINT(s,b) 

int cmp(const void *a, const void *b) {
    return (strcasecmp((char *)a, (char *)b) > 0 ? 1 : 0);
}
void get_opt(int argc,char **argv) {
    int opt; 
    while ((opt = getopt(argc, argv, "al")) != -1) {
        switch(opt) {
            case 'a':
                flag |= 1;
                break;
            case 'l':
                flag |= 2;
                break;
        }
    }
}
void is_type(const struct stat *buf) {
    mode_t m = buf->st_mode; 
    if (S_ISREG(m)) {
       printf("-");
    }else if (S_ISDIR(m)) {
        printf("d");
    }else if (S_ISCHR(m)) {
        printf("c");
    }else if (S_ISBLK(m)) {
        printf("b");
    }else if (S_ISFIFO(m)) {
        printf("p");
    }else if (S_ISLNK(m)) {
        printf("l");
    }else if (S_ISSOCK(m)) {
        printf("s");
    }
}

void is_authority(const struct stat *buf) {
    mode_t m = buf->st_mode;
    printf("%c",m & S_IRUSR?'r':'-');
    printf("%c",m & S_IWUSR?'w':'-');
    printf("%c",m & S_IXUSR?'x':'-');
    printf("%c",m & S_IRGRP?'r':'-');
    printf("%c",m & S_IWGRP?'w':'-');
    printf("%c",m & S_IXGRP?'x':'-');
    printf("%c",m & S_IROTH?'r':'-');
    printf("%c",m & S_IWOTH?'w':'-');
    printf("%c",m & S_IXOTH?'x':'-'); 
}
void is_user_group(uid_t uid,gid_t gid) {
    char str[100] = {' ', '%', '-', '5','s','\0'};
    struct passwd * user = getpwuid(uid);
    str[3] = max_uid1 + 48;
    OUTPUT(str,user->pw_name);
    str[3] = max_gid1 + 48;
    struct group * grp = getgrgid(gid); 
    OUTPUT(str,grp->gr_name);
    
}

int get_max(const char *name, int *n, int *m,int *x, int *y) {
    struct stat *buf = (struct stat *)malloc(sizeof(struct stat));
    lstat(name,buf);
    if (buf->st_nlink > *n) { 
        *n = buf->st_nlink;
        if(get_digit(*n) > max_digit) {
            max_digit = get_digit(*n);
        }
    }
    if (buf ->st_size > *m) {
        *m = buf->st_size;
        max_digit1 = get_digit(*m);
    }
    struct passwd *user = getpwuid(buf->st_uid);
    struct group * grp = getgrgid(buf->st_gid);
    if (strlen(user->pw_name) > *x) {
        *x = strlen(user->pw_name);
        max_uid1 = *x;
    }
    if (strlen(grp->gr_name) > *y) {
        *y = strlen(grp->gr_name);
        max_gid1 = *y;
    }
    free(buf);
}

void do_al(const char *name) {
    struct stat *buf = (struct stat *)malloc(sizeof(struct stat));
    lstat(name, buf);
    is_type(buf);
    is_authority(buf);
    char str[100] = {' ','%','1','l','d','\0'};
    str[2] = max_digit + 48;
    OUTPUT(str,buf->st_nlink);
    uid_t uid = buf->st_uid;
    gid_t gid = buf->st_gid;
    is_user_group(uid,gid);
    off_t size = buf->st_size;
    str[2] = max_digit1 + 48;
    OUTPUT(str,size);
    printf( " %.12s ", 4+ctime(&buf->st_mtime) );
    printf("%s\n",name);
    
}

void outstr() {
    getwidth();
    char file[1000][1000];
    int max_len = 0;
    int num_in_row = 0;
    int num_in_col = 0;
    int sum = 0;
    for (int i = 0 ; i < cnt; ++i) {
        if (flag == 0 && (name[i][0] == '.')) continue;
        strcpy(file[sum++],name[i]);
        max_len =  max_len > strlen(name[i]) ? max_len : strlen(name[i]);
    }
    num_in_row = terminalWidth / max_len;
    if (sum % num_in_row == 0 ){
        num_in_col = sum / num_in_row;
    }else {
        num_in_col = sum / num_in_row + 1;
    }
    int k = 0;
    for (int i = 0; i < num_in_col; ++i) {
        for (int j = 0; j < num_in_row && k < sum; ++j) {
            printf("%-*s  ", max_len, file[i + j * num_in_col]);
            k++;
        }
        printf("\n");
    }
}

int main (int argc, char **argv) {
    int opt;
    for (int i = 1 ; i < argc; ++i ) {
        if (argv[i][0] != '-') {
            strcpy(path[num++],argv[i]);
        }
    }
    if (num == 0) strncpy(path[num++],".",2);
    qsort(path, num, 10000, cmp);
    get_opt(argc,argv);
    for (int i = 0; i < num; ++i) {
        cnt = 0;
        memset(name,0,sizeof(name));
        DIR *dir ;
        if ( (dir = opendir(path[i])) == NULL) {
            perror("opendir");  
            continue;
        }//读取相应的目录
        if (num > 1) printf("\033[32m%s:\033[0m\n",path[i]);
        chdir(path[i]);
        struct dirent *file = (struct dirent *)malloc(sizeof(struct dirent));
        while ((file = readdir(dir)) != NULL) {
            strncpy(name[cnt++],file->d_name,10000);
        }//循环读取目录中的相应文件
        qsort(name, cnt, 10000, cmp);
        for (int j = 0; j < cnt; ++j) {
            if (((flag & 1) == 0) && (name[j][0] == '.')) continue; 
            get_max(name[j],&max_link, &max_size,&max_uid,&max_gid);
        }
        if (flag == 0 || flag == 1) {
            outstr();
        }
        for (int j = 0; j < cnt; ++j) {
            if (((flag & 1) == 0) && (name[j][0] == '.')) continue;
            switch(flag) {
                case 2:{
                    do_al(name[j]);
                }break;
                case 3:{
                    do_al(name[j]);
                }break;
            } 
        }
        if (i != num - 1) printf("\n");
    }
    return 0;
}

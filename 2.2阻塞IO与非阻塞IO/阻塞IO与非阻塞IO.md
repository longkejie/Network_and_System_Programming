# 2021-1-7

## 1.大纲

![image-20210107140940056](https://gitee.com/long_kejie/image/raw/master/image-20210107140940056.png)

## 2.非阻塞和阻塞？

对于文件就有阻塞和非阻塞区部！

### 2.1写文件的步骤

- 打开文件，获取文件描述符（open)
- 调用write系统调用，用户态->内核态（把我的buf，给内核）
- 内核将我给它的buf，写入磁盘
  - 调度底层IO设备(设备管理).
  - 根据inode和block找到磁盘的实际位置
  - 磁头移动，磁盘盘片转动，移动到相应的位置，磁头和磁盘之间隔一个极小的距离，通过相应的写操作，将数据写进去。
- 在写入磁盘前，会先写入缓存区
  - 对于普通用户，给内核就返回一个结果（非阻塞），对用户来说是透明的
  - 如果还需等待内核是否成功写入磁盘的结果，这种是阻塞IO。

### 2.1相关函数

```c
int fcntl(int fd, int cmd, .../*arg*/)//可以用来设置，文件标志。
   	int flag = fcntl(fd,F_GETFL).//获得文件标记
  	fcntl(fd,F_SETFL,flag).//设置文件标记。
    //为了避免设置的值覆盖以前的值，应该先保存以前的值，然后做或操作。
```

### 2.2IO多路复用（select函数）

一个IO处理多个事情

```c
int select (int nfds,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,struct timeval *timeout);

//nfds个数，最大的文件描述符加1
//writefds关注写操作
//readfds关注读操作集和
//exceptfds关注异常操作集和
//fd_set 文件描述符集和
//timeout 时间期限，等多长时间,0就是轮询，NULL是无期限等待。
//RETURN VALUE:被包含在三个文件描述符集和中所有就绪的IO数量。返回0,时间超时，没有事件发生。-1表示出错了

如下函数
void FD_CLR(int fd, fd_set *set);//将fd从set中清除
int FD_ISSET(int fd, fd_set *set);//判断fd是否在set集和中
void FD_SET(int fd,fd_set *set);//将fd加入set中
void FD_ZERO(fd_set *set);//将集和set清空
```

三种情况会返回

- 超时
- 中断
- 有事件就绪

还有如下类似函数

$poll$

$epoll$

epoll最高级
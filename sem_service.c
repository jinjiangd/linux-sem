#include "sem.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#define SERV_PORT 1113
#define LISTENQ  32
#define MAXLINE 1024

/***连接处理函数***/
void str_echo(int fd);

int main(int argc, char *argv[])
{
  int listenfd,connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;
  if((listenfd = socket(AF_INET, SOCK_STREAM,0))==-1)
  {
     fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
     exit(1);
  }
  /* 服务器端填充 sockaddr结构*/
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);
  /* 绑定listenfd描述符  */
  if(bind(listenfd,(struct sockaddr*)(&servaddr),sizeof(struct sockaddr))==-1)
  {
    fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
    exit(1);
   }
   /* 监听listenfd描述符*/
  if(listen(listenfd,5)==-1)
  {
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
        exit(1);
  }
  while (1)
  {
    clilen = sizeof(cliaddr);
    /* 服务器阻塞,直到客户程序建立连接  */
    if((connfd=accept(listenfd,(struct sockaddr*)(&cliaddr),&clilen))==-1)
    {
        fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
        exit(1);
    }
    //有客户端建立了连接后
    if ( (childpid = fork()) == 0)
    { /*子进程*/
     close(listenfd);    /* 关闭监听套接字*/
     str_echo(connfd);   /*处理该客户端的请求*/
     exit (0);
    }
    close(connfd);/*父进程关闭连接套接字，继续等待其他连接的到来*/
  }
}

void str_echo(int sockfd)
{
    ssize_t n;
    char  buf[MAXLINE];
    again:
      while ( (n = read(sockfd, buf, MAXLINE)) > 0)
          write(sockfd, buf, n);
      if (n < 0 && errno == EINTR)//被中断，重入
          goto again;
      else if (n < 0)
      {//出错
        fprintf(stderr,"read error:%s\n\a",strerror(errno));
        exit(1);
      }
}

#ifndef _SEM_H_
#define _SEM_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define PATHNAME "."  //ftok中生成key值
#define PROJ_ID (66)  //ftok中配合PATHNAME生成唯一key值

union semun
{
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};

int create_sems(int nums); //创建含有nums个信号量的集合
int get_sems(); //获取信号量

//初始化semid对应的信号量集中编号为which的信号量值为value
int init_sems(int semid, int which, int value);

int destroy_sems(int semid); //释放该信号量集

int sem_p(int semid, int which); //表示分配，信号量值-1
int sem_v(int semid, int which); //表示释放，信号量值+1

#endif

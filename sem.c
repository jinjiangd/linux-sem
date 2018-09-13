#include "sem.h"

//创建信号量并获取信号量公用函数
static int comm_sem(int nums, int semflag)
{
  key_t key;
  int semid;
  key = ftok(PATHNAME, PROJ_ID);
  if(key < 0)
  {
    perror("ftok");
    return -1;
  }

  semid = semget(key, nums, semflag);
  if(semid < 0)
  {
    perror("semget");
    return -1;
  }
  return semid;
}

//创建含有nums个信号量的集合
int create_sems(int nums)
{
  return comm_sem(nums, IPC_CREAT|IPC_EXCL|0666);
}

//获取信号量
int get_sems()
{
  return comm_sem(0, IPC_CREAT);
}

//初始化semid对应的信号量集中编号为which的信号量值为value
int init_sems(int semid, int which, int value)
{
  union semun _semun;
  int ret;
  _semun.val = value;

  ret = semctl(semid, which, SETVAL, _semun);
  if(ret < 0)
  {
    perror("init_sems");
    return -1;
  }

  return 0;
}

//释放该信号量集
int destroy_sems(int semid)
{
  int ret;
  ret = semctl(semid, 0, IPC_RMID, NULL);
  if(ret < 0)
  {
    perror("destroy_sems");
    return -1;
  }
  return 0;
}

static int comm_sem_op(int semid, int which, int op)
{
  struct sembuf _sembuf;
  _sembuf.sem_num = which;
  _sembuf.sem_op = op;
  _sembuf.sem_flg = 0; //IPC_NOWAIT  SEM_UNDO
  return semop(semid, &_sembuf, 1);
}

//表示分配，信号量值-1
int sem_p(int semid, int which)
{
  return comm_sem_op(semid, which, -1);
}

//表示释放，信号量值+1
int sem_v(int semid, int which)
{
  return comm_sem_op(semid, which, 1);
}

#include "sem.h"

int main(int argc, char* argv[])
{
  int semid;
  pid_t pid;
  int sem_id;
  int index = 0;
  //创建一个包含 1个信号量的信号集
  semid = create_sems(1);
  //初始化编号为 0的信号量 值为1
  init_sems(semid, 0, 1);

  pid = fork(); //创建子进程
  if(pid < 0)
  {
    perror("fork");
    return -1;
  }
  else if(pid == 0)
  {
    //子进程
    //获取信号量
    sem_id = get_sems();
    while(1)
    {
      //对该信号量集中的0号信号 做sem_p操作
      if(index >= 5)
        break;
      sem_p(sem_id, 0);
      printf("你");
      sleep(1);
      printf("好:");
      fflush(stdout);
      sleep(1);
      ++index;
      sem_v(sem_id, 0);
    }
  }
  else
  {
    //父进程
    while(1)
    {
      if(index >= 5)
        break;
      sem_p(semid, 0);
      printf("在");
      sleep(1);
      printf("吗？");
      fflush(stdout);
      sleep(1);
      ++index;
      sem_v(semid, 0);
    }
    wait(NULL);
  }
  destroy_sems(semid);
  return 0;
}

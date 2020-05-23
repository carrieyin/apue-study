#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include <sys/wait.h>
//指定信号量的KEY
#define IPC_KEY 0x12345678 
 
int main()
{
    //1.创建共享内存
    int shmid = shmget(IPC_KEY,512,IPC_CREAT|0777);
    if(shmid<0)
    {
        perror("shmget error");
        exit(1);
    }
    printf("success get sharememory\n");
    //2.将共享内存连接到虚拟内存空间
    void *shm_ptr = shmat(shmid,0,0);   //设置addr为0，让内核选择虚拟地址空间中的第一个可用地址
    if(shm_ptr == (void *)-1)
    {
        perror("shmat error");
        exit(2);
    }
    //3.往共享内存写入数据
    while(1)
    {
        printf(">");
        fflush(stdout);
        scanf("%s",(char *)shm_ptr);
    }
    //4.解除进程与共享内存连接
    shmdt(shm_ptr);
    //5.删除共享内存
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
} 
    
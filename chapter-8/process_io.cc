#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int globvar = 6;
char buf[] = "a write to stdout\n";

int main()
{
    int var;
    pid_t pid;

    var = 88;

    if(write(STDOUT_FILENO, buf, sizeof(buf) -1 != (sizeof(buf) - 1)))
    {

    }

    printf("before fork\n");
    //标准输入输出是行缓冲的，因此上面的语句会立即输出
    //如果输入重定向到文件中，此处不强制冲洗缓冲区父子进程会各输出一次下面的语句，进程终止，文件描述符在进程关闭时，写入文件

    //fflush(stdout);
    if((pid = fork()) < 0)
    {

    }
    else if (pid > 0)
    {
        globvar++;
        var++;
    }
    else
    {
        sleep(2);
    }
    
    printf("pid = %ld, glob = %d, var = %d \n", (long)getpid(), globvar, var);
    exit(0);

}
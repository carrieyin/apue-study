#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("test 缓冲");
    if(fork() == 0)
    {
        if(execl("/usr/bin/less", "less", NULL)<0)
            perror("Err on execl");
    }
}
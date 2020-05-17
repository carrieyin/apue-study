#include <unistd.h>
#include <stdio.h>

int main()
{
    if(fork() ==0)
    {
        if(execl("/usr/bin/less", "less", "executed by execl", NULL)<0)
            perror("Err on execl");
    }
}
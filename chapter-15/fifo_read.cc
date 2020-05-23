#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

//与fifo_read配合使用
int main(int argc, char* argv[])
{
    int ret = mkfifo("my_fifo",0777);
    if (ret == -1)
    {
        printf("make fifo failed!\n");
        return 1;
    }

    char buf[256] = {0};
    int fd = open("my_fifo",O_RDONLY);
    read(fd,buf,256);
    printf("%s\n",buf);
    close(fd);
}
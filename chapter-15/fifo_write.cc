#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

//与fifo_read配合测试
int main()
{
    char buf[] = "I am write process\n";

    int fd = open("my_fifo", O_WRONLY);
    write(fd, buf, strlen(buf));
    close(fd);
    return 0;
}
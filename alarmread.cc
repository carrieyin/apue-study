#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
/*这个程序有两个问题，本意是要实现read时间的上限值
1)第一次调用alarm调用和read之间有一个竞争条件，如果内核在这期间是进程阻塞，且长度超过闹钟时间，则read可能永远阻塞
2)如果系统调用是自动重启的，则该设置不起作用
*/
#define MAXLINE 6092

static void sig_alrm(int signo)
{
    //do nothing ,just return to interrupt the read
}
int main()
{
    int n;
    char line[MAXLINE];
    if(signal(SIGALRM, sig_alrm) == SIG_ERR)
    {

    }

    alarm(10);
    if((n = read(STDIN_FILENO, line, MAXLINE))< 0)
    {
    }
    alarm(0);

    write(STDOUT_FILENO, line, n);
    exit(0);
}
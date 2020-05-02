#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//前提：此程序sig_int运行时间超过5s
//执行该程序后输入ctrl+c，程序收到SIGINT中断
//SIGALRM发生时，会中断SIGINT的处理，此时调用longjmp会导致提早终止sig_int信号处理程序，无法再继续执行
static jmp_buf env_alrm;
static void sig_alrm(int signo)
{
    longjmp(env_alrm, 1);
}

unsigned int sleep1(unsigned int seconds)
{
    if(signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        return seconds;
    }

    if(setjmp(env_alrm) == 0)
    {
        alarm(seconds);
        pause();
    }
}

static void sig_int(int signo)
{
    int i, j;
    volatile int k;

    printf("\nsig_int starting\n");

    for(int i = 0; i < 300000; i++)
    {
        for(int j = 0; j < 4000; j++)
        {
            k += i*j;
        }
    }

    printf("\nsig_int end\n");


}
int main(void)
{
    unsigned int unslept;
    unslept = sleep1(5);
    if(signal(SIGINT, sig_int) == SIG_ERR){
        printf("signal sigint error\n");
    }

    printf("sleep1 returned : %u\n", unslept);
    exit(0);
}

#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//ǰ�᣺�˳���sig_int����ʱ�䳬��5s
//ִ�иó��������ctrl+c�������յ�SIGINT�ж�
//SIGALRM����ʱ�����ж�SIGINT�Ĵ�����ʱ����longjmp�ᵼ��������ֹsig_int�źŴ�������޷��ټ���ִ��
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

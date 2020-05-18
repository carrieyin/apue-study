#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#define MAXLINE 4096

int main()
{
    char line[MAXLINE];
    FILE *fpin;

    if((fpin = popen("myuclc", "r")) == NULL)
    {
        return 0;
    }

    for(;;)
    {
        fputs("promt>", stdout);
        fflush(stdout);

        if(fgets(line, MAXLINE, fpin) == NULL)
        {
            break;
        }

        if(fputs(line, stdout) == EOF)
        {

        }
    }

    if(pclose(fpin) == -1)
    {

    }
    putchar('\n');
    exit(0);



}
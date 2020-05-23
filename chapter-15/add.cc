#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
using namespace std;
#define MAXLINE 4096

// demo
int main()
{
    int n, int1, int2;
    char line[MAXLINE];
   
    /*char s[] = "12 34";
    int a, b;
    sscanf(s, "%d%d", &a, &b);
    printf("%d---%d\n", a, b);*/
    string s;
    while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0)
    {
        line[n] = 0;
        //printf("n is %d \n",n);
        //char line[6] = "12 34";
        printf("string is %s \n", string(line).c_str());
        int temp = sscanf(line, "%d%d", &int1, &int2);
        //printf("%d\n", temp);
        if(temp == 2)
        {
            sprintf(line, "%d\n", int1 + int2);
            n = strlen(line);
            if(write(STDOUT_FILENO, line, n) != n)
            {
                perror("write error");
            }
        }
        else
        {
            if(write(STDOUT_FILENO, "invalid args\n", 13) != 13)
            {

            }
        }
        
    }

    exit(0);
    
}
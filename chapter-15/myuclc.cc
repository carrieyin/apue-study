#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

int main()
{
    int c;
    while ((c = getchar()) != EOF)
    {
        if(isupper(c))
        {
            c = tolower(c);
        }

        if(putchar(c) == EOF)
        {
            
        }

        if(c == '\n')
        {
            fflush(stdout);
        }
    }
    
}
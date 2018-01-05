#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

/**
 * ouputs initials of user name input * 
 */

int main(void)
{
    // get user name
    char* name = GetString();
    
    // prints first letter
    printf("%c", toupper(name[0]));
    for (int i = 1, n = strlen(name); i < n; i++)
    {
        int j;
        j = (int) name[i];
        if (j == 32)
        {
            printf("%c", toupper(name[i+1]));
        }
    }
    printf("\n");
}
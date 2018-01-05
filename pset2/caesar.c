#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

/**
 * caesar cipher crypto program
 * 
 */

int k = 0;
char Encrypt(char a);
int GetK(int n);

int main(int argc, string argv[])
{
    if (argc !=2)
    {
        printf("Please use only one and positive integer\n");
        return 1;
    }
    k = GetK(atoi(argv[1]));
       
    // get the phrase from user
    string phrase = GetString();
        
    //encrypt user input
    for (int i = 0, n = strlen(phrase); i < n; i++)
    {
        Encrypt(phrase[i]);
        printf("%c", Encrypt(phrase[i]));
    }
    printf("\n");
    return 0;
}


//mod26 function to calculate the key
int GetK(int n)
{
    while (n - 26 >= 0)
        {
            n = n - 26;
        }
    return n;
}


//Encrypt a character
char Encrypt(char a)
{
    
    if (isalpha(a) != 0)
    {
        int j;
        j = (int) a;
        
        //encrypt a lower character
        if (islower(a) != 0)
        {
            j = j - 97;
            j = (j + k) % 26;
            j = j + 97;
        }
        else
        //encrypt a uppper character    
            if (isupper(a) != 0)
            {
                j = j - 65;
                j = (j + k) % 26;
                j = j + 65;
            }    
        a = (char) j; 
    }
    return a;
}

#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include<stdbool.h>

/**
 * vigenere cipher crypto program
 * 
 */

int k = 0;
bool KeyAlphaChars(string s);
char Encrypt(char a);
int GetK(char ki);

int main(int argc, string argv[])
{
    if (argc == 1 || argc > 2 || (KeyAlphaChars(argv[1]) == false))  
    {
        printf("Please use one key without any non-alphabetical characters\n");
        return 1;
    }

    
    // get the phrase from user
    string phrase = GetString();
        
    //encrypt user input
    int l = 0;
    for (int i = 0, n = strlen(phrase); i < n; i++)
    {
        // calculate the k key
        
        int keyLenght = strlen(argv[1]);
        char keyChar;
        keyChar= argv[1][(i - l) % keyLenght];
        if (isalpha(phrase[i]))
            {
                GetK(keyChar);
                Encrypt(phrase[i]);
                printf("%c", Encrypt(phrase[i]));
            }        
        else
            {
                printf("%c", phrase[i]);
                l = l + 1;
            }
    }
    printf("\n");
    return 0; 
}
    
// Checks if user input uses only alphabetical char
bool KeyAlphaChars(string s)
{
    bool answer;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if ( isalpha( s[i] ) )
        {
            answer = true;
        }
        else answer = false;
    }
    return answer;
}
 
  
//mod26 function to calculate the key
int GetK(char ki)
{
    k = 0;
    if (islower(ki))
    {
        char a = 'a';
        while(a < ki)
        {
            a++;
            k++;
        }
    }
    if (isupper(ki))
    {
        char a = 'A';
        while (a < ki)
        {
            a++;
            k++;
        }
    }
    return k;
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

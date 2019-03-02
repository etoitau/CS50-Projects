#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <math.h>


char format(int num);

int main(int argc, string argv[])
{
    char targethash[13];
    strcpy(targethash, crypt("AzB", "50"));
    
    //printf("target is: %s\n", targethash);
    char guess[6];    
    //printf("initial guess: %s\n", guess);
    for (int i = 0; i < 52; i++) 
    {
        for (int j = 0; j < 53; j++)
        {
            if(j == 52)
            {
                guess[0] = format(i);
                guess[1] = (char)(0);
                string guesshash = crypt(guess, "50");
                if (strcmp(targethash, guesshash) == 0)
                {
                    printf("Success! it's: %s\n", guess);
                    return 0;
                }
            }
            else
            {
                 for(int k = 0; k < 53; k++)
                 {
                     if (k == 52)
                     {
                         guess[0] = format(i);
                         guess[1] = format(j);
                         guess[2] = (char)(0);
                         string guesshash = crypt(guess, "50");
                         if (strcmp(targethash, guesshash) == 0)
                         {
                             printf("Success! it's: %s\n", guess);
                         }
                     }
                     else
                     {
                         go deeper
                     }
                 
                    for (int m = 0; m < 53; m++)
                    {
                        if(m == 52)
                        {
                            guess[0] = format(i);
                            guess[1] = format(j);
                            guess[2] = (char)(0);
                            string guesshash = crypt(guess, "50");
                            if (strcmp(targethash, guesshash) == 0)
                            {
                                printf("Success! it's: %s\n", guess);
                            }
                        }
                        else
                        {
                            guess[0] = format(i);
                            guess[1] = format(j);
                            guess[2] = format(k);
                            guess[3] = (char)0;
                            string guesshash = crypt(guess, "50");
                            if (strcmp(targethash, guesshash) == 0)
                            {
                                printf("Success! it's: %s\n", guess);
                            }
                        }
                    }    
            }
        }
    }
    //printf("target is: %s\n", targethash);
}

char format(int num)
{
    char out;
    if (num < 26)
    {
        out = num + 97;
    }
    else
    {
        out = num + 39;
    }
    return out;
}

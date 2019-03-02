// CS50 Problem Set 2
// crack
// given a hashed password, figure out the key by brute force
//
// by: Kyle Chatman
// on: 190217
//
// assumes key is max 5 characters and only upper or lowercase letters

#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <math.h>

int inputchecks(int argc, string argv[]);
char format(int num);

int main(int argc, string argv[])
{
    // Check valid input
    if (inputchecks(argc, argv))
    {
        printf("Usage: %s hash\n", argv[0]);
        return 1;
    }
    
    // Get Salt
    char salt[3];
    salt[0] = argv[1][0];
    salt[1] = argv[1][1];
    salt[2] = 0;
        
    char guess[6];    
    //printf("initial guess: %s\n", guess);
    
    // Top loop, first character
    for (int i = 0; i < 52; i++) 
    {
        // Second loop, second character
        for (int j = 0; j < 53; j++)
        {
            // If no second character, go ahead and guess
            if (j == 52)
            {
                guess[0] = format(i);
                guess[1] = (char)(0);
                string guesshash = crypt(guess, salt);
                if (strcmp(argv[1], guesshash) == 0)
                {
                    printf("%s\n", guess);
                    return 0;
                }
            }
            // If there is a second character, look at third
            else
            {
                // Third loop, third character
                for (int k = 0; k < 53; k++)
                {
                    // If no second character, go ahead and guess
                    if (k == 52)
                    {
                        guess[0] = format(i);
                        guess[1] = format(j);
                        guess[2] = (char)(0);
                        string guesshash = crypt(guess, salt);
                        if (strcmp(argv[1], guesshash) == 0)
                        {
                            printf("%s\n", guess);
                            return 0;
                        }
                    }
                    // If there is a third character, look at fourth
                    else
                    {
                        // Fourth loop, fourth character
                        for (int l = 0; l < 53; l++)
                        {
                            // If no fourth character, go ahead and guess
                            if (l == 52)
                            {
                                guess[0] = format(i);
                                guess[1] = format(j);
                                guess[2] = format(k);
                                guess[3] = (char)(0);
                                string guesshash = crypt(guess, salt);
                                if (strcmp(argv[1], guesshash) == 0)
                                {
                                    printf("%s\n", guess);
                                    return 0;
                                }
                            }
                            // If there is a fourth character, look at five
                            else
                            {
                                // Fifth loop, last character
                                for (int m = 0; m < 53; m++)
                                {
                                    // If no fifth character, go ahead and guess
                                    if (m == 52)
                                    {
                                        guess[0] = format(i);
                                        guess[1] = format(j);
                                        guess[2] = format(k);
                                        guess[3] = format(l);
                                        guess[4] = (char)(0);
                                        string guesshash = crypt(guess, salt);
                                        if (strcmp(argv[1], guesshash) == 0)
                                        {
                                            printf("%s\n", guess);
                                            return 0;
                                        }
                                    }
                                    // If there is a fifth character, guess with full five
                                    else
                                    {
                                        guess[0] = format(i);
                                        guess[1] = format(j);
                                        guess[2] = format(k);
                                        guess[3] = format(l);
                                        guess[4] = format(m);
                                        guess[5] = (char)0;
                                        string guesshash = crypt(guess, salt);
                                        if (strcmp(argv[1], guesshash) == 0)
                                        {
                                            printf("%s\n", guess);
                                            return 0;
                                        }
                                    }
                                    // end loop 5
                                } 
                                // end else
                            }    
                            // end loop 4
                        }    
                        // end else
                    }    
                    // end loop 3
                } 
                // end else
            } 
            // end loop 2
        } 
        // end loop 1
    } 
    return 1;
}

int inputchecks(int argc, string argv[])
{
    // Check for null (this shouldn't be possible, but happened in my testing and crashed terminal when strlen was called)
    if (argv[1] == NULL)
    {
        return 1;
    }
    
    // Check only one argument entered
    if (argc != 2)
    {
        return 1;
    }   
    return 0;
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

#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <math.h>


int base10toAZ(long guess10);

char guess[6];

int main(int argc, string argv[])
{
        
    //limit = 53 + pow(53,2) + pow(53,3) + pow(53,4) + pow(52,5) + 1 = 388246253
    for (long guess10 = 1; guess10 < 200; guess10++) 
    {
        printf("%ld a\n", guess10);
        if (guess10 % 53 != 0)
        {
            printf("%ld b\n", guess10);
            base10toAZ(guess10);
            printf("%ld c\n", guess10);
            //printf("- %ld guess: %s\n", guess10, guess);
        }
        printf("%ld d\n", guess10);
//        if (guess10 % 1000000 == 0)
//        {
//            printf("# %ld guess: %s\n", guess10, guess);
//        }
    }
    printf("Done\n");
    printf("final guess: %s\n", guess);   
    
}

int base10toAZ(long guess10)
{
    const int base = 53;
    int place = 0;
    int digitholder[6];
    
    do 
    {
        //printf("start do loop guess: %s\n", guess);
        int digit;
        int rem = guess10 % base;
        if (rem < 27)
        {
            digit = (rem + 96);
        }
        else
        {
            digit = (rem + 38);
        }
        
        if (rem != 0)
        {
            place++;
            digitholder[place] = digit;
            printf("digitholder[%d]=%d\n", place, digit);
        }        
        guess10 = (guess10 - rem) / base;
    }
    while (guess10 != 0);
    
    for (int i = 0; i <= place; i++)
    {
        guess[place - i] = (char)digitholder[i];
    }
    
    
    return 0;
}

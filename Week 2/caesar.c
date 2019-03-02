// CS50 Problem Set 2
// caesar
// apply caesar cipher to given string
//
// by: Kyle Chatman
// on: 190215

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int inputchecks(int argc, string argv[]); // Checks for valid input
string cipher(string plain, int key); // Applies Caesar cipher to string

int main(int argc, string argv[])
{
    // Run input checks to be sure input is valid integer only
    int control = inputchecks(argc, argv);
    
    if (control != 0)
    {
        printf("Usage: %s key\n", argv[0]);
        return control;
    }
    // if passes, okay to convert input to int
    int key = atoi(argv[1]);
    
    // Get plaintext
    string plain = get_string("plaintext:  ");
    
    // Apply cipher
    string ciphertext = plain;
    ciphertext = cipher(plain, key);
    
    printf("ciphertext: %s\n", ciphertext);
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
        return 2;
    }
     
    int arglen = strlen(argv[1]);
    
    // Check all characters of input string are numbers
    for (int i = 0; i < arglen; i++)
    {
        if(isdigit(argv[1][i]) == 0)
        {
            return 3;
        }
    }
    
    int key = atoi(argv[1]);
    
    // Check for int overflow
    char keystr[arglen+1];
    snprintf(keystr, arglen+1, "%i", key);
    if (strcmp(keystr, argv[1]) != 0)
    {
        return 4;
    }
     
    return 0;
}

string cipher(string plain, int key)
{
    int length = strlen(plain);
    string ciphertext = plain;
    
    // For each character in string apply Caeasr cipher if a to z character
    for (int i = 0; i < length; i++)
    {
        int ascii = (int) plain[i];
        // If lowercase
        if (ascii > 96 && ascii < 123)
        {
            ciphertext[i] = (char) ((ascii - 96 + key) % 26 + 96);
        }
        // If uppercase
        else if (ascii > 64 && ascii < 91)
        {
            ciphertext[i] = (char) ((ascii - 64 + key) % 26 + 64);
        }
        // If some other character
        else
        {
            ciphertext[i] = plain[i];
        }
    }
    return ciphertext;
}

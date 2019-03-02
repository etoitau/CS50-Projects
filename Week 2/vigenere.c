// CS50 Problem Set 2
// vigenere
// apply vigenere cypher to given string
//
// by: Kyle Chatman
// on: 190216

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int inputchecks(int argc, string argv[]); // Checks for valid input
string cipher(string plain, string key); // Applies Caesar cipher to string

int main(int argc, string argv[])
{
    // Run input checks to be sure input is valid integer only
    int control = inputchecks(argc, argv);
    
    if (control != 0)
    {
        printf("Usage: %s keyword\n", argv[0]);
        return control;
    }
        
    // Get plaintext
    string plain = get_string("plaintext:  ");
    
    // Apply cipher
    string ciphertext = plain;
    ciphertext = cipher(plain, argv[1]);
    
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
    
    // Check all characters of input string are letters
    for (int i = 0; i < arglen; i++)
    {
        if(isalpha(argv[1][i]) == 0)
        {
            return 3;
        }
    }
    
         
    return 0;
}

string cipher(string plain, string key)
{
    int plainlength = strlen(plain);
    int keylength = strlen(key);
    
    string ciphertext = plain;
    int keyindex = 0;
    // For each character in string apply vigenere cipher if a to z character
    for (int i = 0; i < plainlength; i++)
    {
        int plainascii = (int) plain[i];
        int keyascii = (int) toupper(key[keyindex % keylength]);
        keyascii = keyascii - 65;
        // If lowercase
        if (plainascii > 96 && plainascii < 123)
        {
            ciphertext[i] = (char) ((plainascii - 97 + keyascii) % 26 + 97);
            keyindex++;
        }
        // If uppercase
        else if (plainascii > 64 && plainascii < 91)
        {
            ciphertext[i] = (char) ((plainascii - 65 + keyascii) % 26 + 65);
            keyindex++;
        }
        // If some other character
        else
        {
            ciphertext[i] = plain[i];
        }
        
    }
    return ciphertext;
}

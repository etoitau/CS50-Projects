// CS50 Problem Set 2
// credit
// runs some checks to see if a given number looks like a valid credit card number
//
// by: Kyle Chatman
// on: 190208

#include <cs50.h>
#include <stdio.h>

unsigned long long power(int base, int exp);
int get_length(unsigned long long number);
int get_digit(unsigned long long number, int place);
bool luhn(unsigned long long creditnumber, int length);
string type_card(unsigned long long creditnumber);

int main(void)
{
    bool valid = 1;
    unsigned long long creditnumber = get_long("Please enter credit card number.\n");
    // Check length
    int length = get_length(creditnumber);
    if ( length > 16 )
        valid = 0;
    if (length < 13 )
        valid = 0;
    // Do Luhn's Algorithm check
    if (!luhn(creditnumber, length))
        valid = 0;
    // Check for card types
    string type = type_card(creditnumber);
    // Output findings
    if( valid )
        printf("%s\n", type);
    else
        printf("INVALID\n");
}

// Make exponent function 
unsigned long long power(int base, int exp) 
{
    unsigned long long result = 1;
    for (int i = 0; i < exp; i++)
        result *= base;
    return result;
 }

// Report length of number
int get_length(unsigned long long number)
{
    int i = 0;
    while (number / power(10, i) >= 1) 
    {
        i++;
    }
    return i;
}

// Return the digit "place" over from rightmost in "number"
int get_digit(unsigned long long number, int place)
{
    int  digit = (number % power(10, place) - number % power(10, place - 1))/power(10, place-1);
    return digit;
}

// Run Luhn's Algorithm on number and return 1 if passing, 0 if not
bool luhn(unsigned long long creditnumber, int length)
{
    int sum = 0;
    for (int i = 1; i <= length; i++)
    {
        if (i % 2 == 0)
        {
            int dig = get_digit(creditnumber, i);
            if (dig < 5)
                sum += 2 * get_digit(creditnumber, i);
            else
            {
                sum += get_digit(dig*2, 1);
                sum += get_digit(dig*2, 2);
            }
        } 
        else
            sum += get_digit(creditnumber, i);           
    }
    printf("luhn sum is %i\n", sum);
    if (sum % 10 == 0)
        return 1;
    else
        return 0;
}

// Determine type of credit card
string type_card(unsigned long long creditnumber)
{
    string type = "INVALID";
    int length = get_length(creditnumber);
    // Get first two digits
    int prefix = (creditnumber - creditnumber % power(10, length-2)) / power(10, length-2);
    // Check against types in spec
    if (prefix == 34 || prefix == 37)
        type = "AMEX";
    else if (prefix > 50 && prefix < 56)
        type = "MASTERCARD";
    else if (prefix > 39 && prefix < 50)
        type = "VISA";
    return type;
}

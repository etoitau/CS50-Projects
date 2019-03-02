// CS50 Problem Set 2
// mario
// draw a little mario style pyramid of a given height
//
// by: Kyle Chatman
// on: 190206

#include <cs50.h>
#include <stdio.h>

int get_height(int min, int max);
void build_pyramid(int height);

int main(void)
{
int ht = get_height(1, 8);
build_pyramid(ht);
}

// Get the pyramid height from the user checking for valid range
int get_height(int min, int max)
{
    int height;
    do
    {
        height = get_int("Please enter pyramid height between %i and %i\n", min, max);
    }
    while (height < min || height > max);
    return height;
}

// Draw pyramid of desired height on the screen
void build_pyramid(int height)
{
    for (int i = 1; i <= height; i++)
    {
        for(int j = 0; j < height - i; j++)
        {
            printf(" ");
        }
        for(int k = 0; k < i; k++)
        {
            printf("#");
        }
        printf("  ");
        for(int l = 0; l < i; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}
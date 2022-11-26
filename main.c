/*
 * Oguz Alp Cakmak
 * November 26, 2022
 * Not a great implementation of BASE64 encoding and decoding :) But easy to understand. (I hope.)
 * Usage is very straightforward but be careful with the buffers.
 * Didn't want to use mallocs and stuff so code stays "readable".
 * Didn't use any sources on doing this so... Enjoy!
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#define BUFSIZ 1024

int bits[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};   // Bits.
char const table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // BASE64 table.

void wbits(char c, int from, int to) // Write bits.
{
    for (int i = to - 1; i >= from; i--)
    {
        bits[i] = c % 2;
        c /= 2;
    }
}

int rbits(int from, int to) // Read bits.
{
    int i, sum = 0;
    for (i = to - 1; i >= from; i--)
        sum += bits[i] * pow(2, to - i - 1);
    return sum;
}

char tabix(char c) // Converting ASCII codes to BASE64 table indexes.
{
    if ('A' <= c && c <= 'Z')
        return c - 65;
    else if ('a' <= c && c <= 'z')
        return c - 71;
    else if ('0' <= c && c <= '9')
        return c + 4;
    else if (c == '+')
        return c + 19;
    else if (c == '/')
        return c + 16;
    else
        return '\0';
}

void encode(char instr[], char outstr[])
{
    int i = 0, j = 0;
    int len = strlen(instr);
    instr[len + 1] = '\0'; // To prevent any false reads, putting extra zeros at end of the string.
    while (i < len)
    {
        wbits(instr[i++], 0, 8);
        wbits(instr[i++], 8, 16);
        wbits(instr[i++], 16, 24);
        outstr[j++] = table[rbits(0, 6)];
        outstr[j++] = table[rbits(6, 12)];
        outstr[j++] = table[rbits(12, 18)];
        outstr[j++] = table[rbits(18, 24)];
    }
    if (len % 3 > 0)
    {
        outstr[j - 1] = '=';
        if (len % 3 == 1)
        {
            outstr[j - 2] = '=';
        }
    }
    outstr[j] = '\0';
}

void decode(char instr[], char outstr[])
{
    int i = 0, j = 0;
    int len = strlen(instr);
    while (i < len)
    {
        wbits(tabix(instr[i++]), 0, 6);
        wbits(tabix(instr[i++]), 6, 12);
        wbits(tabix(instr[i++]), 12, 18);
        wbits(tabix(instr[i++]), 18, 24);
        outstr[j++] = rbits(0, 8);
        outstr[j++] = rbits(8, 16);
        outstr[j++] = rbits(16, 24);
    }
    outstr[j] = '\0';
}

int main(void)
{
    char testIn[BUFSIZ], testS1[BUFSIZ], testS2[BUFSIZ], testS3[BUFSIZ];
    printf("\n\tINPUT   = ");
    fgets(testIn, BUFSIZ, stdin);
    strcpy(testS1, testIn); // Keeping the original input untouched.
    encode(testS1, testS2);
    printf("\tENCODED = %s\n", testS2);
    decode(testS2, testS3);
    printf("\tDECODED = %s\n", testS3);
    strcmp(testIn, testS3) == 0 ? printf("\n\t\tENCODE & DECODE SUCCESFUL.\n\n") : printf("\n\t\tENCODE & DECODE FAILURE.\n\n");
    return 0;
}

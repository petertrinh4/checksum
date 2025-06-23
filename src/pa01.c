/*============================================================================
| Assignment: pa01 - Calculate the checksum of an input file given:
|             -> the name of the input file,
|             -> the checksum size of either 8, 16, or 32 bits
|     Author: Peter Trinh
|   Language: c
| To Compile: gcc -o pa01 pa01.c
| To Execute: c -> ./pa01 inputFilename.txt checksumSize
|                  where inputFilename.txt is the input file
|                  and checksumSize is either 8, 16, or 32
|       Note:
|             All input files are simple 8 bit ASCII input
|             All execute commands above have been tested on Eustis
|      Class: CIS3360 - Security in Computing - Summer 2025
| Instructor: McAlpin
|   Due Date: 6/22/25
+===========================================================================*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void printError();

int main(int argc, char *argv[])
{
    if(argc != 3) printError(); //Ensures the program receives two arguments after the executable name

    char *inputFile = argv[1]; //Stores filename string
    int checkSumSize = atoi(argv[2]); //Converts the input string to an integer for checksum size
    int characterCnt = 0; //Counts number of characters read
    FILE *readInputFile; //File pointer for reading
    char buffer[10000]; //Fixed-size array to hold file contents
    int charFromFile; //Stores characters read from file
    int lineLen = 0; //tracks length of printed lines (for formatting input)
    int groupSize; // Number of bytes per checksum group
    int padCount; //Counts number of padding
    unsigned long checksum = 0; //Holds the checksum value

    if(checkSumSize != 8 && checkSumSize != 16 && checkSumSize != 32) //Check for invalid checksum size
    { 
        printError();
    }
    readInputFile = fopen(inputFile, "r"); //Opens file for reading

    if(!readInputFile)
    {
        perror("Error"); 
        exit(1);
    }
    while((charFromFile = fgetc(readInputFile)) != EOF) //Reads chars one by one until the end of file
    {
        buffer[characterCnt++] = (char)charFromFile; //Storing each char into the buffer[10000] while counting chars
    }
    fclose(readInputFile); //Don't need file anymore at this point

    putchar('\n');

    for(int i = 0; i < characterCnt; i++) //Echo text
    {
        putchar(buffer[i]);
        if(buffer[i] == '\n')
        {
            lineLen = 0;
        }
        else if(++lineLen == 80) //Inserts new line since we want a newline after every 80 characters
        {
            putchar('\n');
            lineLen = 0;
        }
    }
    groupSize = checkSumSize / 8; //Gives number of bytes per checksum group
    padCount = (groupSize - (characterCnt % groupSize)) % groupSize; //Calculates how much padding to add

    for(int i = 0; i < padCount; i++) //Appends the padding chars then prints
    {   
        buffer[characterCnt++] = 'X';
        putchar('X');
        if(++lineLen == 80)
        {
            putchar('\n');
            lineLen = 0;
        }
    }
    putchar('\n'); //Formatting
    lineLen = 0;

    if(checkSumSize == 8) //Calculates checksum
    {
        for(int i = 0; i < characterCnt; i++)
        {
            checksum += (unsigned char)buffer[i]; //Combines four chars
        }
        checksum &= 0xFF; //Summing up all words
    }
    else if(checkSumSize == 16)
    {
        for(int i = 0; i < characterCnt; i += 2)
        {
            unsigned short pair = ((unsigned char) buffer[i] << 8) | (unsigned char)buffer[i + 1];
            checksum += pair;
        }
        checksum &= 0xFFFF; //Summing up the words
    }
    else if(checkSumSize == 32)
    {
        for(int i = 0; i < characterCnt; i += 4)
        {
            unsigned long word = ((unsigned char)buffer[i] << 24) |
                                 ((unsigned char)buffer[i + 1] << 16) |
                                 ((unsigned char)buffer[i + 2] << 8) |
                                 (unsigned char)buffer[i + 3];
            checksum += word;
        }
        checksum &= 0xFFFFFFFF;
    }

    if(lineLen > 0) //Adds newline for formatting
    {
        putchar('\n');
        lineLen = 0;
    }
    printf("%2d bit checksum is %8lx for all %4d chars\n", checkSumSize, checksum, characterCnt); //Prints the checksum result
    
    return 0;
}

void printError() //Checking if user enters wrong checksum size
{
    fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
    exit(1);
}

/*=============================================================================
| I Peter Trinh (pe408680) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/
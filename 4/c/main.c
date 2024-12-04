#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INFILENAME "input"
#define DBG 0
#define DEBUG(...) do { if(DBG) printf(__VA_ARGS__); } while(0)
int main(void)
{
    FILE * infile = fopen(INFILENAME, "rb");
    if(!infile)
    {
        printf("file open failed!\n");
        return -1;
    }

    int result1 = 0;
    int result2 = 0;

    /* === input parsing section === */
    fseek(infile, 0, SEEK_END);
    long size = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    char * input_buf = malloc(size);
    if(!input_buf)
    {
        printf("alloc fail!\n");
        return -1;
    }
    DEBUG("size: %ld\n", size);
    if(size != fread(input_buf, 1, size, infile))
    {
        printf("something wrong!\n");
        return -1;
    }
    fclose(infile);


    int columns = 0;
    while(1)
    {
        if(!isalpha(input_buf[columns]))
        {
            break;
        }
        columns++;
    }
    int rows = 0;
    int offset = 0;
    for(int i = columns; i < size; i++) //pack the alphabet values continuously, discarding linefeeds and such
    {
        int row_end = 0;
        while(!isalpha(input_buf[i+offset]) && ((i + offset) < size))
        {
            offset++;
            row_end = 1;
        }
        if(row_end) rows++;
        if((i + offset) >= size) break;
        input_buf[i] = input_buf[i+offset];
    }
    DEBUG("%dx%d\n", rows, columns);

    char (*input)[columns] = (void*)input_buf; //variably modified type, now we can address the buffer as a matrix[rows][columns]
    /*other approach would be a char*[] , each element containing the pointer &input_buf[row_start]
      This would require us to find out the number of rows first to allocate memory (efficiently)
      But this approach doesn't require packing the alphabet values continuously*/

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            DEBUG("%c", input[i][j]);
        }
        DEBUG("\n");
    }


    /* === puzzle solving section === */
    char found[rows][columns]; //VLA
    memset(found, '_', sizeof(char[rows][columns]));

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            /* at each position, check if XMAS or SAMX can be formed rightward, right-down diagonal, downward, & left-down diagonal */
            /* rest of the directions are mirrored with these, and will be checked from the position at the other end */
            int chk_r = (j < (columns-3)) ? 1 : 0;
            int chk_rd = ((j < (columns-3)) && (i < (rows-3))) ? 1 : 0;
            int chk_d = ((i < (rows-3))) ? 1 : 0;
            int chk_ld = ((j >= 3) && (i < (rows-3))) ? 1 : 0;
            if(chk_r)
            {
                if( (input[i][j] == 'X' && input[i][j+1] == 'M' && input[i][j+2] == 'A' && input[i][j+3] == 'S') ||
                    (input[i][j] == 'S' && input[i][j+1] == 'A' && input[i][j+2] == 'M' && input[i][j+3] == 'X') )
                {
                    result1++;
                    { found[i][j] = input[i][j]; found[i][j+1] = input[i][j+1]; found[i][j+2] = input[i][j+2]; found[i][j+3] = input[i][j+3]; }
                }
            }
            if(chk_rd)
            {
                if( (input[i][j] == 'X' && input[i+1][j+1] == 'M' && input[i+2][j+2] == 'A' && input[i+3][j+3] == 'S') ||
                    (input[i][j] == 'S' && input[i+1][j+1] == 'A' && input[i+2][j+2] == 'M' && input[i+3][j+3] == 'X') )
                {
                    result1++;
                    { found[i][j] = input[i][j]; found[i+1][j+1] = input[i+1][j+1]; found[i+2][j+2] = input[i+2][j+2]; found[i+3][j+3] = input[i+3][j+3]; }
                }
            }
            if(chk_d)
            {
                if( (input[i][j] == 'X' && input[i+1][j] == 'M' && input[i+2][j] == 'A' && input[i+3][j] == 'S') ||
                    (input[i][j] == 'S' && input[i+1][j] == 'A' && input[i+2][j] == 'M' && input[i+3][j] == 'X') )
                {
                    result1++;
                    { found[i][j] = input[i][j]; found[i+1][j] = input[i+1][j]; found[i+2][j] = input[i+2][j]; found[i+3][j] = input[i+3][j]; }
                }
            }
            if(chk_ld)
            {
                if( (input[i][j] == 'X' && input[i+1][j-1] == 'M' && input[i+2][j-2] == 'A' && input[i+3][j-3] == 'S') ||
                    (input[i][j] == 'S' && input[i+1][j-1] == 'A' && input[i+2][j-2] == 'M' && input[i+3][j-3] == 'X') )
                {
                    result1++;
                    { found[i][j] = input[i][j]; found[i+1][j-1] = input[i+1][j-1]; found[i+2][j-2] = input[i+2][j-2]; found[i+3][j-3] = input[i+3][j-3]; }
                }
            }
        }
    }

    DEBUG("\n\n\n");
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            DEBUG("%c", found[i][j]);
        }
        DEBUG("\n");
    }

    memset(found, '_', sizeof(char[rows][columns]));
    for(int i = 1; i < rows-1; i++)
    {
        for(int j = 1; j < columns-1; j++)
        {
            if(input[i][j] == 'A')
            {
                if((input[i-1][j-1] == 'M' && input[i+1][j+1] == 'S') || (input[i-1][j-1] == 'S' && input[i+1][j+1] == 'M')) //diagonal 1
                {
                    if((input[i-1][j+1] == 'M' && input[i+1][j-1] == 'S') || (input[i-1][j+1] == 'S' && input[i+1][j-1] == 'M')) //diagonal 2
                    {
                        result2++;
                        { found[i][j] = input[i][j]; found[i-1][j-1] = input[i-1][j-1]; found[i+1][j+1] = input[i+1][j+1]; found[i-1][j+1] = input[i-1][j+1]; found[i+1][j-1] = input[i+1][j-1]; }
                    }
                }
            }
        }
    }

    DEBUG("\n\n\n");
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            DEBUG("%c", found[i][j]);
        }
        DEBUG("\n");
    }

    printf("result1: %d\n", result1);
    printf("result2: %d\n", result2);
}

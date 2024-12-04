#include <stdio.h>
#include <stdlib.h>

#define DBG 0
#define DEBUG(...) do { if(DBG) printf(__VA_ARGS__); } while(0)
int main(void)
{
    char * filename = "input";
    FILE * infile = fopen(filename, "r");
    if(!infile)
    {
        printf("file open failed!\n");
        return -1;
    }

    int result1 = 0;
    int result2 = 0;

    int c;
    int enable = 1;
    //below is basically a state machine implemented in if/else/goto
    //great example for goto use I think
    while((c = fgetc(infile)) != EOF)
    {
check_mul:
        if(c == 'm')
        {
            if(((c = fgetc(infile)) != EOF) && c == 'u')
            {
                if(((c = fgetc(infile)) != EOF) && c == 'l')
                {
                    if(((c = fgetc(infile)) != EOF) && c == '(')
                    {
                        int n = 0;
                        int a = 0, b = 0; //operands
                        while(((c = fgetc(infile)) != EOF))
                        {
                            if(c < '0' || c > '9')
                            {
                                if(c == ',' && n != 0 && a == 0 && b == 0) //assuming 0 is not a valid value
                                {
                                    a = n;
                                    n = 0;
                                    continue;
                                }
                                else if(c == ')' && n != 0 && a != 0 && b == 0)
                                {
                                    b = n;
                                    result1 += (a*b);
                                    if(enable) result2 += (a*b);
                                    goto continue_outer;
                                }
                                else
                                {
                                    goto check_do;
                                }
                            }
                            else
                            {
                                n = (10 * n) + (c - '0');
                            }
                        }
                    }
                    else
                    {
                        goto check_do;
                    }
                }
                else
                {
                    goto check_do;
                }
            }
            else
            {
                goto check_do;
            }
        }
check_do:
        if(c == 'd')
        {
            if(((c = fgetc(infile)) != EOF) && c == 'o')
            {
                if(((c = fgetc(infile)) != EOF) && c == '(')
                {
                    if(((c = fgetc(infile)) != EOF) && c == ')')
                    {
                        enable = 1;
                        goto continue_outer;
                    }
                    else
                    {
                        goto check_mul;
                    }
                }
                else if(c  == 'n')
                {
                    if(((c = fgetc(infile)) != EOF) && c == '\'')
                    {
                        if(((c = fgetc(infile)) != EOF) && c == 't')
                        {
                            if(((c = fgetc(infile)) != EOF) && c == '(')
                            {
                                if(((c = fgetc(infile)) != EOF) && c == ')')
                                {
                                    enable = 0;
                                    goto continue_outer;
                                }
                                else
                                {
                                    goto check_mul;
                                }
                            }
                            else
                            {
                                goto check_mul;
                            }
                        }
                        else
                        {
                            goto check_mul;
                        }
                    }
                    else
                    {
                        goto check_mul;
                    }
                }
                else
                {
                    goto check_mul;
                }
            }
            else
            {
                goto check_mul;
            }
        }
continue_outer:
    }

    printf("result1: %d\n", result1);
    printf("result2: %d\n", result2);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0 //works but kind of a bad implementation, for part 1
typedef enum
{
    INCREASING,
    DECREASING,
    STARTED,
    INVALID,
} list_status;

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

    int c;
    int n = 0; //holds number, assume no 0 in input, also at least 2 nums in a list
    int prev = 0;
    list_status status = INVALID;
    while((c = fgetc(infile)) != EOF)
    {
        if((c < '0' || c > '9') && n != 0)
        {
            //printf("%d ", n);
            //process accumulated number
            if(status == INVALID)
            {
                //first number in the list
                prev = n;
                n = 0;
                status = STARTED;
                continue;
            }
            else if(status == STARTED)
            {
                if((prev - n) >= 1 && (prev - n) <= 3)
                {
                    status = DECREASING;
                }
                else if((prev - n) <= -1 && (prev - n) >= -3)
                {
                    status = INCREASING;
                }
                else
                {
                    //printf("not safe\n");
                    //list not safe
                    do
                    {
                        //skip ahead to next list
                        c = fgetc(infile);
                    } while(c != '\n' && c != EOF);
                    n = 0;
                    status = INVALID;
                    continue;
                }
            }
            else if(status == INCREASING)
            {
                if((n - prev) > 3 || (n - prev) < 1)
                {
                    //printf("not safe\n");
                    //list not safe
                    do
                    {
                        //skip ahead to next list
                        c = fgetc(infile);
                    } while(c != '\n' && c != EOF);
                    n = 0;
                    status = INVALID;
                    continue;
                }
            }
            else if(status == DECREASING)
            {
                if((prev - n) > 3 || (prev - n) < 1)
                {
                    //printf("not safe\n");
                    //list not safe
                    do
                    {
                        //skip ahead to next list
                        c = fgetc(infile);
                    } while(c != '\n' && c != EOF);
                    n = 0;
                    status = INVALID;
                    continue;
                }
            }

            if(c == '\n')
            {
                //printf("safe!\n");
                //end of list reached without issues, is safe
                result1++;
                status = INVALID;
            }
            prev = n;
            n = 0;
        }
        else
        {
            n = n*10 + (c-'0');
        }
    }

    printf("result1: %d\n", result1);
}
#else //better (?) impl
#define DBG 0
#define DEBUG(...) do { if(DBG) printf(__VA_ARGS__); } while(0)
typedef enum
{
    INCREASING,
    DECREASING,
    STARTED,
    INVALID,
} list_status;
int is_list_unsafe(int nums[], int nums_n);
void drop_elem_from_list(int nums[], int nums_n, int drop_index, int *result_array);
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

    char line[256];

    while(fgets(line, sizeof(line), infile) != NULL)
    {
        int prev;
        char * tok = strtok(line, " \n\r");
        list_status status = INVALID;
        while(tok != NULL)
        {
            int n = atoi(tok);
            DEBUG("%d ", n);
            switch(status)
            {
                case INVALID:
                {
                    status = STARTED;
                }
                break;
                case STARTED:
                {
                    if((prev - n) >= 1 && (prev - n) <= 3)
                    {
                        status = DECREASING;
                    }
                    else if((prev - n) <= -1 && (prev - n) >= -3)
                    {
                        status = INCREASING;
                    }
                    else
                    {
                        DEBUG("not safe\n");
                        //list not safe
                        goto continue_to_next_line;
                    }
                }
                break;
                case INCREASING:
                {
                    if((n - prev) > 3 || (n - prev) < 1)
                    {
                        DEBUG("not safe\n");
                        //list not safe
                        goto continue_to_next_line;
                    }
                }
                break;
                case DECREASING:
                {
                    if((prev - n) > 3 || (prev - n) < 1)
                    {
                        DEBUG("not safe\n");
                        //list not safe
                        goto continue_to_next_line;
                    }
                }
                break;
            }
            prev = n;
            tok = strtok(NULL, " \n\r");
        }
        //if it reaches here that means the list was safe, or list was empty (status == INVALID)
        if(status != INVALID)
        {
            DEBUG("safe!\n");
            result1++;
        }
        continue_to_next_line:
    }

    printf("result1: %d\n", result1);

    rewind(infile);
    int result2 = 0;
    #if 0 //what the hell, I give up
    int nums[32]; //same approach won't work for problem 2, so array
    while(fgets(line, sizeof(line), infile) != NULL)
    {
        int nums_n = 0;
        char * tok = strtok(line, " \n\r");
        while(tok != NULL)
        {
            nums[nums_n++] = atoi(tok);
            tok = strtok(NULL, " \n\r");
        }
        list_status status = INVALID;
        for(int i = 1; i < nums_n; i++)
        {
            //algo: if safe ok, if unsafe at ith element, test for safety with i-1, i, or i+1 element removed. we don't have to test for safety from start of list, only from i-2 ?
            if(i == 1) //STARTED
            {
                if((nums[i-1] - nums[i]) >= 1 && (nums[i-1] - nums[i]) <= 3)
                {
                    status = DECREASING;
                    //safe so far
                }
                else if((nums[i-1] - nums[i]) <= -1 && (nums[i-1] - nums[i]) >= -3)
                {
                    status = INCREASING;
                    //safe so far
                }
                else
                {
                    //unsafe
                    //recheck for safety by dropping either [0] or [1]
                    for(int drop = 0; drop <= 1; drop++)
                    {
                        int safe = 1;
                        list_status status2 = STARTED;
                        for(int i = 2; i < nums_n; i++)
                        {
                            int n, prev;
                            prev = (i-1 == drop) ? nums[i-2] : nums[i-1];
                            if(status == STARTED)
                            {
                                if((prev - n) >= 1 && (prev - n) <= 3)
                                {
                                    status = DECREASING;
                                    //safe so far
                                }
                                else if((prev - n) <= -1 && (prev - n) >= -3)
                                {
                                    status = INCREASING;
                                    //safe so far
                                }
                                else
                                {
                                    //unsafe
                                    safe = 0;
                                    break;
                                }
                            }
                            else if(status == DECREASING)
                            {
                                if((prev - n) < 1 || (prev - n) > 3)
                                {
                                    //unsafe
                                    safe = 0;
                                    break;
                                }
                            }
                            else if(status == INCREASING)
                            {
                                if((n - prev) < 1 || (n - prev) > 3)
                                {
                                    //unsafe
                                    safe = 0;
                                    break;
                                }
                            }
                        }
                        if(safe == 1)
                        {
                            //safe list found
                            break;
                        }
                    }
                }
            }
            else if(i == 2)
            {
                if((nums[i-1] - nums[i]) >= 1 && (nums[i-1] - nums[i]) <= 3)
                {
                    if(status == DECREASING)
                    {
                        //safe so far
                    }
                    else
                    {
                        //unsafe, status may change to increased based on the element to drop
                    }
                }
                else if((nums[i-1] - nums[i]) <= -1 && (nums[i-1] - nums[i]) >= -3)
                {
                    if(status == INCREASING)
                    {
                        //safe so far
                    }
                    else
                    {
                        //unsafe, status may change to decreased based on the element to drop
                    }
                }
                else
                {
                    //recheck for safety by dropping either [1], [2] or [3]
                }
            }
            else
            {
                //now that at least 3 elements have formed a continuity, status must stay the same. either [i] or [i-1] may be dropped
            }
        }
    }
    #elif 0 //brute force
    int nums[32];
    while(fgets(line, sizeof(line), infile) != NULL)
    {
        int nums_n = 0;
        char * tok = strtok(line, " \n\r");
        while(tok != NULL)
        {
            nums[nums_n++] = atoi(tok);
            tok = strtok(NULL, " \n\r");
        }
        if(nums_n == 0)
        {
            continue;
        }
        int safety = is_list_unsafe(nums, nums_n);
        if(safety == 0)
        {
            result2++;
            continue;
        }
        else
        {
            int nums_adjusted[nums_n-1];
            for(int i = 0; i < nums_n; i++)
            {
                drop_elem_from_list(nums, nums_n, i, nums_adjusted);
                if(is_list_unsafe(nums_adjusted, nums_n-1) == 0)
                {
                    result2++;
                    goto continue_outer;
                }
            }
        }
        continue_outer:
    }
    #else //thanks reddit for non-brute-force tips
    int nums[32];
    while(fgets(line, sizeof(line), infile) != NULL)
    {
        int nums_n = 0;
        char * tok = strtok(line, " \n\r");
        while(tok != NULL)
        {
            nums[nums_n++] = atoi(tok);
            tok = strtok(NULL, " \n\r");
        }
        if(nums_n == 0)
        {
            continue;
        }
        int safety = is_list_unsafe(nums, nums_n);
        if(safety == 0)
        {
            result2++;
            continue;
        }
        else
        {
            int nums_adjusted[nums_n-1];
            drop_elem_from_list(nums, nums_n, safety, nums_adjusted);
            if(is_list_unsafe(nums_adjusted, nums_n-1))
            {
                drop_elem_from_list(nums, nums_n, safety-1, nums_adjusted);
                if(is_list_unsafe(nums_adjusted, nums_n-1))
                {
                    if(safety == 2)
                    {
                        drop_elem_from_list(nums, nums_n, safety-2, nums_adjusted);
                        if(is_list_unsafe(nums_adjusted, nums_n-1))
                        {
                            //unsafe
                        }
                        else
                        {
                            //safe
                            result2++;
                            continue;
                        }
                    }
                }
                else
                {
                    //safe
                    result2++;
                    continue;
                }
            }
            else
            {
                //safe
                result2++;
                continue;
            }
        }
    }
    #endif

    printf("result2: %d\n", result2);
}

int is_list_unsafe(int nums[], int nums_n) //returns index at which safety breaks
{
    list_status status = STARTED;
    for(int i = 1; i < nums_n; i++)
    {
        if(status == STARTED)
        {
            if((nums[i-1] - nums[i] <= 3) && (nums[i-1] - nums[i] >= 1))
            {
                status = DECREASING;
            }
            else if((nums[i-1] - nums[i] >= -3) && (nums[i-1] - nums[i] <= -1))
            {
                status = INCREASING;
            }
            else
            {
                //unsafe
                return i;
            }
        }
        else if(status == INCREASING)
        {
            if((nums[i-1] - nums[i] < -3) || (nums[i-1] - nums[i] > -1))
            {
                //unsafe
                return i;
            }
        }
        else if(status == DECREASING)
        {
            if((nums[i-1] - nums[i] > 3) || (nums[i-1] - nums[i] < 1))
            {
                //unsafe
                return i;
            }
        }
    }
    return 0;
}

void drop_elem_from_list(int nums[], int nums_n, int drop_index, int *result_array)
{
    int i = 0, j = 0;
    while(i < nums_n)
    {
        if(i == drop_index)
        {
            i++;
            continue;
        }
        else
        {
            result_array[j++] = nums[i++];
        }
    }
}
#endif

#include <stdio.h>
#include <stdlib.h>
int comp(const void * a, const void * b)
{
    return (*(int*)a - *(int*)b);
}
int main(void)
{
    char * filename = "input";
    FILE * infile = fopen(filename, "r");
    if(!infile)
    {
        printf("file open failed!\n");
        return -1;
    }
    int *arr;
    int *brr;
    int n = 0;
    int arrsize = 256;
    arr = malloc(arrsize*sizeof(int));
    brr = malloc(arrsize*sizeof(int));
    if(!arr || !brr)
    {
        printf("alloc fail!\n");
        return -1;
    }
    while(fscanf(infile, "%d %d", &arr[n], &brr[n]) != EOF)
    {
        //printf("%d %d\n", a, b);
        n++;
        if(n >= arrsize)
        {
            arrsize += 256;
            int * arr2 = realloc(arr, arrsize*sizeof(int));
            int * brr2 = realloc(brr, arrsize*sizeof(int));
            if(!arr2 || !brr2)
            {
                printf("alloc fail!\n");
                return -1;
            }
            arr = arr2;
            brr = brr2;
        }
    }

    qsort(arr, n, sizeof(int), comp);
    qsort(brr, n, sizeof(int), comp);

    int result1 = 0;
    for(int i =0; i < n; i++)
    {
        result1 += abs(arr[i] - brr[i]);
    }

    printf("result1 : %d\n", result1);

    int result2 = 0;
    for(int i = 0; i < n; i++)
    {
        int count = 0;
        for(int j = 0; j < n; j++)
        {
            if(arr[i] == brr[j])
            {
                count++;
            }
        }
        result2 += arr[i] * count;
    }

    printf("result2 : %d\n", result2);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mpich/mpi.h>

struct ThreadParam
{
    int size;
    int* part;
};
int* arr;
int n;

void NhapMang(FILE* input)
{
    n = 0;
    int temp = 't';
    while((fscanf(input,"%d ",&temp)) == 1) 
            n++;
    arr = (int*)malloc(sizeof(int)*n);
    rewind(input);
    for(int i = 0;i <= n - 1;i++)
        fscanf(input,"%d ",arr+i);
}

void XuatMang(FILE* output, ThreadParam* param)
{
    for(int i = 0;i <= param->size - 1;i++)
        fprintf(output,"%d ",*(param->part + i));
}

void* SapXep(void* param)
{
    ThreadParam* t = (ThreadParam*) param;
    int key, j;
    for(int i = 1;i <= t->size - 1;i++)
    {
        key = *(t->part + i);
        j = i - 1;
        while(j >= 0 && *(t->part + j) > key)
        {
            *(t->part + j + 1) = *(t->part + j);
            j -= 1;
        }
        *(t->part + j + 1) = key;
    }
    return NULL;
}

int CountDigit(int num)
{
    if(num == 0)
        return 1;
    int digit = 0;
    while(num != 0)
    {
        num /= 10;
        digit++;
    }
    return digit;
}

int main(int argc, char* argv[])
{
    FILE* inp = fopen("input.txt","r");
    FILE* out = fopen("output.txt","w");

    NhapMang(inp);
    int max = *(arr);
    for(int i = 1;i<= n - 1;i++)
        if(max < *(arr+i))
            max = *(arr+i);

    int maxdigit = CountDigit(max);
    ThreadParam* t[maxdigit];
    for(int i = 0;i <= maxdigit - 1;i++)
        t[i] = (ThreadParam*)malloc(sizeof(ThreadParam));

    pthread_t thread[maxdigit];
    int count, k = 0, t_size = 0;
    for(int i = 1;i <= maxdigit;i++)
    {
        count = 0;
        t_size = 0;
        for(;count <= n - 1;count++)
            if(CountDigit(*(arr + count)) == i)
                t_size++;
        t[k]->size = t_size;
        t[k]->part = (int*)malloc(sizeof(int)*t_size);
        k++;
    }
    k = 0;
    for(int i = 1;i <= maxdigit;i++)
    {
        count = 0;
        for(int j = 0;j <= n - 1;j++)
            if(CountDigit(*(arr + j)) == i)
            {
                *(t[k]->part + count) = *(arr + j);
                count++;
            }
        k++;
    }
    double stime = 0.0;
    MPI_Init(NULL,NULL);
    stime -= MPI_Wtime();
    for(int i = 0;i <= maxdigit - 1;i++)
        pthread_create(&thread[i],NULL,&SapXep,(void*)t[i]);
    for(int i = 0;i <= maxdigit - 1;i++)
        pthread_join(thread[i],NULL);
    stime += MPI_Wtime();
    for(int i = 0;i <= maxdigit - 1;i++)
        XuatMang(out,t[i]);
    printf("Parallel time: %lf",stime);
    fclose(inp);
    fclose(out);
    for(int i = 0;i <= maxdigit - 1;i++)
        free(t[i]);
}
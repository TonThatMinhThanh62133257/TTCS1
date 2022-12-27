#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpich/mpi.h>

int* InitArray(int n, int min, int max)
{
    int* t = (int*)malloc(sizeof(int)*n);
    for(int i = 0;i <= n - 1;i++)
        t[i] = rand() % max + min;
    return t;
}

void Sort(int* arr, int* t1, int* t2, int n1, int n2, int l)
{
    int i = 0, j = 0, k = l;
    while(i < n1 && j < n2)
    {
        if(t1[i] <= t2[j])
        {
            arr[k] = t1[i];
            i++;
        }
        else
        {
            arr[k] = t2[j];
            j++;
        }
        k++;
    }
    while(i < n1)
    {
        arr[k] = t1[i];
        i++;
        k++;
    }
    while(j < n2)
    {
        arr[k] = t2[j];
        j++;
        k++;
    }
}

void Merge(int* arr, int l, int m, int r)
{
    int n1 = m - l + 1, n2 = r - m;
    int t1[n1], t2[n2];

    for(int i = 0;i <= n1 - 1;i++)
        t1[i] = arr[l + i];
    for(int i = 0;i <= n2 - 1;i++)
        t2[i] = arr[m + 1 + i];
    
    Sort(arr,t1,t2,n1,n2,l);
}

void MergeSort(int* arr, int l, int r)
{
    if(l < r)
    {
        int m = (l+r-1)/2;
        MergeSort(arr,l,m);
        MergeSort(arr,m+1,r);
        Merge(arr,l,m,r);
    }
}

void MergeChunks(int* arr, int n, int elementsPerProc, int worldSize, int aggregation)
{
    int l, m, r;
    for(int i = 0;i < worldSize;i += 2)
    {
        l = i*elementsPerProc*aggregation;
        r = (i + 2) * (elementsPerProc*aggregation) - 1;
        m = l + (elementsPerProc * aggregation) - 1;
        if(r >= n)
            r = n - 1;
        Merge(arr,l,m,r);
    }
    if(worldSize / 2 >= 1)
        MergeChunks(arr, n, elementsPerProc, worldSize / 2, aggregation * 2);
}

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        printf("Error: invalid input! Please input number of elements that each process will sort and then min value and max value of each elements\n");
        return -1;
    }
    int elementsPerProc = atoi(argv[1]), min = atoi(argv[2]), max = atoi(argv[3]);
    double partime = 0.0, seqtime = 0.0;
    if(min >= max)
    {
        printf("Invalid min, max input!\n");
        return -1;
    }
    int worldSize, worldRank, n;
    int* arr = NULL;
    int* sortedArr = NULL;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
    MPI_Request request;
    if(worldRank == 0)
    {
        n = worldSize * elementsPerProc;
        srand(time(NULL));
        arr = InitArray(n,min,max);
        FILE* out = fopen("beforesort","w");
        for(int i = 0;i <= n - 1;i++)
            fprintf(out,"%d ",arr[i]);
        fclose(out);
    }
    int* subset = (int*)malloc(sizeof(int)*elementsPerProc);
    if(worldRank == 0)
        partime -= MPI_Wtime();
    MPI_Scatter(arr,elementsPerProc,MPI_INT,subset,elementsPerProc,MPI_INT,0,MPI_COMM_WORLD);
    MergeSort(subset,0,elementsPerProc-1);
    if(worldRank == 0)
        sortedArr = (int*)malloc(sizeof(int)*n);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(subset, elementsPerProc, MPI_INT, sortedArr, elementsPerProc, MPI_INT,0,MPI_COMM_WORLD);
    if(worldRank == 0)
    {
        if(sortedArr != NULL)
            printf("True!");
        MergeChunks(sortedArr,n,elementsPerProc,worldSize,1);
        partime += MPI_Wtime();
        printf("Sort result saved to aftersort file!\n");
        FILE* result = fopen("aftersort","w");
        for(int i = 0;i <= n - 1;i++)
        {
            fprintf(result,"%d ",sortedArr[i]);
        }
        printf("\nParallel merge sort execution time: %lf",partime);
        seqtime -= MPI_Wtime();
        MergeSort(arr,0,n-1);
        seqtime += MPI_Wtime();
        printf(", seq merge sort execution time: %lf\n",seqtime);
        free(sortedArr);
        fclose(result);
    }
    free(arr);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

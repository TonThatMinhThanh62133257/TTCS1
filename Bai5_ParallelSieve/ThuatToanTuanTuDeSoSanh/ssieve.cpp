#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>

void Sieve(int n)
{
    double runtime = 0.0;
    runtime -= MPI_Wtime();
    int* marked = (int*)calloc(n,sizeof(int));
    int prime = 2, count = 0;
    for(int i = prime;i * i <= n;i++)
    {
        if(marked[i] == 0)
            for(int j = i*i;j <= n;j += i)
                marked[j] = 1;
    }
    for(int i = prime;i <= n;i++)
        if(marked[i] == 0)
            count++;
    runtime += MPI_Wtime();
    printf("Found %d primes in %lf seconds\n",count,runtime);
    free(marked);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("ERROR! NOT ENOUGH ARGUMENTS!\n");
        return -1;
    }
    int n = atoi(argv[1]);
    MPI_Init(NULL,NULL);
    Sieve(n);
    MPI_Finalize();

}
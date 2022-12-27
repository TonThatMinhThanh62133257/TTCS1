#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>

int main(int argc, char* argv[])
{
    int i, n, index, low, high, size, prime, count, result, first, worldRank, worldSize;
    char* marked = NULL;
    double runtime = 0.0;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&worldRank);
    MPI_Comm_size(MPI_COMM_WORLD,&worldSize);

    MPI_Barrier(MPI_COMM_WORLD);
    if(worldRank == 0)
        runtime -= MPI_Wtime();
    
    if(argc != 2)
    {
        printf("ERROR! NOT ENOUGH ARGUMENTS!");
        MPI_Finalize();
        return -1;
    }

    n = atoi(argv[1]);
    low = 2 + worldRank * (n - 1) / worldSize;
    high = 1 + (worldRank + 1) * (n - 1) / worldSize;
    size = high - low + 1;
    marked = (char*)calloc(size,sizeof(char));
    if(worldRank == 0)
        index = 0;
    prime = 2;

    do
    {
        if(prime * prime > low)
            first = prime * prime - low;
        else
        {
            if((low % prime) == 0)
                first = 0;
            else
                first = prime - (low % prime);
        }
        for(i = first; i < size; i += prime)
            marked[i] = 1;
        
        if(worldRank == 0)
        {
            while(marked[++index])
            {
                ;
            }
            prime = index + 2;
        }
        if(worldSize > 1)
            MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    while(prime*prime <= n);
    
    count = 0;

    for(i = 0; i < size; i++)
        if(marked[i] == 0)
            count++;
    if(worldRank == 0)
        result = 0;
    MPI_Reduce(&count, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(worldRank == 0)
    {
        runtime += MPI_Wtime();
        printf("Found %d primes in %lf seconds\n",result,runtime);
    }
    MPI_Finalize();
    return 0;
}
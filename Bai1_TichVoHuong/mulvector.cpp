#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int max, S = 0;
pthread_mutex_t lock;

struct Vector{
    int numOfElement;
    int* value;
};
Vector *v[2];

void CreateVector(Vector* &vector, int numOfElement)
{
    vector->numOfElement = numOfElement;
    vector->value = (int*)malloc(sizeof(int*)*numOfElement);
}

void ReadVector(FILE* file, Vector* &vector)
{
    int n, *ptr;
    fscanf(file,"%d\n",&n);
    CreateVector(vector,n);
    for(int i = 0; i <= n - 1;i++)
        fscanf(file,"%d\t",(vector->value + i));
}

void* Multiply(void* arg)
{
    int tich;
    int* i = (int *)arg;
    tich = *(v[0]->value + *i) * *(v[1]->value + *i);
    pthread_mutex_lock(&lock);
    S+=tich;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char *argv[])
{
    if(!(argv[1] != NULL && argv[2] != NULL))
    {
        printf("Error: you didn't input anything\n");
        return -1;
    }

    FILE* f1 = fopen(argv[1],"r");
    FILE* f2 = fopen(argv[2],"r");
    v[0] = (Vector*)malloc(sizeof(Vector));
    v[1] = (Vector*)malloc(sizeof(Vector));
    ReadVector(f1,v[0]);
    ReadVector(f2,v[1]);
    
    if(v[0]->numOfElement != v[1]->numOfElement)
    {
        printf("So luong phan tu cua v1 va v2 khong bang nhau!\n");
        return -1;
    }

    max = v[1]->numOfElement;
    pthread_t thread[max];
    int ind[max];
    for(int i = 0;i <= max - 1;i++)
        ind[i] = i;

    for(int i = 0;i <= max - 1;i++)  
        pthread_create(&thread[i],NULL,Multiply,(void*)&ind[i]);

    for(int i = 0;i <= max - 1;i++)
        pthread_join(thread[i],NULL);

    printf("Tich cua 2 vector la: %d\n",S);
    pthread_exit(NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}
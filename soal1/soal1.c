#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> //library thread

void *fact( void *ptr );
long long key[100];
long long data[100];
int count = 0;

long long StrToInt(char arg[]){

    long long i, tot = 0;
    for(i = 0; i < strlen(arg); i++){
        tot += arg[i] - '0';
        if(i < strlen(arg)-1) tot *= 10;
    }

    return tot;
}

int cmpFunc(const void * a, const void * b)
{
    return (*(int *)a - *(int *)b );
}

int main(int argc, char** argv)
{
    pthread_t threads[argc];
    int i;

    for(i = 0; i < argc - 1; i++){
        const long long arg = StrToInt(argv[i+1]);
        pthread_create(&threads[i], NULL, fact, (void *) arg);
    }

    for(i = 0; i < argc - 1; i++)
       pthread_join(threads[i], NULL);
    
    qsort(key, 100, sizeof(long long), cmpFunc);
    qsort(data, 100, sizeof(long long), cmpFunc);

    for(i = 0; i < 100; i++)
        if(data[i] != 0) printf("%lld! = %lld\n", key[i], data[i]);
}

void *fact( void *ptr )
{
    long long i, res, arg;
    arg = (long long) ptr;
    res = 1;

    for(i = 1; i <= arg; i++) res *= i;

    data[count] = res;
    key[count] = arg;
    count++;
}
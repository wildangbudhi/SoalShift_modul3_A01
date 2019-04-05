#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread

void *fact( void *ptr );
int data[100];


int main(int argc, char** argv)
{
    pthread_t threads[argc];
    int i;

    for(i = 0; i < argc - 1; i++){
        const int arg = (int) argv[i+1][0] - '0';
        pthread_create(&threads[i], NULL, fact, (void *) arg);
    }

    for(i = 0; i < argc - 1; i++)
       pthread_join(threads[i], NULL);
    
    for(i = 0; i < 100; i++)
        if(data[i] != 0) printf("%d! = %d\n", i, data[i]);
}

void *fact( void *ptr )
{
    int i, res, arg;
    arg = (int) ptr;
    res = 1;

    for(i = 1; i <= arg; i++) res *= i;

    data[arg] = res;
    // index[in] = arg;
    // in++;
}
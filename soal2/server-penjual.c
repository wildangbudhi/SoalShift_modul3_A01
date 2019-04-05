#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h> //library thread
#define PORT 8080

void *cekStock( void *ptr );
void *runCommand( void *ptr );

int cekStockTimerStart, readBuffer;
int new_socket;
int *stock;

int main(int argc, char const *argv[]) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) > 1) {
        perror("More one Client Connected");
        exit(EXIT_FAILURE);
    }

    key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock = shmat(shmid, NULL, 0);
    *stock = 0;

    pthread_t threads[2];

    pthread_create(&threads[0], NULL, cekStock, NULL);
    pthread_create(&threads[1], NULL, runCommand, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}

void *runCommand( void *ptr ){

    char buffer[1024];

    while(1){
        read( new_socket , buffer, 1024);
        if(!strcmp(buffer, "tambah"))
            *stock = *stock + 1;
    }

}

void *cekStock( void *ptr ){

    while(1){
        cekStockTimerStart = 1;
        sleep(5);
        printf("Stok : %d\n", *stock);
        cekStockTimerStart = 0;
    }
    
}
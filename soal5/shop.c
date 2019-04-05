#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>
#include <fcntl.h>

#define StandbyMode 0
#define BattleMode 1
#define ShopMode 2

int kbhit(void);
void *display( void *ptr );

int *value;

int main(){

    pthread_t thread;
    int choice;

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    *value = 0;

    pthread_create(&thread, NULL, display, NULL);
   
    while(1){
        while(!kbhit());
        choice = getchar() - '0';

        if(choice == 1)
            *value = *value + 1;

        if(choice == 2)
            exit(0);
    }

}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
    ungetc(ch, stdin);
    return 1;
    }

    return 0;
}

void *display( void *ptr ){
    while(1){
        printf("Shop\nFood stock : %d\nChoices\n1. Restock\n2. Back\n", *value);
        sleep(1);
        system("clear");
    }
}
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

void changemode(int dir);
int kbhit(void);
void *hungerStatusDecrease( void *ptr );
void *hygieneStatusDecrease( void *ptr );
void *healthStatusIncrease( void *ptr );
void *display( void *ptr );

char monsName[50];
int hungerStatus = 200;
int hygieneStatus = 100;
int healthStatus = 300;
int food = 0;
int mode = StandbyMode;
int bath = 0;
int enemyHealthStatus = 100;
int *value;

int main(){

    pthread_t thread;
    int choice;

    printf("Insert monstername :\n");
    gets(monsName);

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    pthread_create(&thread, NULL, display, NULL);
    pthread_create(&thread, NULL, hungerStatusDecrease, NULL);
    pthread_create(&thread, NULL, hygieneStatusDecrease, NULL);
    pthread_create(&thread, NULL, healthStatusIncrease, NULL);
    
    changemode(1);
    while(1){
        if(hungerStatus <= 0)
            printf("%s died starving\n", monsName);
        if(hygieneStatus <=0)
            printf("%s died due to illness\n", monsName);

        while(!kbhit());
        choice = getchar() - '0';

        if(mode == StandbyMode){
            if(choice == 1){
                if(food){
                    hungerStatus += 15;
                    food--;
                }
                else printf("insufficient food\n");
            }

            if(choice == 2){
                if(bath <= 0){
                    bath = 20;
                    hygieneStatus += 30;
                }
                else printf("bath is in cooldown\n");
            }

            if(choice == 3)
                mode = BattleMode;
            
            if(choice == 4)
                mode = ShopMode;
            
            if(choice == 5)
                exit(0);
        }

        if(mode == BattleMode){
            if(choice == 1){
                healthStatus -=20;
                enemyHealthStatus -= 20;

                if(healthStatus <= 0){
                    printf("%s died in the battle\n", monsName);
                    exit(0);
                }

                else if(enemyHealthStatus <= 0){
                    printf("%s won the battle\n", monsName);
                    sleep(0.5);
                    mode = StandbyMode;
                }
            }

            if(choice == 2)
                mode = StandbyMode;
        }

        if(mode == ShopMode){
            if(choice == 1){
                if(*value > 0){
                    *value = *value - 1;
                    food++;
                }
                else printf("shop's food is out of stock\n");
            }

            if(choice == 2)
                mode = StandbyMode;
        }
    }

}

void changemode(int dir){
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
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
        if(mode == StandbyMode){
            printf("Standby Mode\nHealth : %d\nHunger : %d\nHygiene : %d\nFood left : %d\n", healthStatus, hungerStatus, hygieneStatus, food);

            if(bath <= 0) printf("Bath is ready\n");
            else printf("Bath will be ready in %ds\n", bath);
            printf("Choices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n");
        }

        if(mode == BattleMode)
            printf("Battle Mode\n%s’s Health : %d\nEnemy’s Health : %d\nChoices\n1. Attack\n2. Run\n", monsName, healthStatus, enemyHealthStatus);
        
        if(mode == ShopMode)
            printf("Shop Mode\nShop food stock : %d\nYour food stock : %d\nChoices\n1. Buy\n2. Back\n", *value, food);

        bath--;
        sleep(1);
        system("clear");
    }
}
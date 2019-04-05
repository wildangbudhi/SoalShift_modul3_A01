#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> //library thread

void *disableAgmal( void *ptr );
void *disableIraj( void *ptr );
void *agmalTerbangun( void *ptr );
void *irajTertidur( void *ptr );
int WakeUp_Status, Spirit_Status; 
int AgmalBangunCounter, IrajTidurCounter;
int AgmalDisabled, IrajDisabled;

int main()
{
    Spirit_Status = 100;

    pthread_t threads[2];
    char input[20];

    do{
        gets(input);

        if(!strcmp(input, "All Status"))
            printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n", WakeUp_Status, Spirit_Status);

        if(!strcmp(input, "Agmal Ayo Bangun")){
            if(AgmalBangunCounter == 2 && !AgmalDisabled)
                pthread_create(&threads[0], NULL, disableAgmal, NULL);

            if(!AgmalDisabled){
                WakeUp_Status += 15;
                AgmalBangunCounter++;
            }
        }
            
        if(!strcmp(input, "Iraj Ayo Tidur")){
            if(IrajTidurCounter == 2 && !IrajTidurCounter)
                pthread_create(&threads[1], NULL, disableIraj, NULL);

            if(!IrajDisabled){
                Spirit_Status -= 30;
                IrajTidurCounter++;
            }
        }
            

    }while(WakeUp_Status < 100 && Spirit_Status > 0);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    pthread_create(&threads[0], NULL, agmalTerbangun, NULL);
    pthread_create(&threads[1], NULL, irajTertidur, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}

void *agmalTerbangun( void *ptr ){
    if(WakeUp_Status >= 100)
        printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
}

void *irajTertidur( void *ptr ){
    if(Spirit_Status <= 0)
        printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
}

void *disableAgmal( void *ptr ){
    printf("Agmal Ayo Bangun disabled 10 s\n");
    AgmalDisabled = 1;
    sleep(10);
    AgmalDisabled = 0;
    AgmalBangunCounter = 0;
}

void *disableIraj( void *ptr ){
    printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
    IrajDisabled = 1;
    sleep(10);
    AgmalDisabled = 0;
    IrajTidurCounter = 0;
}
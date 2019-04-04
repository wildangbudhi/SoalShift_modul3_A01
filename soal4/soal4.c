#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void bikinFile();
void kompresFile();
void ekstrakFile();
void *thread(void *argv);

int main(){
    int num_t = 2, i;
    pthread_t tid[num_t];
    
    for (i=0; i<num_t; i++) {
        pthread_create(&tid[i], NULL, thread, &i);
    }
    for (i=0; i<num_t; i++) {
        pthread_join(tid[i], NULL);
    }
}

void *thread(void *argv){
    int *ptr = (int *) argv;
    int id = *ptr + 1;
    
    bikinFile(id);
    sleep(3);
    kompresFile(id);
    printf("istirahat 15 detik\n");
    sleep(5);
    ekstrakFile(id);
    
    return 0;
}

void ekstrakFile(int x){
    char dir[30];
    sprintf(dir, "~/Documents/FolderProses%d", x);
    char filename[50];
    sprintf(filename, "%s/SimpanProses%d.txt", dir, x);
    char cmd[180];
    sprintf(cmd, "unzip -j %s/KompresProses%d.zip -d %s", dir, x, dir);
    system(cmd);
}

void kompresFile(int x){
    char dir[30];
    sprintf(dir, "~/Documents/FolderProses%d", x);
    char filename[50];
    sprintf(filename, "%s/SimpanProses%d.txt", dir, x);
    char cmd[120];
    sprintf(cmd, "zip -r %s/KompresProses%d.zip %s",dir, x, filename);
    system(cmd);
    char del[80];
    sprintf(del, "rm %s", filename);
    system(del);
}

void bikinFile(int x){
    char filename[50];
    char dir[30];
    sprintf(dir, "~/Documents/FolderProses%d", x);
    char cmd1[50];
    sprintf(cmd1,"mkdir -p %s", dir);
    system(cmd1);
    sprintf(filename, "%s/SimpanProses%d.txt", dir, x);
    char cmd2[70];
    sprintf(cmd2, "ps -ax | head -10 > %s", filename);
    system(cmd2);
}

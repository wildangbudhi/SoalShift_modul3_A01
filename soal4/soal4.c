#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_t tid[2];

void *makeDir1(void *arg){
    system("mkdir -p /home/hp/Documents/FolderProses1");
}
void *makeDir2(void *arg){
    system("mkdir -p /home/hp/Documents/FolderProses2");
}

void *saveFile1(void *arg){
    system("ps -aux --no-headers | head > /home/hp/Documents/FolderProses1/SimpanProses1.txt");
}
void *saveFile2(void *arg){
    system("ps -aux --no-headers | head > /home/hp/Documents/FolderProses2/SimpanProses2.txt");
}

void *compressFile1(void *arg){
    system("cd /home/hp/Documents/FolderProses1/ && zip -rq /home/hp/Documents/FolderProses1/KompresProses1.zip SimpanProses1.txt && rm SimpanProses1.txt");
}
void *compressFile2(void *arg){
    system("cd /home/hp/Documents/FolderProses2/ && zip -rq /home/hp/Documents/FolderProses2/KompresProses2.zip SimpanProses2.txt && rm SimpanProses2.txt");
}

void *extractFile1(void *arg){
    system("unzip -q /home/hp/Documents/FolderProses1/KompresProses1.zip -d /home/hp/Documents/FolderProses1/");
}
void *extractFile2(void *arg){
    system("unzip -q /home/hp/Documents/FolderProses2/KompresProses2.zip -d /home/hp/Documents/FolderProses2/");
}

int main(){
    pthread_create(&tid[0], NULL, &makeDir1, NULL);
    pthread_create(&tid[1], NULL, &makeDir2, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    pthread_create(&tid[0], NULL, &saveFile1, NULL);
    pthread_create(&tid[1], NULL, &saveFile2, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    pthread_create(&tid[0], NULL, &compressFile1, NULL);
    pthread_create(&tid[1], NULL, &compressFile2, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15); //waktu 15 detik

    pthread_create(&tid[0], NULL, &extractFile1, NULL);
    pthread_create(&tid[1], NULL, &extractFile2, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

 return 0;
}

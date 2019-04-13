#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT_PENJUAL 8080

int tambah();

int sockTambah;
char massage[50];

int main(int argc, char const *argv[]) {
    
    tambah();
    beli();
    char buffer[1024] = {0};

    while(1){
        gets(massage);

        if(!strcmp(massage, "tambah")){
            send(sockTambah , massage , strlen(massage) , 0 );
            printf("Tambah message sent\n");
        }
    }

    return 0;
}

int tambah(){
    struct sockaddr_in address;
    int valread;
    struct sockaddr_in serv_addr;
    if ((sockTambah = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_PENJUAL);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sockTambah, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

}
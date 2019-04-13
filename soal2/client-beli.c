#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT_PENJUAL 8080
#define PORT_PEMBELI 8000

int beli();

int sockBeli;
char massage[50];

int main(int argc, char const *argv[]) {
    
    tambah();
    beli();
    char buffer[1024] = {0};

    while(1){
        gets(massage);
        
        if(!strcmp(massage, "beli")){
            send(sockBeli , massage , strlen(massage) , 0 );
            printf("Beli message sent\n");
            read( sockBeli , buffer, 1024);
            printf("%s\n",buffer );
        }
    }

    return 0;
}

int beli(){
    struct sockaddr_in address;
    int valread;
    struct sockaddr_in serv_addr;
    if ((sockBeli = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_PEMBELI);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sockBeli, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

}
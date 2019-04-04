#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread, sock1;
    struct sockaddr_in serv_addr, serv_addr1;
    char buffer[1024] = {0};
    char permintaan[100];
    char *beli="beli";
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
   // if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
//	printf("\n Socket creation error \n");
 //       return -1;
  //  }

    memset(&serv_addr, '0', sizeof(serv_addr));
    //memset(&serv_addr1, '0', sizeof(serv_addr1));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.1.2");
    serv_addr.sin_port = htons(PORT);

//    serv_addr1.sin_family = AF_INET;
 //   serv_addr1.sin_addr.s_addr = INADDR_ANY;inet_addr("192.168.1.3");
  //  serv_addr1.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
   //if (connect(sock, (struct sockaddr *)&serv_addr1, sizeof(serv_addr1)) < 0) {
  //     printf("\nConnection Failed \n");
   //     return -1;
    //}

    while(1){

	    fgets(permintaan, 100, stdin);
	   // send(sock , permintaan , strlen(permintaan) , 0 );
	    if(strcmp(permintaan,beli)){
            	send(sock , permintaan , strlen(permintaan) , 0 );
	    	valread = read( sock , buffer, 1024);
	    	printf("%s\n",buffer );
	    }
	 //  else{
	  //  	send(sock1 , permintaan , strlen(permintaan) , 0 );
	   //}
    }

    return 0;
}

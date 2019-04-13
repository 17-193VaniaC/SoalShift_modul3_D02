#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
//shared mem
	key_t key = 1234;
        int *agmal, *iraj, *agcha, *ircha;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        agmal = shmat(shmid, NULL, 0);
	iraj = agmal+1;
	agcha = iraj+1;
	ircha = agcha+1;

        *agmal = 0;
	*iraj  = 100;
	*agcha = 0;
	*ircha = 0;



    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1,bangunnya=0,tidurnya=0;
    int addrlen = sizeof(address);
    char *ulang="Ulangi\n";
    char buffer[1024] = {0};
    char *agbang="Agmal Ayo Bangun\n";
    char *agbangdis="Agmal Ayo Bangun disabled for 10 s\n";
    char *irtidis="Iraj Ayo Tidur disabled for 10 s\n";
    char *irtid="Iraj Ayo Tidur\n";
    char *bangun="Agmal Terbangun, mereka bangun pagi dan berolahraga\n";
    char *tidur="Iraj ikut tidur dan bagun kesiangan bersama Agmal\n";
    //time

	clock_t mulaia, mulaib;
	double tima, timb;


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
   if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    while(1){
    valread = read( new_socket , buffer, 1024);
    if(!strncmp(buffer,agbang, 14)){
	if(*agcha==3){
	 *agcha=*agcha-3;
	 printf("agcha:%d\n",*agcha);
         send(new_socket , agbangdis , strlen(agbangdis) , 0 );
    /*     while(1){
		tima=((double)clock() - mulaia)/CLOCKS_PER_SEC;
		valread = read( new_socket , buffer, 1024);
		if(!strncmp(buffer,irtid, 14)){
		         if(*ircha==3){
		         *ircha=*ircha-3;
			 printf("ag-ircha:%d\n",*ircha);
		         send(new_socket , irtidis , strlen(irtidis) , 0 );
         	          }
		         else{
        	   	 *ircha=*ircha+1;
		         printf("ag-ircha:%d\n",*ircha);
		         send(new_socket , irtid , strlen(irtid) , 0 );
		         *iraj=*iraj-20;
    	        	  }
	        }
   		else{
       		send(new_socket , ulang , strlen(ulang) , 0 );
		}

		if(tima==10.0){
		break;
		}

	 }*/
	}
        else{
	 *agcha=*agcha+1;
	 send(new_socket , agbang , strlen(agbang) , 0 );
	 *agmal=*agmal+15;
	}
    }
    else if(!strncmp(buffer,irtid, 14)){
         if(*ircha==3){
	 printf("ircha:%d\n",*ircha);
	 *ircha=*ircha-3;
         send(new_socket , irtidis , strlen(irtidis) , 0 );
         }
         else{
         *ircha=*ircha+1;
 	 printf("ircha:%d\n",*ircha);
         send(new_socket , irtid , strlen(irtid) , 0 );
         *iraj=*iraj-20;
	 }
   }
   else{
	send(new_socket , ulang , strlen(ulang) , 0 );
   }


/*
   if(*agmal>=100 ){
   send(new_socket , bangun , strlen(bangun) , 0 );
   break;
   }
   else if(*iraj<=0){
   send(new_socket , tidur, strlen(tidur) , 0 );
   break;
   }*/
}
    return 0;

}


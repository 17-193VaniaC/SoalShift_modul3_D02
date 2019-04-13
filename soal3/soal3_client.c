#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h> 
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 8080

void *endhere1(void* ptr);
void *endhere2(void* ptr);

int main(int argc, char const *argv[]) {
//thread
	pthread_t thread1, thread2, thread3;
        int iret1, iret2, iret3;


//shared mem
 	key_t key = 1234;
        int *agmal, *iraj, *agcha, *ircha;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        agmal = shmat(shmid, NULL, 0);
	iraj = agmal+1;
        agcha = iraj+1;
        ircha = iraj+2;


    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *allstat="All Status";
    char pesan[1024]={0};
    char buffer[1024] = {0};
    char *allyes="All Status";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

   memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    iret2 = pthread_create( &thread2, NULL, endhere1, (void *) agmal);
        if(iret2)
        {
                fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
                exit(EXIT_FAILURE);
        }
    iret3 = pthread_create( &thread3, NULL, endhere2, (void *) iraj);
        if(iret3)
        {
                fprintf(stderr,"Error - pthread_create() return code: %d\n",iret3);
                exit(EXIT_FAILURE);
        }

while(1){
    fgets(pesan, 1024, stdin);
if(!strncmp(pesan, allyes, 9)){
    printf("~~~All Status~~~\n");
    printf("Agmal\t:%d\nIraj\t:%d\n",*agmal,*iraj);
}
else{
    send(sock , pesan , strlen(pesan) , 0 );
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
}
}

    return 0;
}

void *endhere1(void *ptr){
int *agm;
agm=(int *)ptr;
while(1){
   if(*agm>=100 ){
   printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
   exit(1);
 }
}
}

void *endhere2(void *ptr){
int *irj;
irj = (int *)ptr;
while(1){
   if(*irj<=0){
   printf("Iraj ikut tidur dan bagun kesiangan bersama Agmal\n");
   exit(1);
  }
 }
}

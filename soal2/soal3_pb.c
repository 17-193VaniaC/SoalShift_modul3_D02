//thread
#include <stdlib.h>
#include <pthread.h> 
#include <sys/types.h>
#include <sys/wait.h>

//shared mem
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
//server
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080


void *get_message( void * ptr);
void *print_stock( void * banyak);

int main(int argc, char const *argv[])
{	//thread
	pthread_t thread1, thread2;
	int iret1, iret2;

	//shared mem
        key_t key = 1234;
        int *value;

	//inisialisasi shared mem
        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

	//stok awal ada 10
        *value = 10;

	//thread 1 untuk nerima pesan client
	iret1 = pthread_create( &thread1, NULL, get_message, (void *) value);
	if(iret1)
        {
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        	exit(EXIT_FAILURE);
        }

	printf("pthread_create() for thread 1 returns: %d\n",iret1);
  
    pthread_join( thread1, NULL);

    exit(EXIT_SUCCESS);

}


void *get_message( void *ptr){

        char *beli="beli";
	int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt=1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};
	
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
        if (listen(server_fd, 1) < 0) {//listen cuma ke 1 client
                perror("listen");
                exit(EXIT_FAILURE);
        }

	 if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
                perror("accept");
                exit(EXIT_FAILURE);
        }

	int *tambahin;
	tambahin = (int *) ptr;
	char *bisa="transaksi berhasil\n";
	char *gbisa="transaksi gagal\n";
	while(1){
		valread = read( new_socket , buffer, 1024);
		if(strcmp(buffer,beli)){
			if(*tambahin>0){
			send(new_socket, bisa, strlen(bisa), 0);
			*tambahin=*tambahin-1;
			}
			else{
			send(new_socket, gbisa, strlen(gbisa), 0);
			}
		}
		
	}
}

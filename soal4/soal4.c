#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void *listing( void *ptr );
void *zipping( void *ptr );
void *unzipping( void *ptr);

int main()
{
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6;
    char *simpan1="ps -aux | head -n10 > ~/Documents/FolderProses1/SimpanProses1.txt";
    char *simpan2="ps -aux | head -n10 > ~/Documents/FolderProses2/SimpanProses2.txt";
    char *zip1="zip -rm ~/Documents/FolderProses1/KompresProses1.zip ~/Documents/FolderProses1/SimpanProses1.txt";
    char *zip2="zip -rm ~/Documents/FolderProses1/KompresProses2.zip ~/Documents/FolderProses2/SimpanProses2.txt";
    char *unzip1="unzip ~/Documents/FolderProses1/KompresProses1.zip";
    char *unzip2="unzip ~/Documents/FolderProses1/KompresProses2.zip";
    
    int  iret1, iret2, iret3, iret4, iret5, iret6;
   while(1){
    iret1 = pthread_create( &thread1, NULL, listing, (void*) simpan1); //membuat thr$
    if(iret1) //jika eror
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create( &thread2, NULL, listing, (void*) simpan2);//membuat thread k$
    if(iret2)//jika gagal
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL); 

    iret3 = pthread_create( &thread3, NULL, zipping, (void*) zip1); //membuat thr$
    if(iret3) //jika eror
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret3);
        exit(EXIT_FAILURE);
    }

    iret4 = pthread_create( &thread4, NULL, zipping, (void*) zip2);//membuat threa$
    if(iret4)//jika gagal
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret4);
        exit(EXIT_FAILURE);
    }

    pthread_join( thread3, NULL);
    pthread_join( thread4, NULL);

    iret5 = pthread_create( &thread5, NULL, unzipping, (void*) unzip1); //membuat thr$
    if(iret5) //jika eror
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret5);
        exit(EXIT_FAILURE);
    }

    iret6 = pthread_create( &thread6, NULL, unzipping, (void*) unzip2);//membuat threa$
    if(iret6)//jika gagal
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret6);
        exit(EXIT_FAILURE);
    }

    pthread_join( thread5, NULL);
    pthread_join( thread6, NULL);
}

    exit(EXIT_SUCCESS);

}

void *unzziping( void *ptr )
{   sleep(10);
    char *unzipnya;
    unzipnya= (char *) ptr;
    system(unzipnya);
}

void *listing( void *ptr )
{
    char *message;
    message = (char *) ptr;
    system(message);
}

void *zipping( void *ptr )
{   sleep(5);
    char *zippnya;
    zippnya = (char *) ptr;
    system(zippnya); 
    printf("Menunggu 15 detik lagi untuk mengekstrak kembali\n");
}



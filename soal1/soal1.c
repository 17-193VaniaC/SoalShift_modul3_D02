#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
int status;

typedef struct faktorialz{
int urut;
int isi;
//int hasil;
}Lala;


void *faktorial(void* nilai)
{   
    Lala * fak_loop = (Lala* )nilai;
    int hasil = (int) (intptr_t)nilai;
    int j;
    while(status<fak_loop->urut){}
    for (j=1; j<fak_loop->isi; j++)
    {
        hasil = hasil * j;
    }
    printf("thread id:\t%lu\n",pthread_self());
    printf("%d! = %d\n", fak_loop->isi, hasil);
    status++;
}


int main(int argc, char *array[])
{
    int j,i,a, arr[100];
    int *de;
    Lala huha[argc];
    for (i = 1; i < argc; i++)
    {
	int mbuh = atoi(array[i]);
	huha[i].isi = mbuh;
    }

for (i=1; i<argc; ++i)
       {
           for (j= i+1; j<argc; ++j)
           {
               if (huha[i].isi > huha[j].isi)
               {
                   int a =  huha[i].isi;
                   huha[i].isi = huha[j].isi;
                   huha[j].isi = a;
               }
           }
       }

    for (i = 1; i < argc; i++)
    {
        huha[i].urut = i;
    }

    status=1;
    int cr;
    pthread_t tid[argc];
    for (i = 1; i < argc; i++)
	{
	   cr=pthread_create(&(tid[i]), NULL, &faktorial, (void *) &huha[i]);
	}

	for (i = 1; i < argc; i++){

	    pthread_join(tid[i], NULL);//loopnya harusnya beda
	}/*
for(i=0;i<argc;i++){
       status=arr[i];
}
*/
}

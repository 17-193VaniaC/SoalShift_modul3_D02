#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

void *faktorial(void* nilai)
{   
    int fak_loop = (int) (intptr_t)nilai;
    int hasil = (int) (intptr_t)nilai;
    int j;

    for (j=1; j<fak_loop; j++)
    {
        hasil = hasil * j;
    }

    printf("%d! = %d\n", fak_loop, hasil);
}


int main(int argc, char *array[])
{
    int j,i,a, arr[100];
    int *de;

    for (i = 1; i < argc; i++)
    {
	int mbuh = atoi(array[i]);
	arr[i] = mbuh;
    }

 for (i=1; i<argc; ++i)
       {
           for (j= i+1; j<argc; ++j)
           {
               if (arr[i] > arr[j])
               {
                   int a =  arr[i];
                   arr[i] = arr[j];
                   arr[j] = a;
               }
           }
       }   

    int cr;
    pthread_t tid;

    for (i = 1; i < argc; i++)
	{

	    cr = pthread_create(&(tid), NULL, &faktorial, (void *) (intptr_t) arr[i]);
	    //printf("faqs2\n");
	    if(cr!=0) printf("\nPembuatan thread error\n");
	    pthread_join(tid, NULL);
	}
}


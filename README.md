Pembahasan 
1. #### Soal1
   Langkah pengerjaan:
   + Buat struktur data yang akan menyimpan urutan input dan nilainya.
```c
typedef struct faktorialz{
int urut;
int isi;
}Lala;

```

   + Buat fungsi main, masukkan input ke dalam sebuah array struktur data, urutkan dan masukkan urutannya juga.
   ```c
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
 
```
   + Buat status dan thread.
    
```c
    status=1;
    int cr;
    pthread_t tid[argc];
    for (i = 1; i < argc; i++)
    {
        cr=pthread_create(&(tid[i]), NULL, &faktorial, (void *) &huha[i]);
    }

```

   + Join semua thread
    
```c
for (i = 1; i < argc; i++)
    {

        pthread_join(tid[i], NULL);//loopnya harusnya beda
    }

```

   + Buat fungsi faktorial dan tampilkan nilainya.
    
```c
void *faktorial(void* nilai)
{
    Lala * fak_loop = (Lala* )nilai;
    int hasil = (int) (intptr_t)nilai;
    int j;
    while(status<fak_loop->urut) {}
    for (j=1; j<fak_loop->isi; j++)
    {
        hasil = hasil * j;
    }
    printf("thread id:\t%lu\n",pthread_self());
    printf("%d! = %d\n", fak_loop->isi, hasil);
    status++;
}

```
       
       
2. #### Soal2

Untuk Server Penjual:


Variabel yang digunakan adalah sebagai berikut
 ```
 //thread
	pthread_t thread1, thread2;
	int iret1, iret2;
	//shared mem
   key_t key = 1234;
   int *value;
```
agar bisa melakukan shared memory
```
        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);
   
	//stok awal ada 10
        *value = 10;
  ```      
  membuat thread ke-1 untuk nerima pesan dari client
  ```
	iret1 = pthread_create( &thread1, NULL, get_message, (void *) value);
	if(iret1)
        {
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        	exit(EXIT_FAILURE);
        }
  ```
  
	membuat thread ke-2 untuk menampilkan stock saat ini
  ```
        iret2 = pthread_create( &thread2, NULL, print_stock, (void *) value);
        if(iret2)
        {
       		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
                exit(EXIT_FAILURE);
        }
```

  join thread agar dijalankan hingga selesai
```
	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL); 

Membuat fungsi get_message yang digunakan untuk mendapatkan pesan dari client.

Pertama-tama, membuat socket untuk server penjual
```
void *get_message( void *ptr){

        char *tambah="tambah";
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
	
        if (listen(server_fd, 1) < 0) {//listen hanya ke 1 client
                perror("listen");
                exit(EXIT_FAILURE);
        }
	
	      if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
                perror("accept");
                exit(EXIT_FAILURE);
        }
    ```    
  membuat pointer bernama tambahin yang menunjuk ke variable dalam shared memory
  ```
	int *tambahin;
	tambahin = (int *) ptr;
  ```
  
  Thread ini akan terus membaca pesan yang dikirimkan oleh client yang terhubung dengan server penjual
  ```
	while(1){
		valread = read( new_socket , buffer, 1024); //membaca pesan dari client
		if(strcmp(buffer,tambah)){
				*tambahin=*tambahin+1;                  //menambahkan nilai 'value' dalam shared memory
		}
  ```
  Membuat fungsi print_stock yang digunakan untuk menampilkan banyak stok yang ada
 ```
  void *print_stock( void *banyak){
 
  int *stok;                  //membuat pointer yang menunjuk ke alamat 'value' yang ada di shared memory
	stok = (int *) banyak;      
	while(1){                   //Thread ini akan terus menampilkan banyak stok yang ada ('value') yang akan diupdate setelah 5       
		printf("%d \n",*stok);    //detik dan tampilan banyak stok sebelumnya akan dihilangkan
		sleep(5);
		system("clear");
```

Untuk Server Pembeli:

  pthread_t thread1, thread2;
	int iret1, iret2;

	//shared mem
        key_t key = 1234;
        int *value;
Agar bisa melakukan shared memory:
```
        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);
```
Membuat Thread untuk menerima pesan dari client yang terhubung
```
	iret1 = pthread_create( &thread1, NULL, get_message, (void *) value);
	if(iret1)
        {
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        	exit(EXIT_FAILURE);
        }

	  printf("pthread_create() for thread 1 returns: %d\n",iret1);
  
    pthread_join( thread1, NULL);
```

Membuat fungsi get_message yang digunakan untuk menerima pesan dari client
```
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
```

Membuat pointer yang menunjuk ke 'value ' yang merupakan variabel dalam shared memory
```
	int *tambahin;
	tambahin = (int *) ptr;
	char *bisa="transaksi berhasil\n";
	char *gbisa="transaksi gagal\n";
 	while(1){                                           
		valread = read( new_socket , buffer, 1024);   //membaca pesan dari client
		if(strcmp(buffer,beli)){                      //jika pesanyang diterima adalah "beli"
			if(*tambahin>0){                            //dan jika value di shared memory tidak nol
			send(new_socket, bisa, strlen(bisa), 0);    //maka server pembeli akan mengirimkan pesan berupa transaksi berhasil
			*tambahin=*tambahin-1;                      //nilai value di shared memory akan berkurang
			}
			else{                                       //jika value di shared memory kosong
			send(new_socket, gbisa, strlen(gbisa), 0);  //maka pembeli akan mengirimkan pesan berupa transaksi gagal
			}
		}
	}
}

```

Untuk Client Pembeli:

```
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

    memset(&serv_addr, '0', sizeof(serv_addr));
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {		//connect ke server
        printf("\nConnection Failed \n");
        return -1;
    }
```    
  
 Client dapat terus mengirimkan pesan ke server
 ```
    while(1){

	    fgets(permintaan, 100, stdin);				//mengambil string dari client
	 
	    if(strcmp(permintaan,beli)){
            	send(sock , permintaan , strlen(permintaan) , 0 );	//mengirimkan pesan ke server
	    	valread = read( sock , buffer, 1024);			//menerima pesan dari server
	    	printf("%s\n",buffer );					//menampilkan pesan dari server
	    }
 ```

Untuk Client Pembeli:
Hampir sama dengan client penjual
```
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char permintaan[100];
    char *tambah="tambah";
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
```
Perbedaan client penjual dan pembeli terletak di sini
```
    while(1){
	    fgets(permintaan, 100, stdin);				//client memasukan permintaan
	    if(strcmp(permintaan,tambah)){				//jika yang diminta adalah "tambah"
            	send(sock , permintaan , strlen(permintaan) , 0 );	//client mengirimkan permintaan 
	    }
}
 ``` 
3. Soal 3
4. #### Soal 4

	Membuat sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 
Dengan Syarat : 
➔	Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
➔	Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
➔	Ketika Mengekstrak file .zip juga harus secara bersama-sama
➔	Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik    untuk mengekstrak kembali”
➔	Menggunakan Multithreading

Berikut adalah fungsi yang digunakan untuk membuat thread:

void *listing( void *ptr ); = untuk membuat file berisi list dari proses (SimpanProses1.txt atau SimpanProses2.txt)
void *zipping( void *ptr ); = untuk men-zip file SimpanProses1.txt dan SimpanProses2.txt
void *mengunzip( void *ptr ); untuk mengekstrak file KompresProses1.zip dan KompresProses2.zip
Penjelasan dari isi fungsi main:

membuat 4 thread
```
    pthread_t thread1, thread2, thread3, thread4;
``` 

String untuk system pada thread
 ```
    char *simpan1="ps -aux | head -n10 > ~/Documents/FolderProses1/SimpanProses1.txt";
    char *simpan2="ps -aux | head -n10 > ~/Documents/FolderProses2/SimpanProses2.txt";
    char *zip1="zip -j -m ~/Documents/FolderProses1/KompresProses1.zip ~/Documents/FolderProses1/SimpanProses1.txt";
    char *zip2="zip -j -m ~/Documents/FolderProses2/KompresProses2.zip ~/Documents/FolderProses2/SimpanProses2.txt";
    char *unzip1="unzip ~/Documents/FolderProses1/KompresProses1.zip -d ~/Documents/FolderProses1/";
    char *unzip2="unzip ~/Documents/FolderProses2/KompresProses2.zip -d ~/Documents/FolderProses2/";
``` 
Dalam program ini dibuat 6 thread
Thread 1 dan 2 membuat file SimpanProses1.txt dan SimpanProses2.txt menggunakan fungsi listing
```
    int  iret1, iret2, iret3, iret4, iret5, iret6;
  
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
```
Thread 3 dan 4 menzip file SimpanProses1.txt dan SimpanProses2.txt menggunakan fungsi zipping
```
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

}
```
Thread 5 dan 6 mengunzip file KompresProses1.txt dan KompresProses2.txt menggunakan fungsi unzipping
```
iret5 = pthread_create( &thread5, NULL, mengunzip, (void*) unzip1); //membuat thr$
    if(iret5) //jika eror
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret5);
        exit(EXIT_FAILURE);
    }

    iret6 = pthread_create( &thread6, NULL, mengunzip, (void*) unzip2);//membuat threa$
    if(iret6)//jika gagal
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret6);
        exit(EXIT_FAILURE);
    }

pthread_join( thread5, NULL);
pthread_join( thread6, NULL);
```
Fungsi listing
```
void *listing( void *ptr )
{
    char *message;
    message = (char *) ptr;
    system(message);
}
```
Fungsi zipping: setelah memanggil fungsi sys
```
void *zipping( void *ptr )
{
    char *zippnya;
    zippnya = (char *) ptr;
    system(zippnya); 
    printf("Menunggu 15 detik lagi untuk mengekstrak kembali\n");
}
```
Fungsi mengunzip: thread yang menggunakan fungsi ini menunggu 15 setik sebelum mengekstrak kembali
```
void *mengunzip( void *ptr )
{   sleep(15);
    char *unzipnya;
    unzipnya= (char *) ptr;
    system(unzipnya);
}
```

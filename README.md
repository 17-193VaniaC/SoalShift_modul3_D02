Pembahasan 
1. soal1
2. soal2

Untuk Server Penjual:


Variabel yang digunakan:
  //thread
	pthread_t thread1, thread2;
	int iret1, iret2;
	//shared mem
   key_t key = 1234;
   int *value;

	agar bisa melakukan shared memory
  
  ```  
        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);
  ``` 
	//stok awal ada 10
        *value = 10;
        
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

Untuk Client Penjual:


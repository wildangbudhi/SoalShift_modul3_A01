# Laporan Soal Shift Modul 3
<br />

Daftar Isi
1. [NO1](#NO1)
2. [NO2](#NO2)
3. [NO3](#NO3)
4. [NO4](#NO4)
5. [NO5](#NO5)

## NO1
Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
	./faktorial 5 3 4
	3! = 6
	4! = 24
	5! = 120

### JAWAB: [Soal 1](/soal1/)

### PENJELASAN:
- Mengambil argument dalam CLI dengan menambahkan parameter pada fungsi main ```argc``` untuk jumlah argument sedangkan ```char** argv``` merupakan array of string (dalam c array of array of char) sebagai berikut:
  ```c
  int main(int argc, char** argv);
  ```
- Buat threads sebanyak ```argc``` yang menjalankan fungsi ```void *fact( void *ptr );``` untuk menghitung faktorial dan hasil diletakkan pada array ```data``` pada index sesuai dengan inputan dan mengambil data dari string argumen mulai dari index ke 1 karena index pertama merupakan nama filenya lalu kita ambil sebagai char dengan menyambil index ke 0 dari string tersebut lalu dilakukan pengurangan terhadap ```'0'``` untuk mendapatkan nilai dalam tipe Int.
  ```c
  for(i = 0; i < argc - 1; i++){
      const int arg = (int) argv[i+1][0] - '0';
      pthread_create(&threads[i], NULL, fact, (void *) arg);
  }
  ```
- Fungsi ```void *fact( void *ptr );```
  ```c
  void *fact( void *ptr )
  {
      int i, res, arg;
      arg = (int) ptr;
      res = 1;

      for(i = 1; i <= arg; i++) res *= i;

      data[arg] = res;
  }
  ```

- Dikarenakan bisa dipastikan bahwa array ```data``` pasti berurutan secara ascending maka untuk mencetak isinya tinggal kita lakukan traversing untuk mendapatkan data yang tidak ```0``` dikarenakan tidak ada hasil Faktorial yang menghasilkan ```0```.
  ```c
  for(i = 0; i < 100; i++)
      if(data[i] != 0) printf("%d! = %d\n", i, data[i]);
  ```

## NO2
Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
- Terdapat 2 server: server penjual dan server pembeli
- 1 server hanya bisa terkoneksi dengan 1 client
- Server penjual dan server pembeli memiliki stok barang yang selalu sama
- Client yang terkoneksi ke server penjual hanya bisa menambah stok
  - Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1
- Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
  - Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1
- Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
  - Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
  - Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”
- Server penjual akan mencetak stok saat ini setiap 5 detik sekali
- **Menggunakan thread, socket, shared memory**


### JAWAB: [Soal 2](/soal2/)

### PENJELASAN:
Terdapat 1 Client dan 2 Server (server-penjual dan server-pember)

**Client**
- Terdapat 2 fungsi yang mirip yaitu untuk menghubungkan ke server
  ```c
  // server-penjual

  int tambah(){
    struct sockaddr_in address;
    int valread;
    struct sockaddr_in serv_addr;
    if ((sockTambah = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_PENJUAL);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sockTambah, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

  }
  ```
  ```c
  // server-pembeli

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
  ```
- Lakukan pengiriman ke server jika sesuai
  ```c
  while(1){
      gets(massage);

      if(!strcmp(massage, "tambah")){
          send(sockTambah , massage , strlen(massage) , 0 );
          printf("Tambah message sent\n");
      }
      
      if(!strcmp(massage, "beli")){
          send(sockBeli , massage , strlen(massage) , 0 );
          printf("Beli message sent\n");
          read( sockBeli , buffer, 1024);
          printf("%s\n",buffer );
      }
    }
  ```



## NO3
Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:
- Terdapat 2 karakter Agmal dan Iraj
- Kedua karakter memiliki status yang unik
  - Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
  - Iraj memiliki Spirit_Status, di awal program memiliki status 100
  - Terdapat 3 Fitur utama
    - All Status, yaitu menampilkan status kedua sahabat
      Ex: Agmal WakeUp_Status = 75 
          Iraj Spirit_Status = 30
    - “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
    - “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
  - Terdapat Kasus yang unik dimana:
    - Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
    - Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
  - Program akan berhenti jika Salah Satu :
    - WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
    - Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)


  - **Syarat Menggunakan Lebih dari 1 Thread**


### JAWAB:

### PENJELASAN:


## NO4
Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 
Dengan Syarat : 
- Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
- Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
- Ketika Mengekstrak file .zip juga harus secara bersama-sama
- Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
- Wajib Menggunakan Multithreading
- Boleh menggunakan system


### JAWAB: [Soal 4](/soal4/)

### PENJELASAN:
1. Pertama, kita akan membuat sebuah thread yang nantinya akan menghasilkan folder ``FolderProses1`` dan ``FolderProses2`` apabila dijalankan.
```c
void *makeDir1(void *arg){
    system("mkdir -p /home/hp/Documents/FolderProses1");
}
void *makeDir2(void *arg){
    system("mkdir -p /home/hp/Documents/FolderProses2");
}
```
2. Untuk menyimpan data dari proses, kita akan membuat sebuah thread juga.
```c
void *saveFile1(void *arg){
    system("ps -aux --no-headers | head > /home/hp/Documents/FolderProses1/SimpanProses1.txt");
}
void *saveFile2(void *arg){
    system("ps -aux --no-headers | head > /home/hp/Documents/FolderProses2/SimpanProses2.txt");
}
```
3. Ketiga, membuat thread seperti di bawah ini yang berfungsi untuk mengkompres file.
```c
void *compressFile1(void *arg){
    system("cd /home/hp/Documents/FolderProses1/ && zip -rq /home/hp/Documents/FolderProses1/KompresProses1.zip SimpanProses1.txt && rm SimpanProses1.txt");
}
void *compressFile2(void *arg){
    system("cd /home/hp/Documents/FolderProses2/ && zip -rq /home/hp/Documents/FolderProses2/KompresProses2.zip SimpanProses2.txt && rm SimpanProses2.txt");
}
```
4. Lalu, membuat thread yang nantinya bisa mengekstrak file apabila dijalankan.
```c
void *extractFile1(void *arg){
    system("unzip -q /home/hp/Documents/FolderProses1/KompresProses1.zip -d /home/hp/Documents/FolderProses1/");
}
void *extractFile2(void *arg){
    system("unzip -q /home/hp/Documents/FolderProses2/KompresProses2.zip -d /home/hp/Documents/FolderProses2/");
}
```
Perlu diperhatikan bahwa kita harus menunggu selama 15 detik untuk mengekstrak file, kemudian untuk fungsi ``pthread_join(tid[0], NULL);`` harus selalu dipanggil agar perintah dapat dieksekusi oleh program secara bersamaan.

## NO5
Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta. 
- Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat  memberi nama pada monsternya.
- Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.
- Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).
- Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.
- Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian. 
- Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
  - Pembeli (terintegrasi dengan game)
    - Dapat mengecek stok makanan yang ada di toko.
    - Jika stok ada, pembeli dapat membeli makanan.
  - Penjual (terpisah)
    - Bisa mengecek stok makanan yang ada di toko
    - Penjual dapat menambah stok makanan.
<br />
Spesifikasi program:
- Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)
- Program terdiri dari 3 scene yaitu standby, battle, dan shop.
- Pada saat berada di standby scene, program selalu menampilkan health status, hunger status, hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit. Contoh :

  Standby Mode
  Health : [health status]
  Hunger : [hunger status]
  Hygiene : [hygiene status]
  Food left : [your food stock]
  Bath will be ready in [cooldown]s
  Choices
    - Eat
    - Bath
    - Battle
    - Shop
    - Exit

- Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. Contoh :

  Battle Mode
  Monster’s Health : [health status]
  Enemy’s Health : [enemy health status]
  Choices
    - Attack
    - Run

- Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene. Contoh :

  Shop Mode
  Shop food stock : [shop food stock]
  Your food stock : [your food stock]
  Choices
    - Buy
    - Back

- Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. Contoh :

  Shop
  Food stock : [shop food stock]
  Choices
    - Restock
    - Exit

- Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))

### JAWAB:

### PENJELASAN:

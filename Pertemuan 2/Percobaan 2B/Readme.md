# Jawaban Pertanyaan Percobaan 2B

## Soal 1: Mengapa alamat IP default ESP32 pada mode Access Point adalah 192.168.4.1?

**Jawaban:**
Alamat IP `192.168.4.1` merupakan konvensi bawaan dari framework ESP-IDF dan Arduino-ESP32 untuk mode _softAP_. Alamat ini menggunakan blok IP privat kelas C (`192.168.x.x`), tetapi memilih subnet `4.1` (bukan `0.1` atau `1.1` yang lazim dipakai oleh router rumah). Pemilihan subnet ini bertujuan meminimalkan risiko terjadinya tabrakan atau konflik alamat IP ketika ESP32 dioperasikan secara bersamaan dalam mode ganda (AP + STA). Nilai IP default ini juga dapat dikonfigurasi ulang secara manual menggunakan perintah `WiFi.softAPConfig()`.

## Soal 2: Jelaskan perbedaan mendasar antara mode Station (STA) dan Access Point (AP) pada ESP32

**Jawaban:**
Perbedaan mendasar antara kedua mode tersebut dapat dirangkum pada tabel berikut:

| Aspek | Station (STA) | Access Point (AP) |
| :--- | :--- | :--- |
| **Peran** | Bertindak sebagai klien (_client_) | Bertindak sebagai penyedia jaringan (_hotspot_) |
| **Ketergantungan** | Membutuhkan router/jaringan WiFi eksternal | Membuat jaringannya sendiri secara mandiri |
| **Sumber IP** | Menerima IP dari DHCP router (`WiFi.localIP()`) | Menentukan IP sendiri dan membagikannya ke klien (`WiFi.softAPIP()`) |
| **Kapasitas** | Terhubung ke 1 jaringan Access Point | Dapat melayani beberapa perangkat klien sekaligus |
| **Tujuan Penggunaan** | Mengakses internet, cloud, atau server lokal | _Provisioning_ awal, konfigurasi, atau kontrol nirkabel langsung |

## Soal 3: Apa risiko keamanan jika Access Point ESP32 dibiarkan tanpa password atau menggunakan password yang lemah?

**Jawaban:**
Siapa pun yang berada dalam jangkauan sinyal WiFi ESP32 dapat terhubung tanpa otorisasi. Hal ini membuka risiko penyadapan data (_man-in-the-middle_), manipulasi halaman konfigurasi atau web server yang di-host oleh ESP32 (sehingga kredensial WiFi rumah bisa dicuri atau diubah), serta potensi serangan _Denial of Service_ (DoS) di mana perangkat lain membanjiri antarmuka softAP dan menghabiskan kuota koneksi klien yang terbatas pada ESP32. Penggunaan password yang terlalu pendek atau mudah ditebak juga rentan terhadap serangan _brute-force_.

## Soal 4: Modifikasi program agar ESP32 dapat beroperasi dalam mode ganda (AP + STA) secara bersamaan, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md

**Jawaban:**
Program diubah dengan mengganti mode operasi menjadi `WiFi.mode(WIFI_AP_STA)` sehingga ESP32 dapat menjalankan fungsi Access Point sekaligus Station. Program mengaktifkan Access Point terlebih dahulu dengan `WiFi.softAP()` agar SSID AP ESP32 segera terpancar, kemudian menghubungkan Station ke router rumah dengan `WiFi.begin()`. Pada fungsi `loop()`, program memantau jumlah klien yang tersambung ke jaringan AP ESP32 serta status koneksi Station ke router rumah secara berkala.

[Perubahan Kode Percobaan 2B](percobaan2BMod.ino)

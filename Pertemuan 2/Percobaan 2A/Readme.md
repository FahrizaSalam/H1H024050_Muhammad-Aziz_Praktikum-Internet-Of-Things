# Jawaban Pertanyaan Percobaan 2A

## Soal 1: Gambarkan diagram alur (flowchart) proses koneksi WiFi pada program di atas

**Jawaban:**
Alurnya dimulai dari inisialisasi komunikasi serial (`Serial.begin(115200)`) dan konfigurasi pin LED sebagai output (`pinMode(ledPin, OUTPUT)`). Selanjutnya, program mengatur mode WiFi menjadi Station (`WiFi.mode(WIFI_STA)`) dan memulai proses koneksi dengan memanggil `WiFi.begin(ssid, password)`. Program kemudian masuk ke perulangan pengecekan status (`while (WiFi.status() != WL_CONNECTED)`), di mana program memberi jeda 500 ms dan mencetak tanda titik (`.`) ke Serial Monitor secara berulang sampai koneksi berhasil. Setelah status menjadi `WL_CONNECTED`, program mencetak informasi jaringan (IP Address, MAC Address, dan nilai RSSI) ke Serial Monitor serta menyalakan LED indikator (`digitalWrite(ledPin, HIGH)`). Pada fungsi `loop()`, program terus memantau status koneksi setiap 5 detik: jika tetap terhubung mencetak status terhubung, dan jika terputus mencetak status terputus serta mematikan LED indikator.

## Soal 2: Apa fungsi dari perintah WiFi.mode(WIFI_STA) pada program tersebut?

**Jawaban:**
Perintah `WiFi.mode(WIFI_STA)` berfungsi untuk mengatur radio WiFi ESP32 agar beroperasi dalam mode Station (STA) atau klien. Perintah ini menentukan antarmuka (_interface_) radio yang diaktifkan sebelum `WiFi.begin()` dipanggil, sehingga ESP32 dapat memindai, melakukan proses otentikasi, dan terhubung ke jaringan Access Point/router WiFi yang sudah ada guna memperoleh konfigurasi jaringan serta alamat IP dari DHCP router tersebut.

## Soal 3: Jelaskan apa yang akan terjadi apabila SSID atau password WiFi yang dimasukkan salah

**Jawaban:**
Jika SSID salah (jaringan tidak ditemukan), nilai status koneksi biasanya menjadi `WL_NO_SSID_AVAIL`. Jika password salah (SSID ditemukan tetapi proses otentikasi gagal), status umumnya bernilai `WL_CONNECT_FAILED`. Pada kode dasar yang belum diberi batas waktu atau jumlah percobaan, kedua kondisi ini mengakibatkan program terjebak selamanya di dalam perulangan `while (WiFi.status() != WL_CONNECTED)` dan terus mencetak tanda titik (`.`) tanpa henti. Akibatnya, baris kode setelah perulangan tersebut (pencetakan IP Address, MAC Address, RSSI, dan penyalaan LED) tidak akan pernah dieksekusi.

## Soal 4: Modifikasi program agar ESP32 dapat melakukan penyambungan ulang (auto-reconnect) secara otomatis jika koneksi terputus, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md

**Jawaban:**
Program diubah dengan memisahkan logika inisialisasi koneksi ke dalam fungsi `connectToWiFi()` dengan pembatasan percobaan maksimal 20 kali agar tidak menggantung tanpa batas, serta menambahkan logika _auto-reconnect_ non-blocking pada `loop()` menggunakan `millis()`. Jika koneksi terputus, program mematikan LED dan setiap interval 5 detik melakukan pemutusan sesi lama dengan `WiFi.disconnect()` lalu memanggil `WiFi.reconnect()` secara periodik hingga koneksi tersambung kembali tanpa menghentikan eksekusi kode lainnya.

[Perubahan Kode Percobaan 2A](percobaan2AMod.ino)

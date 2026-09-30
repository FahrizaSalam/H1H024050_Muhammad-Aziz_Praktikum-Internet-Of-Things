# Jawaban Pertanyaan Percobaan 4B

## Soal 1: Mengapa penggunaan delay() yang lama sebaiknya dihindari pada program yang menggabungkan proses publish dan subscribe secara bersamaan?

**Jawaban:**
Karena fungsi `delay()` bersifat memblokir (*blocking*), yang artinya akan menghentikan seluruh eksekusi program (termasuk penerimaan pesan) selama waktu tunda tersebut. Jika menggunakan `delay()`, fungsi penerima `client.loop()` tidak akan bisa dijalankan selama jeda tersebut, sehingga alat tidak bisa merespons perintah masuk secara *real-time* dan koneksi dengan broker bisa terputus (*timeout*).

## Soal 2: Jelaskan cara kerja mekanisme non-blocking menggunakan fungsi millis() pada program di atas!

**Jawaban:**
Mekanisme *non-blocking* memantau waktu berlalu tanpa harus membekukan kode. Program mencatat kapan aksi terakhir dijalankan dalam variabel `waktuTerakhirPublish`, dan terus membandingkan waktu sekarang `millis()` dengan catatan waktu sebelumnya. Jika selisihnya melewati ambang batas tertentu (misal 5000ms), maka perintah kirim data dijalankan dan waktu terbaru kembali dicatat. Semua proses selain cek IF akan berjalan mulus tak terhambat.

## Soal 3: Apa yang akan terjadi apabila fungsi client.loop() jarang dipanggil (misalnya hanya sekali setiap 10 detik)?

**Jawaban:**
Jika `client.loop()` jarang dipanggil, ESP32 akan kesulitan merespons pesan secara *real-time* (jeda merespons sangat lambat). Selain itu, karena PubSubClient butuh mengirim data *keep-alive* lewat perulangan tersebut, jeda 10 detik mungkin menyebabkan *broker* MQTT memutuskan koneksi server-klien dan menganggap ESP32 sudah putus koneksi (*offline*).

## Soal 4: Modifikasi program agar menambahkan satu topic perintah baru untuk mengendalikan aktuator kedua (misalnya buzzer), dengan fungsi callback yang dapat membedakan topic mana yang menerima pesan, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md!

**Jawaban:**
Penjelasan penambahan/modifikasi kode:
- Ditambahkan `const char* topicBuzzer = ...` untuk mendeklarasikan wujud koneksi/topic baru untuk buzzer.
- Ditambahkan `const int buzzerPin = 27;` untuk menghubungkan port hardware 27 di ESP32 pada aktuator Buzzer.
- Ditambahkan perintah `client.subscribe(topicBuzzer);` saat koneksi dihubungkan, agar MCU mengawasi topic baru tersebut.
- Ditambahkan `pinMode(buzzerPin, OUTPUT);` di *setup* agar MCU bisa mengontrol arus di GPIO 27.
- Pada callback, diberikan `if (String(topic) == topicPerintah)` dan percabangan turunannya `else if (String(topic) == topicBuzzer)` untuk mengetahui masuknya data berasal dari topic mana, dan menyesuaikan kendali pada pin LED atau Buzzer menggunakan fungsi ternary bersyarat `digitalWrite(buzzerPin, String(perintah) == "ON" ? HIGH : LOW);`.

[Perubahan Kode Percobaan 4B](Percobaan4BMod.ino)

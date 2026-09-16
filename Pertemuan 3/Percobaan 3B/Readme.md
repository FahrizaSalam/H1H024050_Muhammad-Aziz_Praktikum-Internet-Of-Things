# Jawaban Pertanyaan Percobaan 3B

## Soal 1: Apa fungsi dari topic pada protokol MQTT, dan mengapa topic yang digunakan perlu dibuat unik?

**Jawaban:**
Topic berfungsi sebagai alamat/kanal pesan agar broker tahu ke mana harus meneruskan data dari *publisher* ke *subscriber*. Topic harus unik agar data kita tidak tercampur dengan data orang lain yang menggunakan broker publik yang sama.

## Soal 2: Jelaskan fungsi dari perintah client.loop() yang dipanggil pada setiap iterasi loop()!

**Jawaban:**
Menjaga koneksi MQTT tetap aktif (*keep-alive*) dan memproses pesan masuk/keluar. Tanpa fungsi ini, koneksi akan terputus karena broker menganggap klien tidak merespons.

## Soal 3: Apa yang akan terjadi apabila koneksi ke broker MQTT terputus di tengah program berjalan?

**Jawaban:**
Pengiriman data gagal. Pada iterasi berikutnya, program akan mendeteksi putusnya koneksi dan memanggil `hubungkanMQTT()` untuk mencoba menyambung ulang sebelum mengirim data lagi.

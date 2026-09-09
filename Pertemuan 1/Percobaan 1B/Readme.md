# Jawaban Pertanyaan Percobaan 2A

## Soal 1: Mengapa diperlukan nilai ambang batas (threshold) dalam sistem kendali aktuator berbasis sensor?

**Jawaban:**
Ambang batas berfungsi sebagai titik acuan untuk mengubah data sensor yang bersifat kontinu (analog/numerik) menjadi keputusan kendali yang bersifat diskret (ON/OFF). Tanpa _threshold_, mikrokontroler tidak punya kriteria kapan harus mengaktifkan atau mematikan aktuator, sehingga sistem tidak bisa merespons kondisi lingkungan secara otomatis dan konsisten.

## Soal 2: Jelaskan apa yang akan terjadi apabila nilai suhuThreshold diturunkan menjadi sangat rendah, misalnya 20.0

**Jawaban:**
Karena suhu ruangan normal umumnya sudah di atas 20°C, aktuator akan hampir selalu dalam kondisi ON (menyala terus-menerus), bahkan saat kondisi lingkungan sebenarnya tidak memerlukan aktuasi. Ini membuat sistem kehilangan fungsinya sebagai kendali otomatis yang responsif terhadap perubahan suhu, dan pada aktuator nyata (relay ke beban listrik) bisa memboroskan energi atau mempercepat keausan komponen.

## Soal 3: Apa perbedaan antara kendali aktuator secara terus-menerus (kondisi tunggal) dengan kendali menggunakan histerisis (dua ambang batas)?

**Jawaban:**
Kendali kondisi tunggal langsung membandingkan suhu terhadap satu ambang batas setiap iterasi, sehingga jika suhu berosilasi tepat di sekitar nilai ambang tersebut, aktuator bisa menyala-mati berulang kali dalam waktu singkat (_flickering_) yang mempercepat keausan relay. Kendali histerisis menggunakan dua ambang batas (atas dan bawah) dengan status aktuator yang "diingat" antar-iterasi, sehingga aktuator hanya berubah status saat melewati salah satu batas secara jelas, dan tetap stabil selama suhu berada di zona antara kedua batas tersebut.

## Soal 4: Modifikasi program agar menggunakan dua ambang batas (histerisis), misalnya aktuator menyala pada suhu di atas 30°C dan baru mati pada suhu di bawah 28°C, dan berikan penjelasan di setiap baris kode nya dalam bentuk README.md

**Jawaban:**
Programnya diubah dengan menambahkan variabel status (`statusAktuator`) yang menyimpan kondisi aktuator antar-iterasi, lalu logika ON/OFF dipisah menjadi dua syarat: menyala hanya jika status sebelumnya mati dan suhu melewati batas atas (30°C), serta mati hanya jika status sebelumnya nyala dan suhu turun di bawah batas bawah (28°C). Di luar kedua kondisi itu, status tetap dipertahankan.

[Perubahan Kode Percobaan 2](Percobaan1BMod.ino)

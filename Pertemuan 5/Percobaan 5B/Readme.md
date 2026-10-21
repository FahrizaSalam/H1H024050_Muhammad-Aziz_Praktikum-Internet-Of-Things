# Jawaban Pertanyaan Percobaan 5B

## Soal 1: Jelaskan fungsi dari blok try-except pada fungsi on_message() dan apa yang terjadi apabila blok tersebut dihilangkan!

**Jawaban:**
Blok `try-except` pada fungsi `on_message()` berfungsi sebagai **penanganan error** saat memproses payload yang diterima dari MQTT. Blok `try` mencoba melakukan parsing JSON dan mengambil nilai `suhu` dari payload. Jika payload bukan format JSON yang valid (`ValueError`) atau tidak memiliki key `"suhu"` (`KeyError`), maka blok `except` akan menangkap error tersebut, mencetak pesan "Data tidak valid, diabaikan", dan menghentikan pemrosesan data tersebut dengan `return`.

Apabila blok `try-except` dihilangkan, program akan langsung **crash** (*runtime error*) ketika menerima pesan yang formatnya tidak sesuai (misalnya bukan JSON, atau JSON tanpa key `"suhu"`). Ini akan menghentikan seluruh program dan data yang sudah dikumpulkan sebelumnya bisa hilang karena belum disimpan ke CSV.

## Soal 2: Jelaskan arti nilai z-score dan mengapa batas |z| > 2 digunakan sebagai penentu anomali!

**Jawaban:**
**Z-score** menyatakan seberapa jauh suatu nilai data menyimpang dari rata-rata populasi, diukur dalam satuan simpangan baku. Rumusnya: `z = (x - rata-rata) / simpangan baku`. Nilai z-score = 0 berarti data sama dengan rata-rata, z-score = 1 berarti data 1 simpangan baku di atas rata-rata, dan seterusnya.

Batas `|z| > 2` digunakan karena berdasarkan distribusi normal, sekitar **95,4%** data berada dalam rentang ±2 simpangan baku dari rata-rata. Artinya, data dengan `|z| > 2` hanya memiliki probabilitas sekitar **4,6%** untuk muncul secara normal, sehingga layak dicurigai sebagai **anomali** atau data tidak wajar. Batas ini memberikan keseimbangan antara mendeteksi anomali yang signifikan tanpa terlalu banyak *false positive*.

## Soal 3: Apa kelemahan metode z-score apabila data memiliki tren naik atau turun yang tajam?

**Jawaban:**
Kelemahan utama metode z-score pada data dengan tren tajam adalah:
1. **Rata-rata tidak representatif**: Jika data memiliki tren naik/turun yang tajam, rata-rata akan tertarik ke arah tengah keseluruhan data, sehingga data di awal dan akhir tren akan memiliki z-score tinggi meskipun sebenarnya bukan anomali.
2. **Simpangan baku membesar**: Tren yang tajam menyebabkan variasi data yang besar secara keseluruhan, sehingga simpangan baku menjadi besar. Akibatnya, anomali sesungguhnya (lonjakan abnormal) bisa **tidak terdeteksi** karena z-score-nya menjadi kecil relatif terhadap simpangan baku yang sudah besar.
3. **Tidak membedakan tren dan anomali**: Z-score menganggap seluruh data sebagai satu kelompok stasioner, sehingga perubahan suhu yang konsisten (tren) bisa dianggap anomali, dan sebaliknya lonjakan sesaat di tengah tren bisa terlewat.

Solusi yang lebih tepat untuk data bertren adalah menggunakan z-score pada data yang sudah dihilangkan trennya (*detrended*), atau menggunakan metode deteksi anomali berbasis jendela waktu (*sliding window*).

## Soal 4: Modifikasi program agar menggunakan metode moving average (jendela 5 data) pada data hasil CSV sebelum dihitung z-score-nya, kemudian bandingkan jumlah anomali yang terdeteksi, dan berikan penjelasan di setiap baris kode yang ditambahkan dalam bentuk README.md.

**Jawaban:**
Penjelasan perubahan kode:
- Ditambahkan variabel `WINDOW = 5` sebagai ukuran jendela moving average.
- Ditambahkan kolom baru `suhu_ma` pada DataFrame menggunakan `df["suhu"].rolling(window=WINDOW, min_periods=1).mean()` yang menghitung rata-rata bergerak dari kolom suhu dengan jendela 5 data. Parameter `min_periods=1` memastikan data awal tetap dihitung meskipun belum ada 5 sampel.
- Perhitungan `rata2` dan `std` diubah agar menggunakan kolom `suhu_ma` (data yang sudah dihaluskan) bukan `suhu` mentah.
- Perhitungan z-score diubah menggunakan `suhu_ma` sehingga deteksi anomali berbasis data yang sudah difilter.
- Pada visualisasi, ditambahkan plot data mentah dengan transparansi (`alpha=0.5`) dan plot moving average dengan warna hijau serta penanda kotak (`marker="s"`), sehingga perbandingan terlihat jelas.
- Titik anomali diplot pada kurva moving average, bukan data mentah.

**Perbandingan jumlah anomali**: Dengan menggunakan moving average, jumlah anomali yang terdeteksi cenderung **lebih sedikit** dibanding tanpa moving average. Ini karena moving average menghaluskan fluktuasi acak, sehingga hanya lonjakan yang benar-benar signifikan dan konsisten yang terdeteksi sebagai anomali. *Noise* sesaat yang pada metode biasa bisa terdeteksi sebagai anomali, setelah dihaluskan oleh moving average tidak lagi melampaui batas z-score ±2.

[Perubahan Kode Percobaan 5B](Percobaan5BMod.py)

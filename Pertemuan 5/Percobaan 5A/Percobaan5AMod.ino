#include <DHT.h>                // library sensor DHT
#include <math.h>               // [MOD] library math untuk fungsi sqrt()

#define DHTPIN 4                // pin data sensor DHT22
#define DHTTYPE DHT22           // tipe sensor DHT22

DHT dht(DHTPIN, DHTTYPE);      // inisialisasi objek sensor DHT

const int N = 5;                // jumlah sampel moving average
float buffer[N];                // array penyimpan N pembacaan terakhir
int indeks = 0;                 // posisi penulisan berikutnya pada buffer
int jumlah = 0;                 // jumlah sampel yang sudah terisi

float suhuMin = 1000.0;         // variabel pencatat suhu minimum (awal sangat tinggi)
float suhuMax = -1000.0;        // variabel pencatat suhu maksimum (awal sangat rendah)

float hitungMovingAverage(float nilaiBaru) { // fungsi hitung rata-rata bergerak
  buffer[indeks] = nilaiBaru;   // simpan nilai baru ke posisi indeks saat ini
  indeks = (indeks + 1) % N;    // geser posisi indeks secara melingkar (circular)
  if (jumlah < N) jumlah++;     // tambah jumlah sampel jika belum penuh

  float total = 0;              // variabel penampung total nilai
  for (int i = 0; i < jumlah; i++) { // perulangan menjumlahkan semua sampel
    total += buffer[i];         // tambahkan nilai buffer ke total
  }
  return total / jumlah;        // kembalikan nilai rata-rata
}

float hitungSimpanganBaku() {   // [MOD] fungsi hitung simpangan baku dari buffer
  if (jumlah < 2) return 0.0;  // [MOD] minimal 2 data untuk simpangan baku

  float rata2 = 0;             // [MOD] variabel rata-rata
  for (int i = 0; i < jumlah; i++) { // [MOD] perulangan hitung total
    rata2 += buffer[i];        // [MOD] jumlahkan semua nilai buffer
  }
  rata2 = rata2 / jumlah;      // [MOD] bagi total dengan jumlah untuk rata-rata

  float totalKuadrat = 0;      // [MOD] variabel total selisih kuadrat
  for (int i = 0; i < jumlah; i++) { // [MOD] perulangan hitung selisih kuadrat
    float selisih = buffer[i] - rata2; // [MOD] hitung selisih tiap data dengan rata-rata
    totalKuadrat += selisih * selisih; // [MOD] kuadratkan dan jumlahkan
  }

  float varians = totalKuadrat / (jumlah - 1); // [MOD] hitung varians (sample variance)
  return sqrt(varians);         // [MOD] akar kuadrat varians = simpangan baku
}

void setup() {                  // konfigurasi awal
  Serial.begin(115200);         // baud rate serial 115200
  dht.begin();                  // mulai sensor DHT
}

void loop() {                   // perulangan utama
  float suhu = dht.readTemperature(); // baca suhu dari sensor DHT22

  if (isnan(suhu)) {            // jika pembacaan gagal (NaN)
    Serial.println("Pembacaan gagal, data diabaikan"); // cetak pesan error
  } else {                      // jika pembacaan valid
    float suhuFilter = hitungMovingAverage(suhu); // hitung moving average
    float stdDev = hitungSimpanganBaku(); // [MOD] hitung simpangan baku dari buffer

    if (suhu < suhuMin) suhuMin = suhu; // perbarui suhu minimum
    if (suhu > suhuMax) suhuMax = suhu; // perbarui suhu maksimum

    Serial.print("Mentah: ");   // cetak label data mentah
    Serial.print(suhu);         // cetak nilai suhu mentah
    Serial.print(" | Filter: "); // cetak label data filter
    Serial.print(suhuFilter);   // cetak nilai suhu hasil filter
    Serial.print(" | Min: ");   // cetak label minimum
    Serial.print(suhuMin);      // cetak nilai suhu minimum
    Serial.print(" | Max: ");   // cetak label maksimum
    Serial.print(suhuMax);      // cetak nilai suhu maksimum
    Serial.print(" | StdDev: "); // [MOD] cetak label simpangan baku
    Serial.println(stdDev);     // [MOD] cetak nilai simpangan baku
  }

  delay(2000);                  // jeda 2 detik sebelum pembacaan berikutnya
}

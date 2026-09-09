#include <DHT.h>
#define DHTPIN 4 // pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22 // tipe sensor yang digunakan
DHT dht(DHTPIN, DHTTYPE); // inisialisasi objek sensor DHT

void setup() {
  Serial.begin(115200); // inisialisasi komunikasi serial dengan baudrate 115200
  dht.begin(); // inisialisasi sensor DHT22
  Serial.println("Memulai akuisisi data sensor DHT22...");
}

void loop() {
  // Membaca data kelembaban dan suhu
  float kelembaban = dht.readHumidity(); // membaca nilai kelembaban udara (%)
  float suhu = dht.readTemperature(); // membaca nilai suhu dalam Celcius (°C)
  
  // Periksa apakah pembacaan berhasil
  if (isnan(kelembaban) || isnan(suhu)) { // mengecek apakah hasil pembacaan bernilai NaN (Not a Number)
    Serial.println("Gagal membaca data dari sensor DHT22!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu); // menampilkan nilai suhu ke Serial Monitor
    Serial.print(" °C, Kelembaban: ");
    Serial.print(kelembaban); // menampilkan nilai kelembaban ke Serial Monitor
    Serial.println(" %");
  }
  
  delay(2000); // jeda pembacaan setiap 2 detik
}

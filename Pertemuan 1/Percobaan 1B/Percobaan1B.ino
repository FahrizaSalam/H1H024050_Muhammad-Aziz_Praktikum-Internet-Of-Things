#include <DHT.h>
#define DHTPIN D4 // pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22 // tipe sensor yang digunakan
#define RELAYPIN D1 // pin kendali relay/LED indikator
DHT dht(DHTPIN, DHTTYPE); // inisialisasi objek sensor DHT

const float suhuThreshold = 30.0; // ambang batas suhu (°C)

void setup() {
  Serial.begin(115200); // inisialisasi komunikasi serial dengan baudrate 115200
  dht.begin(); // inisialisasi sensor DHT22
  pinMode(RELAYPIN, OUTPUT); // mengatur pin relay/LED sebagai output
  digitalWrite(RELAYPIN, LOW); // pastikan aktuator mati di awal
}

void loop() {
  float suhu = dht.readTemperature(); // membaca nilai suhu dari sensor DHT22
  
  if (isnan(suhu)) { // mengecek apakah hasil pembacaan suhu bernilai NaN (Not a Number)
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu); // menampilkan nilai suhu ke Serial Monitor
    Serial.print(" °C -> ");
    
    // Kendali aktuator berdasarkan hasil akuisisi data sensor
    if (suhu > suhuThreshold) { // mengecek apakah suhu melebihi ambang batas
      digitalWrite(RELAYPIN, HIGH); // aktifkan relay/LED
      Serial.println("Aktuator: ON");
    } else { // jika suhu berada di bawah atau sama dengan ambang batas
      digitalWrite(RELAYPIN, LOW); // matikan relay/LED
      Serial.println("Aktuator: OFF");
    }
  }
  
  delay(2000); // jeda antar pembacaan sensor (2 detik)
}

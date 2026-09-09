#include <DHT.h>
#define DHTPIN 4 // pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22
#define RELAYPIN 26 // pin kendali relay/LED indikator
DHT dht(DHTPIN, DHTTYPE);

const float batasAtas = 30.0;  // suhuThreshold tunggal diganti jadi dua ambang batas (ini batas atas)
const float batasBawah = 28.0; // ambang batas bawah, tempat aktuator dimatikan

bool statusAktuator = false; // menyimpan status aktuator saat ini agar "diingat" antar-loop

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); // pastikan aktuator mati di awal
}

void loop() {
  float suhu = dht.readTemperature();
  
  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // Kendali aktuator dengan histerisis (dua ambang batas)
    if (!statusAktuator && suhu > batasAtas) {
      statusAktuator = true; // nyalakan hanya jika sebelumnya mati DAN suhu melewati batas atas
    } else if (statusAktuator && suhu < batasBawah) {
      statusAktuator = false; // matikan hanya jika sebelumnya nyala DAN suhu turun di bawah batas bawah
    }
    // jika suhu di antara batasBawah dan batasAtas, status TIDAK diubah (inti dari histerisis)

    digitalWrite(RELAYPIN, statusAktuator ? HIGH : LOW); // terapkan variabel status ke pin relay
    Serial.println(statusAktuator ? "Aktuator: ON" : "Aktuator: OFF"); // tampilkan status dari variabel
  }

  delay(2000); // jeda antar pembacaan sensor
}

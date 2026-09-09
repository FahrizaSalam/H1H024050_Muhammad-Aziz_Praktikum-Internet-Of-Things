#include <WiFi.h>

const char* ssid     = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";

const int ledPin = 2;  // LED indikator status koneksi

unsigned long previousMillis = 0;                 // variabel pencatat waktu terakhir percobaan reconnect
const long reconnectInterval = 5000;              // jeda antar percobaan reconnect (ms)

// Fungsi baru: memisahkan logika koneksi WiFi agar bisa dipanggil ulang
void connectToWiFi() {
    Serial.print("Menghubungkan ke WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int attempt = 0;                              // penghitung jumlah percobaan koneksi
    while (WiFi.status() != WL_CONNECTED && attempt < 20) {  // batasi maks 20 percobaan (tidak menggantung selamanya)
        delay(500);
        Serial.print(".");
        attempt++;                                // tambah counter percobaan
    }

    if (WiFi.status() == WL_CONNECTED) {          // cek hasil koneksi alih-alih langsung asumsikan berhasil
        Serial.println();
        Serial.println("WiFi berhasil terhubung!");
        Serial.print("IP Address  : ");
        Serial.println(WiFi.localIP());
        Serial.print("MAC Address : ");
        Serial.println(WiFi.macAddress());
        Serial.print("RSSI (dBm)  : ");
        Serial.println(WiFi.RSSI());
        digitalWrite(ledPin, HIGH);
    } else {                                      // tangani kasus gagal koneksi awal
        Serial.println();
        Serial.println("Gagal terhubung pada percobaan awal, akan dicoba lagi di loop utama.");
        digitalWrite(ledPin, LOW);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    connectToWiFi();                              // panggil fungsi koneksi WiFi yang sudah dipisah
}

void loop() {
    unsigned long currentMillis = millis();        // catat waktu sekarang untuk logika non-blocking

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Status: Terhubung");
        digitalWrite(ledPin, HIGH);               // nyalakan LED saat terhubung (pembaruan status real-time)
    } else {
        Serial.println("Status: Terputus");
        digitalWrite(ledPin, LOW);

        // Logika auto-reconnect non-blocking menggunakan millis()
        if (currentMillis - previousMillis >= reconnectInterval) {  // cek apakah sudah waktunya mencoba ulang
            previousMillis = currentMillis;       // perbarui waktu terakhir percobaan
            Serial.println("Mencoba menyambungkan ulang ke WiFi...");  // informasi reconnect ke serial
            WiFi.disconnect();                    // putuskan koneksi lama sebelum mencoba ulang
            WiFi.reconnect();                     // coba sambungkan kembali ke WiFi
        }
    }

    delay(5000);
}

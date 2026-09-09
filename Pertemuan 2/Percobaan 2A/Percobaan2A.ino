#include <WiFi.h> // memanggil library WiFi bawaan ESP32

const char* ssid     = "NAMA_WIFI_ANDA";     // SSID (nama) jaringan WiFi yang dituju
const char* password = "PASSWORD_WIFI_ANDA"; // kata sandi jaringan WiFi yang dituju

// LED indikator status koneksi
const int ledPin = 2; // pin GPIO 2 untuk LED indikator

void setup() {
    Serial.begin(115200); // inisialisasi komunikasi serial pada baudrate 115200
    pinMode(ledPin, OUTPUT); // mengatur pin LED sebagai output
    digitalWrite(ledPin, LOW); // memastikan LED mati di awal program

    // Set mode WiFi menjadi Station
    WiFi.mode(WIFI_STA); // mengatur ESP32 beroperasi dalam mode Station (klien)
    WiFi.begin(ssid, password); // memulai proses koneksi ke jaringan WiFi

    Serial.print("Menghubungkan ke WiFi");
    // Menunggu hingga ESP32 berhasil terhubung ke WiFi
    while (WiFi.status() != WL_CONNECTED) { // cek status koneksi secara berulang
        delay(500); // jeda 500 ms antar pengecekan
        Serial.print("."); // mencetak titik sebagai indikator proses
    }

    // Jika berhasil terhubung, tampilkan informasi jaringan
    Serial.println();
    Serial.println("WiFi berhasil terhubung!");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP()); // menampilkan alamat IP lokal dari DHCP router
    Serial.print("MAC Address : ");
    Serial.println(WiFi.macAddress()); // menampilkan alamat fisik (MAC) ESP32
    Serial.print("RSSI (dBm) : ");
    Serial.println(WiFi.RSSI()); // menampilkan kekuatan sinyal WiFi yang diterima

    digitalWrite(ledPin, HIGH); // nyalakan LED sebagai indikator koneksi berhasil
}

void loop() {
    // Cek status koneksi setiap 5 detik
    if (WiFi.status() == WL_CONNECTED) { // mengecek apakah koneksi WiFi masih terhubung
        Serial.println("Status: Terhubung"); // menampilkan status jika terhubung
    } else {
        Serial.println("Status: Terputus"); // menampilkan pesan jika koneksi terputus
        digitalWrite(ledPin, LOW); // mematikan LED jika koneksi terputus
    }
    delay(5000); // jeda pengecekan status setiap 5 detik
}
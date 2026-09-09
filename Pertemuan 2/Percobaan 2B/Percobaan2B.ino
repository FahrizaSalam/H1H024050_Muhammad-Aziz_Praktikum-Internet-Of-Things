#include <WiFi.h> // memanggil library WiFi bawaan ESP32

// Kredensial Access Point (AP) yang akan dibuat oleh ESP32
const char* ap_ssid = "ESP32_AccessPoint"; // SSID (nama hotspot) yang dipancarkan ESP32
const char* ap_password = "12345678";      // kata sandi AP (minimal 8 karakter untuk enkripsi WPA2)

void setup() {
    Serial.begin(115200); // inisialisasi komunikasi serial pada baudrate 115200

    // Set mode WiFi menjadi Access Point
    WiFi.mode(WIFI_AP); // mengatur ESP32 beroperasi dalam mode Access Point (hotspot)
    WiFi.softAP(ap_ssid, ap_password); // mengaktifkan softAP dengan SSID dan password yang ditentukan

    IPAddress apIP = WiFi.softAPIP(); // mengambil alamat IP Access Point ESP32
    Serial.println("Access Point aktif!");
    Serial.print("SSID : ");
    Serial.println(ap_ssid); // menampilkan nama SSID Access Point ke Serial Monitor
    Serial.print("IP Address : ");
    Serial.println(apIP); // menampilkan alamat IP default Access Point (192.168.4.1)
}

void loop() {
    // Menampilkan jumlah perangkat yang terhubung setiap 5 detik
    int jumlahClient = WiFi.softAPgetStationNum(); // membaca jumlah stasiun/klien yang sedang terhubung ke ESP32
    Serial.print("Jumlah perangkat terhubung: ");
    Serial.println(jumlahClient); // mencetak jumlah klien ke Serial Monitor
    delay(5000); // jeda pembacaan dan pemantauan setiap 5 detik
}
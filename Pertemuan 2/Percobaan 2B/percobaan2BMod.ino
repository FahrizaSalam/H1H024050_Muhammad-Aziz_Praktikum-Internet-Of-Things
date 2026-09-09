#include <WiFi.h>

// Kredensial WiFi rumah (ditambahkan untuk mode Station)
const char* sta_ssid     = "NAMA_WIFI_ANDA";      // SSID WiFi rumah untuk koneksi Station
const char* sta_password = "PASSWORD_WIFI_ANDA";   // password WiFi rumah untuk koneksi Station

// Kredensial Access Point milik ESP32
const char* ap_ssid     = "ESP32_AccessPoint";
const char* ap_password = "12345678";  // minimal 8 karakter

void setup() {
    Serial.begin(115200);

    // Set mode gabungan: ESP32 berperan sebagai AP sekaligus STA (sebelumnya hanya WIFI_AP)
    WiFi.mode(WIFI_AP_STA);                        // ganti WIFI_AP menjadi WIFI_AP_STA

    // 1) Aktifkan Access Point terlebih dahulu
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("Access Point aktif!");
    Serial.print("AP SSID       : ");              // tambah prefix "AP" pada label agar jelas
    Serial.println(ap_ssid);
    Serial.print("AP IP Address : ");              // tambah prefix "AP" pada label agar jelas
    Serial.println(WiFi.softAPIP());

    // 2) Mulai koneksi Station ke jaringan WiFi rumah (seluruh blok ini baru)
    WiFi.begin(sta_ssid, sta_password);            // mulai koneksi Station ke WiFi rumah
    Serial.print("Menghubungkan ke WiFi rumah");   // informasi proses koneksi Station
    while (WiFi.status() != WL_CONNECTED) {        // tunggu sampai Station terhubung
        delay(500);                                // jeda 500ms antar percobaan
        Serial.print(".");                         // indikator proses di serial
    }
    Serial.println();                              // baris baru setelah titik-titik
    Serial.println("Berhasil terhubung ke WiFi rumah!");  // konfirmasi koneksi Station berhasil
    Serial.print("STA IP Address : ");             // tampilkan IP Address mode Station
    Serial.println(WiFi.localIP());                // cetak IP Address Station
}

void loop() {
    // Pantau jumlah perangkat yang terhubung ke Access Point ESP32
    int jumlahClient = WiFi.softAPgetStationNum();
    Serial.print("Jumlah perangkat terhubung ke AP : ");  // label diperjelas dengan " ke AP"
    Serial.println(jumlahClient);

    // Pantau status koneksi Station ke WiFi rumah (seluruh blok ini baru)
    Serial.print("Status koneksi ke WiFi rumah     : ");  // tampilkan status koneksi Station
    Serial.println(WiFi.status() == WL_CONNECTED ? "Terhubung" : "Terputus");  // ternary untuk status Station

    Serial.println("-----");                       // garis pemisah agar output lebih rapi
    delay(5000);
}

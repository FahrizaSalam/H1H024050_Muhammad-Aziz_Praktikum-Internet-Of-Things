#include <ESP8266WiFi.h>         // library WiFi ESP8266
#include <ESP8266HTTPClient.h>   // library HTTP request ESP8266
#include <WiFiClientSecure.h>    // library untuk HTTPS
#include <ArduinoJson.h>         // library ArduinoJson

const char* ssid     = "POCO X5 5G";     // nama WiFi
const char* password = "cobaliathplu";   // password WiFi
const char* serverUrl = "https://httpbin.org/post";  // endpoint HTTP POST

void setup() {
  Serial.begin(115200); // inisialisasi serial

  WiFi.begin(ssid, password); // mulai koneksi WiFi

  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) { // tunggu hingga terhubung
    delay(500); // jeda 500ms
    Serial.print("."); // indikator proses
  }
  Serial.println();
  Serial.println("WiFi berhasil terhubung!"); // konfirmasi koneksi
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { // pastikan WiFi terhubung
    WiFiClientSecure client; // objek WiFi secure
    client.setInsecure(); // abaikan verifikasi sertifikat SSL

    HTTPClient http; // objek HTTPClient
    
    // Perbaikan: Sertakan client pada http.begin()
    http.begin(client, serverUrl); // inisialisasi koneksi ke server
    http.addHeader("Content-Type", "application/json"); // header format JSON

    // Buat data JSON
    JsonDocument doc; // objek JSON
    doc["suhu"] = 28.5;         // data suhu
    doc["kelembaban"] = 65.0;   // data kelembaban

    String requestBody; // variabel string JSON
    serializeJson(doc, requestBody); // konversi JSON ke string

    Serial.print("Mengirim data: ");
    Serial.println(requestBody); // tampilkan data JSON

    // Kirim via HTTP POST
    int httpResponseCode = http.POST(requestBody); // kirim request dan simpan kode

    if (httpResponseCode > 0) { // jika respons berhasil
      Serial.print("Kode Response HTTP: ");
      Serial.println(httpResponseCode); // tampilkan kode HTTP
      Serial.println("Isi Response:");
      Serial.println(http.getString()); // tampilkan body respons
    } else { // jika gagal
      Serial.print("Pengiriman gagal, kode error: ");
      Serial.println(httpResponseCode); // tampilkan error
    }

    http.end(); // tutup koneksi
  }

  delay(10000);  // jeda 10 detik
}
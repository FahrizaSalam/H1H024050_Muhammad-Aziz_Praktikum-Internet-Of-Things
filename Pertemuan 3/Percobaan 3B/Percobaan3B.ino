#include <ESP8266WiFi.h>      // library WiFi ESP8266
#include <WiFiClientSecure.h> // library untuk HTTPS/TLS
#include <PubSubClient.h>     // library MQTT
#include <ArduinoJson.h>      // library ArduinoJson

// Konfigurasi WiFi
const char* ssid = "POCO X5 5G";       // nama WiFi
const char* password = "cobaliathplu"; // password WiFi

// Konfigurasi HiveMQ Cloud
const char* mqttServer = "97904deac669490f8ece9602e0e13b99.s1.eu.hivemq.cloud"; // host broker
const int mqttPort = 8883; // port broker

// Kredensial HiveMQ Cloud
const char* mqttUsername = "Kelompok7"; // username broker
const char* mqttPassword = "Kelompok7"; // password broker
const char* mqttTopic = "unsoed/tk245004/kelompok7/sensor"; // topik MQTT

WiFiClientSecure espClient; // objek WiFi secure
PubSubClient client(espClient); // objek MQTT client

// Fungsi menghubungkan WiFi
void hubungkanWiFi() {
  WiFi.begin(ssid, password); // mulai koneksi WiFi
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) { // tunggu hingga terhubung
    delay(500); // jeda 500ms
    Serial.print("."); // indikator proses
  }
  Serial.println();
  Serial.println("WiFi berhasil terhubung!"); // konfirmasi koneksi
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // tampilkan IP
}

// Fungsi menghubungkan MQTT
void hubungkanMQTT() {
  while (!client.connected()) { // selama belum terhubung ke broker
    Serial.println();
    Serial.print("Menghubungkan ke broker MQTT...");
    String clientId = "ESP8266Client-" + String(ESP.getChipId(), HEX); // buat ID unik
    Serial.print(" Client ID: ");
    Serial.println(clientId); // tampilkan ID

    // Coba koneksi ke broker
    if (client.connect(clientId.c_str(), mqttUsername, mqttPassword)) {
      Serial.println("MQTT berhasil terhubung!"); // konfirmasi koneksi
      Serial.print("Broker : ");
      Serial.println(mqttServer); // tampilkan broker
      Serial.print("Port   : ");
      Serial.println(mqttPort); // tampilkan port
    } else {
      Serial.print("MQTT gagal, rc=");
      Serial.println(client.state()); // tampilkan kode error
      Serial.println("Mencoba lagi dalam 2 detik...");
      delay(2000); // jeda sebelum coba lagi
    }
  }
}

void setup() {
  Serial.begin(115200); // inisialisasi serial
  delay(1000); // jeda awal
  Serial.println("\n ESP8266 MQTT - HiveMQ Cloud");

  hubungkanWiFi(); // panggil fungsi koneksi WiFi

  espClient.setInsecure(); // abaikan validasi sertifikat TLS
  client.setServer(mqttServer, mqttPort); // atur server MQTT

  hubungkanMQTT(); // panggil fungsi koneksi MQTT
}

void loop() {
  if (!client.connected()) { // jika koneksi MQTT terputus
    Serial.println("\nMQTT terputus!");
    hubungkanMQTT(); // hubungkan kembali
  }
  client.loop(); // wajib dipanggil untuk menjaga koneksi MQTT

  // Buat data JSON
  JsonDocument doc; // objek JSON
  doc["suhu"] = 28.5;       // data suhu
  doc["kelembaban"] = 65.0; // data kelembaban

  char buffer[128]; // buffer untuk JSON string
  serializeJson(doc, buffer); // konversi JSON ke string

  // Kirim data ke topik MQTT
  bool berhasil = client.publish(mqttTopic, buffer);

  if (berhasil) { // jika publish berhasil
    Serial.println("\nData berhasil dikirim!");
    Serial.print("Topic   : ");
    Serial.println(mqttTopic); // tampilkan topik
    Serial.print("Payload : ");
    Serial.println(buffer); // tampilkan payload data
  } else { // jika gagal
    Serial.println("\nGagal mengirim data!");
  }

  delay(5000); // jeda 5 detik sebelum publish lagi
}
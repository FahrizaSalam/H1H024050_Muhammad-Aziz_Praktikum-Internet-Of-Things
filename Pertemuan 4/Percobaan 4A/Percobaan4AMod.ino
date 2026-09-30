#include <WiFi.h>                // library WiFi
#include <PubSubClient.h>        // library MQTT client
#include <ArduinoJson.h>         // library JSON

const char* ssid = "Atharva";     // nama WiFi
const char* password = "starlink"; // password WiFi

const char* mqttServer = "broker.hivemq.com"; // alamat broker MQTT
const int mqttPort = 1883;                     // port broker MQTT
const char* topicPerintah = "unsoed/tk245004/kelompokAnda/perintah"; // topic subscribe

const int ledPin = 2;                 // pin aktuator LED
const bool LED_ACTIVE_LOW = true;     // true untuk LED bawaan; false untuk LED eksternal ke GND

WiFiClient espClient; // objek koneksi WiFi
PubSubClient client(espClient); // objek klien MQTT

void callback(char* topic, byte* payload, unsigned int length) { // fungsi terima pesan
  String pesan; // variabel penyimpan pesan
  for (unsigned int i = 0; i < length; i++) { // perulangan membaca byte
    pesan += (char)payload[i]; // konversi byte ke karakter
  }
  
  Serial.print("Pesan diterima ["); // cetak info topic
  Serial.print(topic); // cetak nama topic
  Serial.print("]: "); // cetak pemisah
  Serial.println(pesan); // cetak isi pesan

  JsonDocument doc; // objek JSON
  DeserializationError error = deserializeJson(doc, pesan); // parsing pesan JSON

  if (error) { // jika parsing gagal
    Serial.print("Gagal parsing JSON: "); // cetak error
    Serial.println(error.c_str()); // keterangan error
    return; // hentikan proses callback
  }

  const char* perintah = doc["perintah"]; // ambil nilai perintah
  
  int intensitas = 255; // nilai default intensitas (terang maksimal)
  if (doc.containsKey("intensitas")) { // periksa jika ada data intensitas
    intensitas = doc["intensitas"]; // ambil nilai intensitas dari JSON
  }
  
  if (String(perintah) == "ON") { // jika perintah ON
    analogWrite(ledPin, intensitas); // nyalakan LED dengan intensitas PWM
    Serial.print("Aktuator: ON, Intensitas: "); // cetak status dan intensitas
    Serial.println(intensitas); // tampilkan nilai intensitas
  } else if (String(perintah) == "OFF") { // jika perintah OFF
    analogWrite(ledPin, 0); // matikan LED dengan intensitas 0
    Serial.println("Aktuator: OFF"); // cetak status
  }
}

void hubungkanWiFi() { // fungsi koneksi WiFi
  WiFi.begin(ssid, password); // mulai koneksi WiFi
  Serial.print("Menghubungkan ke WiFi"); // cetak info proses
  while (WiFi.status() != WL_CONNECTED) { // tunggu sampai terhubung
    delay(500); // jeda 500ms
    Serial.print("."); // cetak indikator loading
  }
  Serial.println("\nWiFi berhasil terhubung!"); // cetak koneksi sukses
}

void hubungkanMQTT() { // fungsi koneksi broker MQTT
  while (!client.connected()) { // cek jika belum terhubung
    Serial.print("Menghubungkan ke broker MQTT..."); // cetak info koneksi
    String clientId = "ESP32Client-" + String(random(0xffff), HEX); // buat ID klien unik
    
    if (client.connect(clientId.c_str())) { // mencoba terhubung
      Serial.println("berhasil terhubung!"); // cetak berhasil
      client.subscribe(topicPerintah); // subscribe ke topic perintah
      Serial.print("Subscribe ke topic: "); // cetak info subscribe
      Serial.println(topicPerintah); // nama topic yang disubscribe
    } else { // jika gagal terhubung
      Serial.print("gagal, rc="); // cetak gagal
      Serial.print(client.state()); // kode state MQTT
      Serial.println(" coba lagi dalam 2 detik"); // info coba ulang
      delay(2000); // jeda 2 detik
    }
  }
}

void setup() { // konfigurasi awal
  Serial.begin(115200); // baud rate serial
  pinMode(ledPin, OUTPUT); // inisiasi pin sebagai output
  analogWrite(ledPin, 0); // pastikan LED mati awal menggunakan PWM

  hubungkanWiFi(); // panggil fungsi koneksi WiFi
  client.setServer(mqttServer, mqttPort); // atur server dan port MQTT
  client.setCallback(callback); // daftarkan fungsi penangkap pesan
}

void loop() { // perulangan utama
  if (!client.connected()) { // periksa apakah koneksi MQTT putus
    hubungkanMQTT(); // hubungkan kembali jika putus
  }
  client.loop(); // jaga koneksi dan tangkap pesan masuk
}

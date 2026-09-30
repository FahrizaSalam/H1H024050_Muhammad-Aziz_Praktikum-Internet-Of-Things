#include <WiFi.h>                // library WiFi
#include <PubSubClient.h>        // library MQTT client
#include <ArduinoJson.h>         // library JSON
#include <DHT.h>                 // library sensor suhu DHT

const char* ssid = "Atharva";     // nama WiFi
const char* password = "starlink"; // password WiFi

const char* mqttServer = "broker.hivemq.com"; // alamat broker MQTT
const int mqttPort = 1883;                     // port broker MQTT
const char* topicData = "unsoed/tk245004/kelompokAnda/data"; // topic data suhu
const char* topicPerintah = "unsoed/tk245004/kelompokAnda/perintah"; // topic perintah LED
const char* topicBuzzer = "unsoed/tk245004/kelompokAnda/buzzer"; // topic perintah buzzer

#define DHTPIN 4                 // pin sensor DHT
#define DHTTYPE DHT22            // tipe sensor DHT

const int ledPin = 2;            // pin aktuator LED
const bool LED_ACTIVE_LOW = true;// true untuk LED bawaan; false untuk LED eksternal ke GND
const int buzzerPin = 14;        // pin aktuator Buzzer

DHT dht(DHTPIN, DHTTYPE);        // objek sensor DHT
WiFiClient espClient;            // objek koneksi WiFi
PubSubClient client(espClient);  // objek klien MQTT

unsigned long waktuTerakhirPublish = 0; // penyimpan waktu terakhir (non-blocking)
const long intervalPublish = 5000;      // interval pengiriman 5 detik

void callback(char* topic, byte* payload, unsigned int length) { // fungsi terima pesan
  String pesan; // variabel pesan
  for (unsigned int i = 0; i < length; i++) pesan += (char)payload[i]; // konversi byte ke teks
  
  JsonDocument doc; // objek JSON
  if (deserializeJson(doc, pesan)) return; // abaikan jika parsing error
  
  const char* perintah = doc["perintah"]; // ambil teks perintah
  
  if (String(topic) == topicPerintah) { // filter topic dari LED
    digitalWrite(ledPin, String(perintah) == "ON" ? HIGH : LOW); // eksekusi perintah ke LED
    Serial.print("Perintah LED diterima -> Aktuator LED: "); // cetak rute pesan
    Serial.println(perintah); // isi perintah
  } 
  else if (String(topic) == topicBuzzer) { // filter topic dari Buzzer
    digitalWrite(buzzerPin, String(perintah) == "ON" ? HIGH : LOW); // eksekusi perintah ke Buzzer
    Serial.print("Perintah Buzzer diterima -> Aktuator Buzzer: "); // cetak rute pesan
    Serial.println(perintah); // isi perintah
  }
}

void hubungkanWiFi() { // fungsi koneksi WiFi
  WiFi.begin(ssid, password); // mulai koneksi
  while (WiFi.status() != WL_CONNECTED) delay(500); // tunggu hingga nyambung
  Serial.println("WiFi berhasil terhubung!"); // cetak berhasil
}

void hubungkanMQTT() { // fungsi koneksi MQTT
  while (!client.connected()) { // jika belum terhubung
    String clientId = "ESP32Client-" + String(random(0xffff), HEX); // nama klien acak
    if (client.connect(clientId.c_str())) { // jika sukses sambung
      client.subscribe(topicPerintah); // langganan ke topic perintah
      client.subscribe(topicBuzzer); // langganan ke topic buzzer
      Serial.println("Terhubung dan subscribe topic perintah dan buzzer"); // info berhasil
    } else { // jika gagal
      delay(2000); // jeda sebelum coba lagi
    }
  }
}

void setup() { // konfigurasi awal
  Serial.begin(115200); // mulai serial monitor
  pinMode(ledPin, OUTPUT); // pin LED sebagai output
  pinMode(buzzerPin, OUTPUT); // pin buzzer sebagai output
  dht.begin(); // inisialisasi DHT
  
  hubungkanWiFi(); // mulai sambung WiFi
  client.setServer(mqttServer, mqttPort); // atur MQTT server
  client.setCallback(callback); // atur penerima pesan
}

void loop() { // proses utama
  if (!client.connected()) hubungkanMQTT(); // pastikan MQTT selalu terhubung
  client.loop(); // jalankan sistem monitor pesan
  
  if (millis() - waktuTerakhirPublish > intervalPublish) { // cek jeda pengiriman (non-blocking)
    waktuTerakhirPublish = millis(); // catat waktu terbaru
    
    float suhu = dht.readTemperature(); // baca suhu dari sensor
    if (!isnan(suhu)) { // cek suhu valid
      JsonDocument doc; // buat JSON objek
      doc["suhu"] = suhu; // isi key suhu dengan data
      
      char buffer[128]; // penyangga data
      serializeJson(doc, buffer); // susun JSON
      client.publish(topicData, buffer); // kirim ke server
      
      Serial.print("Data terkirim: "); // info terkirim
      Serial.println(buffer); // isi data
    }
  }
}

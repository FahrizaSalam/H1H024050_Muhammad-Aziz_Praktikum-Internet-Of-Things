import json                     # library untuk parsing data JSON
import time                     # library untuk mendapatkan waktu
import paho.mqtt.client as mqtt # library MQTT client Python
import pandas as pd             # library untuk pengolahan data tabular
import matplotlib.pyplot as plt # library untuk visualisasi grafik

BROKER = "broker.hivemq.com"    # alamat broker MQTT publik
PORT = 1883                     # port broker MQTT
TOPIC = "unsoed/tk245004/kelompokAnda/data" # topic MQTT sesuai topicData ESP32
JUMLAH_SAMPEL = 30              # jumlah data yang akan dikumpulkan

data = []                       # list penyimpan data yang diterima

def on_connect(client, userdata, flags, reason_code, properties): # callback saat terhubung
    print("Terhubung ke broker, kode:", reason_code) # cetak status koneksi
    client.subscribe(TOPIC)     # subscribe ke topic data

def on_message(client, userdata, msg): # callback saat menerima pesan
    try:                        # blok penanganan error
        payload = json.loads(msg.payload.decode()) # parsing payload JSON
        suhu = float(payload["suhu"]) # ambil nilai suhu sebagai float
    except (ValueError, KeyError): # jika data tidak valid
        print("Data tidak valid, diabaikan") # cetak pesan error
        return                  # hentikan proses

    data.append({"waktu": time.strftime("%Y-%m-%d %H:%M:%S"), "suhu": suhu}) # simpan data dan waktu

    print(f"[{len(data)}/{JUMLAH_SAMPEL}] suhu = {suhu}") # cetak progres pengumpulan data

    if len(data) >= JUMLAH_SAMPEL: # jika data sudah cukup
        client.disconnect()     # putuskan koneksi MQTT

# --- Tahap 1: akuisisi data dari MQTT ---
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2) # buat objek MQTT client versi 2
client.on_connect = on_connect  # daftarkan callback koneksi
client.on_message = on_message  # daftarkan callback pesan masuk
client.connect(BROKER, PORT)    # hubungkan ke broker
client.loop_forever()           # jalankan loop hingga disconnect

# --- Tahap 2: simpan ke CSV ---
df = pd.DataFrame(data)         # konversi list ke DataFrame
df.to_csv("data_suhu.csv", index=False) # simpan ke file CSV tanpa indeks
print("Data tersimpan di data_suhu.csv") # cetak konfirmasi penyimpanan

# --- Tahap 3: analisis statistik dan deteksi anomali (z-score) ---
rata2 = df["suhu"].mean()       # hitung rata-rata suhu
std = df["suhu"].std()          # hitung simpangan baku suhu

if std == 0:                    # jika simpangan baku nol (data seragam)
    df["zscore"] = 0.0          # set z-score menjadi 0
else:                           # jika data bervariasi
    df["zscore"] = (df["suhu"] - rata2) / std # hitung z-score tiap data
df["anomali"] = df["zscore"].abs() > 2 # tandai anomali jika |z| > 2

print(f"Rata-rata : {rata2:.2f}") # cetak rata-rata
print(f"Std deviasi: {std:.2f}")  # cetak simpangan baku
print(f"Minimum   : {df['suhu'].min():.2f}") # cetak nilai minimum
print(f"Maksimum  : {df['suhu'].max():.2f}") # cetak nilai maksimum
print(f"Jumlah anomali: {df['anomali'].sum()}") # cetak jumlah anomali

# --- Tahap 4: visualisasi ---
plt.figure(figsize=(10, 4))     # buat figure dengan ukuran 10x4 inci
plt.plot(df.index, df["suhu"], marker="o", label="Suhu") # plot data suhu
anom = df[df["anomali"]]        # ambil data anomali saja
plt.scatter(anom.index, anom["suhu"], color="red", s=100, label="Anomali", zorder=3) # plot titik anomali berwarna merah
plt.axhline(rata2, color="gray", linestyle="--", label="Rata-rata") # garis horizontal rata-rata
plt.xlabel("Urutan data")       # label sumbu x
plt.ylabel("Suhu (°C)")         # label sumbu y
plt.title("Data Suhu dan Deteksi Anomali") # judul grafik
plt.legend()                    # tampilkan legenda
plt.tight_layout()              # atur layout agar rapi
plt.savefig("grafik_suhu.png")  # simpan grafik sebagai gambar PNG
plt.show()                      # tampilkan grafik

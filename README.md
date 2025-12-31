# Self Balancing Robot (PID Control) 🤖⚖️

Proyek ini adalah implementasi robot penyeimbang diri (*Self Balancing Robot*) dua roda yang menggunakan prinsip pendulum terbalik. Robot ini menjaga keseimbangannya secara aktif menggunakan algoritma kontrol **PID (Proportional, Integral, Derivative)**.

## 📝 Deskripsi Proyek
Dibuat sebagai **Proyek Akhir Mata Kuliah Sistem Automasi** di Fakultas Ilmu Komputer, Universitas Brawijaya.
Robot ini membaca kemiringan menggunakan sensor IMU dan menggerakkan motor DC maju/mundur untuk melawan gravitasi agar tetap tegak.

## ⚙️ Prinsip Kerja (Closed-Loop System)
1.  **Sensing:** Sensor MPU6050 (DMP) membaca sudut kemiringan (*Pitch*) robot.
2.  **Comparison:** Sudut aktual dibandingkan dengan *Setpoint* (titik seimbang, misal: 1.16°). Selisihnya disebut *Error*.
3.  **Control (PID):** Algoritma PID menghitung aksi koreksi berdasarkan *Error*:
    * **P:** Merespons besarnya error saat ini.
    * **I:** Mengoreksi error akumulatif (steady-state).
    * **D:** Meredam osilasi dengan merespons laju perubahan error.
4.  **Actuation:** Arduino mengirim sinyal PWM ke Driver L298N untuk menggerakkan motor.

## 🛠️ Hardware Komponen
* **Mikrokontroler:** Arduino Uno R3
* **Sensor:** MPU6050 (Accelerometer + Gyroscope)
* **Driver Motor:** L298N
* **Aktuator:** 2x Motor DC dengan Gearbox + Roda
* **Power:** Baterai AA / Li-Ion 12V
* **Chassis:** Akrilik Custom

## 🧠 Parameter PID (Tuning Result)
Berdasarkan hasil pengujian dan analisis grafik respon sistem:
* **Kp (Proportional):** 30.0
* **Ki (Integral):** 100.0
* **Kd (Derivative):** 1.5
* **Setpoint:** 1.16° (Sudut keseimbangan mekanis)

## 📂 Struktur Kode
Program ditulis menggunakan **Arduino IDE (C++)** dan memanfaatkan library:
* `PID_v1.h` (Algoritma Kontrol)
* `LMotorController.h` (Kontrol Driver Motor)
* `MPU6050_6Axis_MotionApps20.h` (Pembacaan Sensor DMP)

## 👥 Tim Pengembang (Kelompok 5)
1.  Akmal Ahmad Ghozali
2.  Achmad Nadhif Ma'ruf
3.  Dhio Rahmansyah
4.  Muhammad Irsyaddhia Fahlevi
5.  **Muhfi Fawwaz Rizqullah**

## 🔌 Skema Rangkaian (Wiring Diagram)
Berikut adalah detail koneksi kabel antara komponen (Arduino, L298N, MPU6050):

![Wiring Diagram] Wiring.png

---

## 🎥 Video Demo Robot
Lihat bagaimana robot ini menyeimbangkan diri secara otomatis dalam video berikut:

### [▶️ Tonton Video Demo di YouTube] https://youtube.com/shorts/0nAdDa0303k?feature=share

*(Klik link di atas untuk melihat video pengujian)*

#include <PID_v1.h>
#include <LMotorController.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define MIN_ABS_SPEED 30

MPU6050 mpu;

// MPU control/status vars
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

// orientation/motion vars
Quaternion q;
VectorFloat gravity;
float ypr[3];

// --- PID CONTROL CONFIGURATION ---
double originalSetpoint = 1.16; // Titik seimbang fisik robot
double setpoint = originalSetpoint;
double input, output;

// Nilai Konstanta PID (Hasil Tuning)
double Kp = 30;
double Ki = 100;
double Kd = 1.5;

PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

// --- MOTOR CONTROL CONFIGURATION ---
double motorSpeedFactorLeft = 0.5;
double motorSpeedFactorRight = 0.5;

// Pin Mapping L298N
int ENA = 11;
int IN1 = 7;
int IN2 = 6;
int IN3 = 5;
int IN4 = 4;
int ENB = 10;

LMotorController motorController(ENA, IN1, IN2, ENB, IN3, IN4, motorSpeedFactorLeft, motorSpeedFactorRight);

void setup() {
    // Baud rate tinggi untuk monitoring data real-time
    Serial.begin(115200);

    // I2C setup
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock

    // Inisialisasi MPU6050
    mpu.initialize();
    devStatus = mpu.dmpInitialize();

    // Offset Gyro (Sesuaikan dengan kalibrasi hardware Anda)
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    mpu.setZAccelOffset(0);

    if (devStatus == 0) {
        mpu.setDMPEnabled(true);
        packetSize = mpu.dmpGetFIFOPacketSize();
        
        // Setup PID
        pid.SetMode(AUTOMATIC);
        pid.SetSampleTime(10); // Frekuensi sampling 10ms
        pid.SetOutputLimits(-255, 255); // Batas PWM Arduino
    } else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
        while (1); // Stop program jika sensor gagal
    }
}

void loop() {
    // Cek apakah ada cukup data di FIFO
    fifoCount = mpu.getFIFOCount();

    if (fifoCount >= packetSize) {
        // Baca data dari FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize; // Update count agar tidak overflow

        // Dapatkan data orientasi (Quaternion & Gravity)
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        // Ambil nilai Pitch (Kemiringan depan-belakang)
        input = ypr[1] * 180 / M_PI;

        // Hitung Output menggunakan PID
        pid.Compute();

        // --- SERIAL MONITORING (Untuk Plotter) ---
        // Format: Input (Pitch), Setpoint
        Serial.print(input);
        Serial.print(","); 
        Serial.println(setpoint);

        // --- PENGGERAK MOTOR ---
        // Menggerakkan motor berdasarkan hasil kalkulasi PID
        // Tanda minus (-) digunakan untuk negative feedback loop
        motorController.move(-output, MIN_ABS_SPEED);
    }
}

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// --- DEFINE YOUR LED PIN HERE ---
const int ledPin = 7; // LED pin

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10); // wait for serial

  if (!mpu.begin()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  Serial.println("MPU6050 connected!");

  // Configure accelerometer range (optional)
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED off
}

void loop() {
  // 1. Read motion data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // 2. Send formatted data to Python (ax, ay, az)
  Serial.print(a.acceleration.x); Serial.print(',');
  Serial.print(a.acceleration.y); Serial.print(',');
  Serial.println(a.acceleration.z);

  // 3. Listen for commands from Python to control the LED
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'O') {
      digitalWrite(ledPin, HIGH); // Turn LED ON
    } 
    else if (cmd == 'F') {
      digitalWrite(ledPin, LOW);  // Turn LED OFF
    }
  }

  delay(50); // Small delay for stability
}
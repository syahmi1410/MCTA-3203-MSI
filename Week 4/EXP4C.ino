#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;
Servo gateServo;

const int greenLedPin = 5; 
const int servoPin = 6;    
const int redLedPin = 7;   

const unsigned long streamDurationMs = 2000; 
const unsigned long sampleDelayMs = 50;      

void setup() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  gateServo.attach(servoPin);
  gateServo.write(90); 
  
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  
  // Fixed quotes here
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  
  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);
}

void openGate() {
  for (int pos = 90; pos <= 180; pos += 2) {
    gateServo.write(pos);
    delay(15);
  }
  Serial.println("Status: Open");
}

void closeGate() {
  for (int pos = 180; pos >= 90; pos -= 2) {
    gateServo.write(pos);
    delay(15);
  }
  Serial.println("Status: Closed");
}

void wrongCardFlash() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(redLedPin, LOW);
    delay(150);
    digitalWrite(redLedPin, HIGH);
    delay(150);
  }
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    
    if (cmd == '1') {
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
      openGate();
    } 
    else if (cmd == '0') {
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
      closeGate();
    } 
    else if (cmd == 'X') {
      wrongCardFlash();
    } 
    else if (cmd == 'R') {
      unsigned long start = millis();
      while (millis() - start < streamDurationMs) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        // Stream as a single comma-separated line for Python parsing
        Serial.print(ax); Serial.print(",");
        Serial.print(ay); Serial.print(",");
        Serial.print(az); Serial.print(",");
        Serial.print(gx); Serial.print(",");
        Serial.print(gy); Serial.print(",");
        Serial.println(gz); // println adds the newline at the end
        
        delay(sampleDelayMs);
      }
    }
  }
}
  

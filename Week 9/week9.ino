#include "BluetoothSerial.h"
#include <ESP32Servo.h> 

BluetoothSerial SerialBT;
Servo myServo;

// Pin Definitions
const int potpin = 34;    // Potentiometer input
const int servoPin = 13;  // Signal pin for the Servo
const int LED = 14;        // Built-in LED for feedback

char command;
int potValue = 0;

void setup() {
  Serial.begin(9600);
  
  // Start Bluetooth with your preferred display name
  SerialBT.begin("Group 2"); 
  
  // Setup hardware
  myServo.attach(servoPin);
  pinMode(LED, OUTPUT);
  
  Serial.println("System Initialized. Connect your smartphone via Bluetooth.");
}

void loop() {
  // 1. Read Potentiometer (Sensor) data
  potValue = analogRead(potpin);
  
  // 2. Transmit the value to the smartphone terminal
  SerialBT.print("Sensor (Pot) Value: ");
  SerialBT.println(potValue);

  // 3. Check for incoming Bluetooth commands
  if (SerialBT.available()) {
    command = SerialBT.read();
    
    // Process commands to move the Servo
    switch (command) {
      case '1': // Move to start position
        myServo.write(0);
        digitalWrite(LED, HIGH);
        SerialBT.println("Action: Servo set to 0°");
        break;
      case '2': // Move to middle position
        myServo.write(90);
        SerialBT.println("Action: Servo set to 90°");
        break;
      case '3': // Move to end position
        myServo.write(180);
        digitalWrite(LED, LOW);
        SerialBT.println("Action: Servo set to 180°");
        break;
    }
  }
  
  // Delay to prevent flooding the Bluetooth terminal
  delay(1000); 
}
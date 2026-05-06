#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX

// Define the LED pin
const int ledPin = 13; 

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);

  // Wait for HuskyLens to connect
  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed! Check wiring and Protocol (Serial 9600)"));
    delay(500);
  }
  Serial.println("HuskyLens Ready!");
}

void loop() {
  if (!huskylens.request()) {
    // If request fails or nothing is seen, ensure LED is OFF
    digitalWrite(ledPin, LOW);
    return;
  }

  // Check if any learned object (like your water bottle) is in view
  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();
    
    // Check if the detected object is "Object 1"
    if (result.command == COMMAND_RETURN_BLOCK && result.ID == 1) {
      Serial.println("Water Bottle (Object 1) detected!");
      digitalWrite(ledPin, HIGH); // Turn LED ON
    } 
    else {
      // If a different object is seen, turn LED OFF
      digitalWrite(ledPin, LOW);
    }
  } 
  else {
    // If the camera sees nothing at all, turn LED OFF
    digitalWrite(ledPin, LOW);
  }
}
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

HUSKYLENS huskylens;
SoftwareSerial mySerial(4, 5); // RX, TX

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  clearLED();

  while (!huskylens.begin(mySerial)) {
    Serial.println(F("Begin failed!"));
    delay(500);
  }
  Serial.println("HuskyLens Ready for 5 Colors!");
}

void loop() {
  if (!huskylens.request()) {
    clearLED();
    return;
  }

  if (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();

    if (result.command == COMMAND_RETURN_BLOCK) {
      switch (result.ID) {
        case 1: // RED
          setColor(255, 0, 0);
          break;
        case 2: // BLUE
          setColor(0, 0, 255);
          break;
        case 3: // GREEN
          setColor(0, 255, 0);
          break;
        case 4: // YELLOW (Red + Green)
          setColor(255, 255, 0);
          Serial.println("Detected: Yellow");
          break;
        case 5: // PURPLE (Red + Blue)
          setColor(255, 0, 255);
          Serial.println("Detected: Purple");
          break;
        default:
          clearLED();
          break;
      }
    }
  } else {
    clearLED();
  }
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void clearLED() {
  setColor(0, 0, 0);
}
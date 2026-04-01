const int ledPin = 7; // LED connected to D7

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED OFF
}

void loop() {
  // Listen for commands from Python
  if (Serial.available() > 0) {
    char cmd = Serial.read(); // Read one byte

    if (cmd == '1') {         // Turn ON
      digitalWrite(ledPin, HIGH);
    } 
    else if (cmd == '0') {    // Turn OFF
      digitalWrite(ledPin, LOW);
    }
  }
}
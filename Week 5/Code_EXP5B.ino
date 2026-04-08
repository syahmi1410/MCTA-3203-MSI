const int relay1 = 7;
const int relay2 = 8;
const int ledPin = 13;
const int ldrPin = A0;

int ldrValue = 0;

int darkThreshold = 80;
int lightThreshold = 40;

bool isDark = false;

unsigned long previousMillis = 0;
const long interval = 2000;

bool actuatorState = false; // false = extend, true = retract

void setup()
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  ldrValue = analogRead(ldrPin);
  Serial.println(ldrValue);

  // Detect DARK
  if (ldrValue > darkThreshold)
  {
    isDark = true;
  }
  else if (ldrValue < lightThreshold)
  {
    isDark = false;
  }

  if (isDark)
  {
    stopActuator();
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;

      actuatorState = !actuatorState;

      if (actuatorState)
        extendActuator();
      else
        retractActuator();
    }
  }
}

void extendActuator()
{
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
}

void retractActuator()
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
}

void stopActuator()
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}
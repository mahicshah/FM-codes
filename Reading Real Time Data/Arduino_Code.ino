int analogPin = A0;
int analogValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {analogValue = analogRead(analogPin);
  analogValue = map(analogValue, 0, 1023, 0, 255);
  Serial.write(analogValue);
  delay(50);
}

#include <SoftwareSerial.h>

int analogPin = A0;
int analogValue = 0;

SoftwareSerial mySerial(0, 1);

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  analogValue = analogRead(analogPin);  //Read analog input
  analogValue = map(analogValue, 0, 1023, 0, 255);  
  Serial.write(analogValue);
  delay(100);
}

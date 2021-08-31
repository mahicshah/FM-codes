int timedelay;
int count;

void setup() {
  Serial.begin(9600);
  timedelay = 1000;
  count = 0;
}

void loop() {
  String s = "counter : " + String (count);
  Serial.println(s);
  count++;
  delay(timedelay);
}

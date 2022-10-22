#include <Arduino.h>
unsigned long time = 0;

void setup() {
  // put your setup code here, to run once:
  digitalWrite(11, HIGH);
  pinMode(11, INPUT_PULLUP);

  // serial begin
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int testdata = 5689;
  if (digitalRead(11) == LOW && millis() - time > 1000) {
    time = millis();
    Serial.println("Time("+String(int(millis()/1000)) + ")=serialsend->" + String(testdata));
  }
}
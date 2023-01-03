#include <Arduino.h>

unsigned long time = 0;
int lastdata = 0;

void setup() {
  // put your setup code here, to run once:
  digitalWrite(11, HIGH);
  pinMode(11, INPUT_PULLUP);
  digitalWrite(12, HIGH);
  pinMode(12, INPUT_PULLUP);
  // serial begin
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //"AC ON" : "5678" : 0,
  //"AC OFF" : "1234" : 1,
  int key2operation[2] = {5678, 1234};

  ////PIN 11 LOW,AC ON
  if (digitalRead(11) == LOW && millis() - time > 1000) {
    time = millis();
    // AC ON
    if (lastdata == 0) {
      Serial.println(key2operation[0]);
      lastdata = 1;
    }
  }
  
  
  ////PIN 12 LOW,AC OFF
  if (digitalRead(12) == LOW && millis() - time > 1000) {
    time = millis();
    // AC OFF
    if (lastdata == 0) {
      Serial.println(key2operation[1]);
      lastdata = 1;
    }
  }
  if(digitalRead(12) == HIGH && digitalRead(11) == HIGH){
    lastdata = 0;
  }

  
}

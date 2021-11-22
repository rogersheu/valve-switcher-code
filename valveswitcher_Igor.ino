#include<LiquidCrystal.h>
LiquidCrystal lcd(10,11,12,22,23,24,25,26,27,28,29);

int incomingByte = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 30; i < 49; i += 1) {
    pinMode(i, INPUT);
  }


  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);

  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(20, HIGH);
  digitalWrite(21, HIGH);
}


void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 49) {
      changeState(1);
    }
    else if (incomingByte == 50) {
      changeState(2);
    }
    else if (incomingByte == 51) {
      changeState(3);
    }
    else if (incomingByte == 52) {
      changeState(4);
    }
    else {
    }
  }
}


void changeState(int number) {
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(20, HIGH);
  digitalWrite(21, HIGH);
  int pinNum = number + 17; //num from 1 to 4
  digitalWrite(pinNum, LOW);
  //delay(6000);

}

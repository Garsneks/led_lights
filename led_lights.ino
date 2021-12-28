#include <avr/sleep.h>

const int modeButton = 2;
const int led1 = 9;
const int led2 = 10;
const int led3 = 11;
const int led4 = 12;

bool goToSleep = false;
int buttonPressTime = 0;
int timeNow = 0;

void wakeUp()
{
  detachInterrupt(digitalPinToInterrupt(modeButton));
  sleep_disable();
  ledsOn(true);
}

void ledsOn(bool state){
  if (state){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  }
  else{
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  }
}

void setup() {
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  Serial.begin(9600);
  ledsOn(true);
}

void loop() {
  Serial.println("do something");
  if (digitalRead(modeButton) == LOW){
    buttonPressTime = millis();
    while (digitalRead(modeButton) == LOW){
      timeNow = millis();
      if ((buttonPressTime + 1000) <= timeNow){
        digitalWrite(led1, LOW);
      }
      if ((buttonPressTime + 2000) <= timeNow){
        digitalWrite(led2, LOW);
      }
      if ((buttonPressTime + 3000) <= timeNow){
        goToSleep = true;
        digitalWrite(led3, LOW);
      }
    }
    if (goToSleep != true){
      ledsOn(true);
    }
  }
  if (digitalRead(modeButton) == HIGH && goToSleep){
    goToSleep = false;
    sleep_enable();
    attachInterrupt(digitalPinToInterrupt(modeButton), wakeUp, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    ledsOn(false);
    delay(1000);
    sleep_cpu();
  }
}

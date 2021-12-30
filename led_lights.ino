#include <avr/sleep.h>

const int modeButton = 2;
const int led1 = 10;
const int led2 = 9;
const int led3 = 6;
const int led4 = 5;

bool goToSleep = false;
int buttonPressTime = 0;
int timeNow = 0;

int selectedMode = 0;
const int finalMode = 2;
int currentLed = 0;
int ledArray[] = {255, 255, 255, 255};
const int ledCount = sizeof(ledArray)/sizeof(int);

void wakeUp()
{
  detachInterrupt(digitalPinToInterrupt(modeButton));
  sleep_disable();
}

void ledsOn(bool state){
  if (state){
    analogWrite(led1, 255);
    delay(100);
    analogWrite(led2, 255);
    delay(100);
    analogWrite(led3, 255);
    delay(100);
    analogWrite(led4, 255);
  }
  else{
    analogWrite(led1, 0);
    analogWrite(led2, 0);
    analogWrite(led3, 0);
    analogWrite(led4, 0);
  }
}

void setup() {
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  ledsOn(true);
  Serial.begin(9600);
}

void runLedsWithDelay(int delayTime){
  analogWrite(led1, ledArray[0]);
    delay(delayTime);
    analogWrite(led2, ledArray[1]);
    delay(delayTime);
    analogWrite(led3, ledArray[2]);
    delay(delayTime);
    analogWrite(led4, ledArray[3]);
    delay(delayTime);
}

void loop() {
  if (digitalRead(modeButton) == LOW){
    buttonPressTime = millis();
    while (digitalRead(modeButton) == LOW){
      timeNow = millis();
      if ((buttonPressTime + 1500) <= timeNow){
        goToSleep = true;
        analogWrite(led3, 0);
      }
      else if ((buttonPressTime + 1000) <= timeNow){
        analogWrite(led2, 0);
      }
      else if ((buttonPressTime + 500) <= timeNow){
        analogWrite(led1, 0);
      }
    }
    if (goToSleep != true){
      ledsOn(true);
      selectedMode = selectedMode + 1;
      if (selectedMode > finalMode){
        selectedMode = 0;
      }
    }
    
  }
  if (selectedMode == 1){
    currentLed = random(ledCount);
    runLedsWithDelay(200);
    if (ledArray[currentLed] == 255){
      ledArray[currentLed] = 100;
    }
    else{
      ledArray[currentLed] = 255;
    }
  }
  if (selectedMode == 2){
    ledsOn(true);
    if (currentLed == ledCount + 1){
      currentLed = 0;
    }
    ledArray[currentLed] = 100;
    runLedsWithDelay(200);
    currentLed = currentLed + 1;
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

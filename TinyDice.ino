#include <avr/sleep.h>
#include <avr/power.h>

int middleLedsPin = 4;
int bottomTopLedsPin = 1;
int topBottomLedsPin = 3;
int centerLedPin = 0;
int buttonPin = 2;

int buttonState;

long ran;

int time = 2000;

unsigned long lastmillis;

int i = 0;

typedef void (* NumFuncPTR) ();

void One(){
  digitalWrite (centerLedPin, HIGH);
};

void Two(){
  digitalWrite (middleLedsPin, HIGH);
};

void Three(){
  digitalWrite (topBottomLedsPin, HIGH);
  digitalWrite (centerLedPin, HIGH);
};

void Four(){
  digitalWrite (bottomTopLedsPin, HIGH);
  digitalWrite (topBottomLedsPin, HIGH);
};

void Five(){
  digitalWrite (bottomTopLedsPin, HIGH);
  digitalWrite (topBottomLedsPin, HIGH);
  digitalWrite (centerLedPin, HIGH);
};

void Six(){
digitalWrite (middleLedsPin, HIGH);
digitalWrite (bottomTopLedsPin, HIGH);
digitalWrite (topBottomLedsPin, HIGH);
};

NumFuncPTR number[6] = {One,Two,Three,Four,Five,Six};

void pin2Interrupt(void){
  /* This will bring us back from sleep. */
  /* We detach the interrupt to stop it from
  * continuously firing while the interrupt pin
  * is low.
  */
  detachInterrupt(0);
}

void enterSleep(void){
  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(0, pin2Interrupt, LOW);
  delay(100);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  /* The program will continue from here. */
  /* First thing to do is disable sleep. */
  sleep_disable();
}

void Blink(){ //for debugging
  digitalWrite(centerLedPin,HIGH);
  delay(500);
  digitalWrite(centerLedPin,LOW);
  delay(500);
}

void setup (){
  //Disable stuff we dont need so we can entire extreme low power sleep(.1uA)
  // Disable the ADC by setting the ADEN bit (bit 7) to zero.
  ADCSRA = ADCSRA & B01111111;
  // Disable the analog comparator by setting the ACD bit (bit 7) to one.
  ACSR = B10000000;
  randomSeed(analogRead(1));
  pinMode (middleLedsPin, OUTPUT);
  pinMode (bottomTopLedsPin, OUTPUT);
  pinMode (topBottomLedsPin, OUTPUT);
  pinMode (centerLedPin, OUTPUT);
  pinMode (buttonPin, INPUT_PULLUP);
  lastmillis = millis();
}

void loop(){
  
  buttonState = digitalRead(buttonPin);
  
  while (buttonState == LOW){
    buttonState = digitalRead(buttonPin);
    time = 2000;
    lastmillis = millis();
    reset();
    number[random(0,6)]();
    delay(30);
  }

  if (lastmillis + time <= millis()){
    reset();
    enterSleep();
  }
}

void reset(){
  digitalWrite (middleLedsPin, LOW);
  digitalWrite (bottomTopLedsPin, LOW);
  digitalWrite (topBottomLedsPin, LOW);
  digitalWrite (centerLedPin, LOW);
}

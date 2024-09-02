// SETUP: 3 buttons

#include <Wire.h>

//------------------WIRING------------------
int startStopButtonPin = 10; // detects button pressing from the start/stop button.
int backButtonPin = 11;      // detects button pressing from the back button.
int nextButtonPin = 12;      // detects button pressing from the next button.
int buttonPressedLedPin = 9; // pin of the button pressing witness ; the number of the LED positive pin (the longer one)

void setup()
{
  Serial.begin(9600);
  pinMode(startStopButtonPin, INPUT);   // set push button pin into input mode
  pinMode(backButtonPin, INPUT);        // set push button pin into input mode
  pinMode(nextButtonPin, INPUT);        // set push button pin into input mode
  pinMode(buttonPressedLedPin, OUTPUT); // set LED pin into output mode
}

void loop()
{
  // SERIAL PRINT the value of startStopButtonPin, backButtonPin and nextButtonPin
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime >= 500)
  {
    Serial.print("startStopButtonPin: ");
    Serial.print(digitalRead(startStopButtonPin));
    Serial.print(" backButtonPin: ");
    Serial.print(digitalRead(backButtonPin));
    Serial.print(" nextButtonPin: ");
    Serial.println(digitalRead(nextButtonPin));
    lastPrintTime = millis();
  }
}
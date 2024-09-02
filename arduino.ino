// SETUP: 3 buttons and 1 LED
// 1 LCD screen

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//------------------WIRING------------------
int anyButtonPin = 10; // detects button pressing from any of the 3 buttons.
int pressLedPin = 9;   // pin of the button pressing witness ; the number of the LED positive pin (the longer one)

void setup()
{
  // initialize the LCD
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("LEON PAPA MAMAN!");

  pinMode(anyButtonPin, INPUT); // set push button pin into input mode
  pinMode(pressLedPin, OUTPUT); // set LED pin into output mode
}

void loop()
{
  if (digitalRead(anyButtonPin) == HIGH) // if the button is not pressed
    digitalWrite(pressLedPin, LOW);      // switch off LED
  else                                   // if the button is pressed
    digitalWrite(pressLedPin, HIGH);     // switch on LED
}
// SETUP: 3 buttons

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//------------------WIRING------------------
int startStopButtonPin = 10; // detects button pressing from the start/stop button.
int backButtonPin = 11;      // detects button pressing from the back button.
int nextButtonPin = 12;      // detects button pressing from the next button.
int tempoLedPin = 9;         // pin of the tempo LED ; the number of the LED positive pin (the longer one)

// Song setlist as an array of dictionaries
// Each dictionary contains the song position in the setlist, its name and its tempo
// 1, Sirocco, 111
// 2, Beehive, 116
// 3, Tempete, 126
// Funkyla	104
// Tempête	126
// Memento	110
// Sonido Picante	121
// Back in the Game	117
// Imago	127
// Zoukazeph	97
// Orange Pressée	110
// Blackan	190
// Ethiodream	94
// Karaba	125
// Ouverture	110
// Soul Remedy	100
// De boa	98
// Apuca	130
// Beehive	116
// Moonwhale	106
// Sirocco	111

struct Song
{
  String name;
  int tempo;
};

Song setlist[] = {
    {"Sirocco", 111},
    {"Beehive", 116},
    {"Tempete", 126},
    {"Moonwhale", 106},
    {"Ethiodream", 94},
    {"Imago", 127},
    {"Back in the Game", 117},
    {"Karaba", 125},
    {"Soul Remedy", 100},
    {"Apuca", 130},
    {"Funkyla", 104},
    {"De boa", 98},
    {"Memento", 110}};
// {"Sonido Picante", 121},
// {"Zoukazeph", 97},
// {"Orange Pressée", 110},
// {"Blackan", 190},
// {"Ouverture", 110},
// {"Beehive", 116},
// {"Moonwhale", 106},
// {"Sirocco", 111}};

// initialize song position, so that it is available when the loop starts
int songIndex = -1;
bool playing = false;
unsigned long tempoLedBlinkInterval = 0;
unsigned long lastTempoLedBlinkTime = 0;

void setup()
{
  // initialize the LCD
  lcd.init();

  // Turn on the backlight and print a message.
  lcd.backlight();
  lcd.print("*** ZEPHLIST ***");
  Serial.begin(9600);
  pinMode(startStopButtonPin, INPUT); // set push button pin into input mode
  pinMode(backButtonPin, INPUT);      // set push button pin into input mode
  pinMode(nextButtonPin, INPUT);      // set push button pin into input mode
  pinMode(tempoLedPin, OUTPUT);       // set LED pin into output mode
}

void loop()
{
  digitalWrite(tempoLedPin, LOW); // set LED pin to LOW

  // if next button is pressed
  if (digitalRead(nextButtonPin) == HIGH)
  {
    // increment song position
    songIndex++;
    // if song position is greater than the number of songs in the setlist
    if (songIndex >= sizeof(setlist) / sizeof(setlist[0]))
    {
      // reset song position to -1
      songIndex = -1;
    }
    // wait for the button to be released
    while (digitalRead(nextButtonPin) == HIGH)
    {
    }
    displayData(songIndex);
  }

  // if back button is pressed
  if (digitalRead(backButtonPin) == HIGH)
  {
    // decrement song position
    songIndex--;
    // if song position is less than -1
    if (songIndex < -1)
    {
      // set song position to the last song in the setlist
      songIndex = sizeof(setlist) / sizeof(setlist[0]) - 1;
    }
    // wait for the button to be released
    while (digitalRead(backButtonPin) == HIGH)
    {
    }
    displayData(songIndex);
  }

  if (songIndex >= 0 && digitalRead(startStopButtonPin) == HIGH)
  {
    while (digitalRead(startStopButtonPin) == HIGH)
    {
      // wait for the button to be released
    }
    Song song = getSong(songIndex);
    // if the song is not playing
    if (!playing)
    {
      // set playing to true
      playing = true;
      // Make the tempo LED blink at the song tempo
      tempoLedBlinkInterval = 60000 / song.tempo / 2;
      lastTempoLedBlinkTime = millis(); // initialize with the current time
      // print the song name
      Serial.print("Playing ");
      Serial.println(song.name);
    }
    else
    {
      // set playing to false
      playing = false;
      // print the song name
      Serial.print("Stopped ");
      Serial.println(song.name);
      // set the tempo LED to LOW
      stopTempo();
      // go back to the beginning of the loop
    }
    while (digitalRead(startStopButtonPin) == LOW && playing)
    {
      // if the song is playing, make the tempo LED blink
      playTempo(tempoLedBlinkInterval, lastTempoLedBlinkTime);
    }
    // go back to the beginning of the loop
  }

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

Song getSong(int songIndex)
{
  // get the song from the setlist
  return setlist[songIndex];
}

void displayData(int songIndex)
{
  if (songIndex == -1)
  {
    lcd.clear();
    lcd.print("*** ZEPHLIST ***");
  }
  else
  {
    printSong(songIndex);
  }
}

void printSong(int songIndex)
{
  Song song = getSong(songIndex);
  int songPosition = songIndex + 1;
  // On a single line, print the song position and name
  // On a second line, print the song tempo
  String songData = "" + String(songPosition) + ". " + song.name;
  String tempoData = String(song.tempo) + " BPM";

  Serial.println(songData + " " + tempoData);
  lcd.clear();
  lcd.print(songData);
  lcd.setCursor(0, 1);
  lcd.print(tempoData);
}

void playTempo(unsigned long blinkInterval, unsigned long &lastBlinkTime)
{
  if (millis() - lastBlinkTime >= blinkInterval)
  {
    lastBlinkTime = millis();
    digitalWrite(tempoLedPin, !digitalRead(tempoLedPin));
  }
}

void stopTempo()
{
  digitalWrite(tempoLedPin, LOW);
}
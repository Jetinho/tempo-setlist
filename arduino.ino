#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//------------------WIRING------------------
const int startStopButtonPin = 10; // detects button pressing from the start/stop button.
const int backButtonPin = 11;      // detects button pressing from the back button.
const int nextButtonPin = 12;      // detects button pressing from the next button.
const int tempoLedPin = 9;         // pin of the tempo LED ; the number of the LED positive pin (the longer one)

// Song setlist as an array of dictionaries
// Each dictionary contains the song position in the setlist, its name and its tempo
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
// Will be used for other setlists
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

void initializeLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.print("*** ZEPHLIST ***");
}

void initializePins()
{
  pinMode(startStopButtonPin, INPUT);
  pinMode(backButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);
  pinMode(tempoLedPin, OUTPUT);
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

Song getSong(int songIndex)
{
  return setlist[songIndex];
}

void printSong(int songIndex)
{
  Song song = getSong(songIndex);
  int songPosition = songIndex + 1;
  // On a single line, print the song position and name
  // On a second line, print the song tempo
  String songData = String(songPosition) + ". " + song.name;
  String tempoData = String(song.tempo) + " BPM";

  Serial.println(songData + " " + tempoData);
  lcd.clear();
  lcd.print(songData);
  lcd.setCursor(0, 1);
  lcd.print(tempoData);
}

void handleNextButtonPress()
{
  if (digitalRead(nextButtonPin) == HIGH)
  {
    songIndex++;
    if (songIndex >= sizeof(setlist) / sizeof(setlist[0]))
    {
      songIndex = -1;
    }
    while (digitalRead(nextButtonPin) == HIGH)
    {
      // wait for the button to be released
    }
    Serial.println("Pressed next button ");
    displayData(songIndex);
    if (playing && songIndex >= 0)
    {
      updateTempo();
    }
  }
}

void handleBackButtonPress()
{
  if (digitalRead(backButtonPin) == HIGH)
  {
    songIndex--;
    if (songIndex < -1)
    {
      songIndex = sizeof(setlist) / sizeof(setlist[0]) - 1;
    }
    while (digitalRead(backButtonPin) == HIGH)
    {
      // wait for the button to be released
    }
    Serial.println("Pressed back button ");
    displayData(songIndex);
    if (playing && songIndex >= 0)
    {
      updateTempo();
    }
  }
}

void handleStartStopButtonPress()
{
  if (digitalRead(startStopButtonPin) == HIGH)
  {
    while (digitalRead(startStopButtonPin) == HIGH)
    {
      // wait for the button to be released
    }
    Serial.println("Pressed start/stop button ");
    if (songIndex == -1)
    {
      return;
    }
    if (!playing)
    {
      playing = true;
      updateTempo();
      Serial.print("Playing ");
      Serial.println(getSong(songIndex).name);
    }
    else
    {
      playing = false;
      Serial.print("Stopped ");
      Serial.println(getSong(songIndex).name);
      digitalWrite(tempoLedPin, LOW);
    }
  }
}

void updateTempo()
{
  Song song = getSong(songIndex);
  tempoLedBlinkInterval = 60000 / song.tempo / 2;
  lastTempoLedBlinkTime = millis();
}

void blinkTempoLed()
{
  if (playing && millis() - lastTempoLedBlinkTime >= tempoLedBlinkInterval)
  {
    lastTempoLedBlinkTime = millis();
    digitalWrite(tempoLedPin, !digitalRead(tempoLedPin));
  }
}

void setup()
{
  Serial.begin(9600);
  initializeLCD();
  initializePins();
}

void loop()
{
  handleNextButtonPress();
  handleBackButtonPress();
  handleStartStopButtonPress();
  blinkTempoLed();
}
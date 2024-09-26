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
// Each dictionary contains the song position in the setlist, its title and its tempo
struct Song
{
  String title;
  int tempo;
  int subDivisions;
};

Song setlist[] = {
    {"Sirocco", 111, 4},
    {"Beehive", 116, 4},
    {"Tempete", 126, 4},
    {"Moonwhale", 106, 4},
    {"Ethiodream", 94, 4},
    {"Imago", 127, 3},
    {"Back in the Game", 117, 4},
    {"Karaba", 125, 4},
    {"Soul Remedy", 100, 4},
    {"Apuca", 130, 4},
    {"Funkyla", 104, 4},
    {"De boa", 98, 4},
    {"Memento", 110, 4},
    {"Sonido Picante", 121, 4},
    {"Zoukazeph", 97, 4},
    {"Orange Pressée", 110, 4},
    {"Blackan", 190, 5},
    {"Ouverture", 110, 4}};

// initialize song position, so that it is available when the loop starts
int songIndex = -1;
bool playing = false;
unsigned long ledOnInterval = 0;
unsigned long ledOffInterval = 0;
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
  // On a single line, print the song position and title
  // On a second line, print the song tempo
  String songData = String(songPosition) + ". " + song.title;
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
      Serial.println(getSong(songIndex).title);
    }
    else
    {
      playing = false;
      Serial.print("Stopped ");
      Serial.println(getSong(songIndex).title);
      digitalWrite(tempoLedPin, LOW);
    }
  }
}

void updateTempo()
{
  Song song = getSong(songIndex);
  unsigned long totalInterval = 60000 / song.tempo;  // Total interval for one beat in milliseconds
  ledOnInterval = totalInterval / song.subDivisions; // Interval for LED to be on
  ledOffInterval = totalInterval - ledOnInterval;    // Interval for LED to be off

  lastTempoLedBlinkTime = millis();
}

void blinkTempoLed()
{
  static bool isLedOn = false;

  if (playing)
  {
    unsigned long currentTime = millis();
    if (isLedOn)
    {
      // LED is currently on
      if (currentTime - lastTempoLedBlinkTime >= ledOnInterval)
      {
        isLedOn = false;
        lastTempoLedBlinkTime = currentTime;
        digitalWrite(tempoLedPin, LOW);
      }
    }
    else
    {
      // LED is currently off
      if (currentTime - lastTempoLedBlinkTime >= ledOffInterval)
      {
        isLedOn = true;
        lastTempoLedBlinkTime = currentTime;
        digitalWrite(tempoLedPin, HIGH);
      }
    }
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
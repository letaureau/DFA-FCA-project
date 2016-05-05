#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_MUSIC_PLAYER_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_NFC_SHIELD

/* Include 1Sheeld library. */
#include "rgb_lcd.h"
#include <Wire.h>

#include <OneSheeld.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      8

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

rgb_lcd lcd;
const int vib = 3;
const int light = 2;
int flag = 0;
/* Voice commands set by the user. */
const char personality1 = "parth";
const char personality2 = "namita";
const char personality3 = "izabela";
const char personality;
const char musicCommand1[] = "music";
const char musicCommand2[] = "songs";
const char lightCommand1[] = "lights";
const char lightCommand2[] = "light";
const char vibrationCommand1[] = "vibration";
const char vibrationCommand2[] = "vibrations";
const char welcomeCommand[] = "hello dexter";
const char stopCommand[] = "stop";


void setup()
{

  pinMode(vib, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Welcome!!!");
  lcd.setRGB(255, 0, 0);
  delay(1000);
  lcd.setRGB(0, 0, 255);
  delay(1000);
  lcd.setRGB(0, 255, 0);
  delay(1000);
  /* Start Communication. */
  OneSheeld.begin();
  /* Error Commands handiling. */
  VoiceRecognition.setOnError(error);
  /* Subscribe to the new tag scanned event. */
  NFC.setOnNewTagScanned(&tagFunction);
  /* Subscribe to on Tag error event. */
  NFC.setOnError(&tagErrorFunction);
  pixels.begin(); // This initializes the NeoPixel library.
  VoiceRecognition.start();
  delay(3000);
}

void loop ()
{
  /* Check if new command received. */
  if (VoiceRecognition.isNewCommandReceived())
  {
    lcd.clear();
    lcd.print(VoiceRecognition.getLastCommand());
    delay(2000);
    /* Compare the music command. */
    if (!strcmp(musicCommand1, VoiceRecognition.getLastCommand()) || !strcmp(musicCommand2, VoiceRecognition.getLastCommand()))
    {
      stopping();
      lcd.clear();
      lcd.print("MUSIC ON");
      TextToSpeech.say("MUSIC ON");
      delay(2000);
      MusicPlayer.play();
    }
    /* Compare the lights command. */
    else if (!strcmp(lightCommand1, VoiceRecognition.getLastCommand()) || !strcmp(lightCommand2, VoiceRecognition.getLastCommand()))
    {
      stopping();
      lcd.clear();
      lcd.print("LIGHTS ON");
      TextToSpeech.say("LIGHTS ON");
      delay(2000);
      /* LIGHT. */
      for (int i = 0; i < NUMPIXELS; i++)
      {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }
    }
    else if (!strcmp(vibrationCommand1, VoiceRecognition.getLastCommand()) || !strcmp(vibrationCommand2, VoiceRecognition.getLastCommand()))
    {
      stopping();
      lcd.clear();
      lcd.print("VIBRATION ON");
      TextToSpeech.say("VIBRATION ON");
      delay(2000);
      /* VIBRATION. */
      for (int i = 0 ; i < 3; i++)
      {
        digitalWrite(vib, HIGH);
        delay(2000);
        digitalWrite(vib, LOW);
        delay(2000);
      }
    }
    /* Compare the stop command. */
    else if (!strcmp(stopCommand, VoiceRecognition.getLastCommand()))
    {
      TextToSpeech.say("STOP");
      delay(2000);
      stopping();
    }
    /* Compare the next command. */
    else if (!strcmp(welcomeCommand, VoiceRecognition.getLastCommand()))
    {
      welcome();
    }
  }
}

/* Error checking function. */
void error(byte errorData)
{
  /* Switch on error and print it on the terminal. */
  switch (errorData)
  {
    case NETWORK_TIMEOUT_ERROR: Terminal.println("Network timeout"); break;
    case NETWORK_ERROR: Terminal.println("Network Error"); break;
    case AUDIO_ERROR: Terminal.println("Audio error"); break;
    case SERVER_ERROR: Terminal.println("No Server"); break;
    case SPEECH_TIMEOUT_ERROR: Terminal.println("Speech timeout"); break;
    case NO_MATCH_ERROR: Terminal.println("No match"); break;
    case RECOGNIZER_BUSY_ERROR: Terminal.println("Busy"); break;
  }
}

/* OnError event. */
void tagErrorFunction(byte errorNumber)
{
  /* Switch on error number. */
  switch(errorNumber)
  {
    case INDEX_OUT_OF_BOUNDS: Terminal.println("INDEX_OUT_OF_BOUNDS");break;
    case RECORD_CAN_NOT_BE_PARSED: Terminal.println("RECORD_CAN_NOT_BE_PARSED");break;
    case TAG_NOT_SUPPORTED: Terminal.println("TAG_NOT_SUPPORTED");break;
    case NO_ENOUGH_BYTES: Terminal.println("NO_ENOUGH_BYTES");break;
    case TAG_READING_ERROR: Terminal.println("TAG_READING_ERROR");break;
    case RECORD_NOT_FOUND: Terminal.println("RECORD_NOT_FOUND");break;
  }
}


void welcome()
{
  stopping();
  lcd.clear();
  lcd.print("TIME SLEPT:");
  lcd.setCursor(2, 1);
  lcd.print("5:35:16");
  delay(1000);
  TextToSpeech.say("I AM DEXTER YOUR PERSONAL SAFETY ASSISTANT");
  delay(3000);
  TextToSpeech.say("Good morning");
  TextToSpeech.say(personality);
  TextToSpeech.say("I see that you have not slept well the day before, which of the settings would you like me to enable");
  delay(5000);
  TextToSpeech.say("Your options are : Vibration");
  delay(2000);
  digitalWrite(vib, HIGH);
  delay(2000);
  digitalWrite(vib, LOW);
  delay(2000);
  TextToSpeech.say("Lights");
  rainbow(2000);
  TextToSpeech.say("Music");
  MusicPlayer.play();
  delay(2000);
  MusicPlayer.stop();
  VoiceRecognition.start();
  //delay(2000);
}

void responseFunction(byte recordNumber , char data[])
{
  /* Check response and compare data. */
  if(!strcmp(data,personality1))
  {
    Terminal.println("Parth");
    personality = "Parth";
  }
  else if(!strcmp(data,personality2))
  {
    Terminal.println("Namita");
    personality = "Namita";
  }
  else if(!strcmp(data,personality3))
  {
    Terminal.println("Izabela");
    personality = "Izabela";
  }
}

void rainbow(int wait) {
  for (int i = 0; i < NUMPIXELS; i++)
  {
    int r = random(0, 255);
    int g = random(0, 255);
    int b = random(0, 255);
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  delay(wait);
  for (int i = 0; i < NUMPIXELS; i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}

void stopping()
{
  /* Stop the track. */
  MusicPlayer.stop();
  digitalWrite(vib, LOW);
  for (int i = 0; i < NUMPIXELS; i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}

/* A function to be called once a new tag is scanned. */
void tagFunction(NFCTag &myTag)
{
  /* Create and object of type NFCRecord. */
  NFCRecord &myRecordNumber0 = myTag.getRecord(0);
  /* Subscribe to record parsed data response event. */
  myTag.setOnRecordParsedDataResponse(&responseFunction);
  /* 
  /* Check if there's a record with number 0 in Tag. */
  if(!myRecordNumber0.isNull())
  {
    /* Parse and query the data in the first record. */
    myRecordNumber0.queryParsedData();  
  }
  else
  {
    /* Print out no record found. */
    Terminal.println("No Record found"); 
  }
}


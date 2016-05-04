/*

Voice Recognition Shield Example

This example shows an application on 1Sheeld's voice recognition shield.

By using this example, you can play, delay and stop your smartphone's
music using voice commands.

OPTIONAL:
To reduce the library compiled size and limit its memory usage, you
can specify which shields you want to include in your sketch by
defining CUSTOM_SETTINGS and the shields respective INCLUDE_ define.

*/

#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_MUSIC_PLAYER_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_NFC_SHIELD


/* Include 1Sheeld library. */
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

const int vib = 3;
const int light = 2;
int flag = 0;
/* Voice commands set by the user. */
const char * personality1 = "parth";
const char * personality2 = "namita";
const char * personality3 = "izabela";
const char musicCommand[] = "music";
const char lightCommand[] = "lights";
const char vibrationCommand[] = "vibration";
const char audiobookCommand[] = "audiobook";
const char stopCommand[] = "stop";


void setup()
{
  /* Start Communication. */
  OneSheeld.begin();
  /* Error Commands handiling. */
  /* Subscribe to the new tag scanned event. */
  NFC.setOnNewTagScanned(&tagFunction);
  /* Subscribe to on Tag error event. */
  NFC.setOnError(&tagErrorFunction);
  /* Error Voice Recognition Commands handiling. */
  VoiceRecognition.setOnError(error);
  TextToSpeech.say("I AM DEXTER YOUR PERSONAL SAFETY ASSISTANT");
  delay(5000);
  TextToSpeech.say("Good afternoon, I see that you have not slept well the day before, which of the settings would you like me to enable");
  delay(10000);
  TextToSpeech.say("Your options are : Vibration, Lights, Music and Audiobook");
  delay(5000);
  pixels.begin(); // This initializes the NeoPixel library.
  VoiceRecognition.start();

}

void loop ()
{
  /* Check if new command received. */
  if (VoiceRecognition.isNewCommandReceived())
  {
    /* Compare the play command. */
    if (!strcmp(musicCommand, VoiceRecognition.getLastCommand()))
    {
      TextToSpeech.say("MUSIC ON");
      delay(2000);
      //if(flag = 0)
      //{
      /* MUSIC */
      MusicPlayer.next();
      //}
      //else if(flag =1)
      //{
      //  MusicPlayer.play();
      //}
      //flag =0;
    }
    /* Compare the delay command. */
    else if (!strcmp(lightCommand, VoiceRecognition.getLastCommand()))
    {
      TextToSpeech.say("LIGHTS ON");
      delay(2000);
      /* LIGHT. */
      for (int i = 0; i < NUMPIXELS; i++) {

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Moderately bright green color.

        pixels.show(); // This sends the updated pixel color to the hardware.


      }

    }
    else if (!strcmp(vibrationCommand, VoiceRecognition.getLastCommand()))
    {
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
      /* Stop the track. */
      MusicPlayer.stop();
      digitalWrite(vib, LOW);
      for (int i = 0; i < NUMPIXELS; i++) {

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.

        pixels.show(); // This sends the updated pixel color to the hardware.


      }
    }
    /* Compare the next command. */
    else if (!strcmp(audiobookCommand, VoiceRecognition.getLastCommand()))
    {
      TextToSpeech.say("AUDIOBOOK ON");
      delay(2000);
      /* Next track. */
      MusicPlayer.next();
      flag = 1;
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

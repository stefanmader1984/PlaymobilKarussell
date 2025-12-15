/***************************************************
DFPlayerMini PINS:
GND: Speaker -
SPK_1: Speaker +
TX: 11
RX: 10
VCC: 3V

Motordriver MX1508 PINS:
INT1: 5
INT2: 6
VCC: 3V
GND

LEDS PINS:
VCC: 5V
GND
Data: A0

Button PINS:
GND
Button1: 8
 ****************************************************/

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "Adafruit_NeoPixel.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(11,10);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif


DFRobotDFPlayerMini myDFPlayer;

const int ForwardPin = 6;   // Pin for forward motor control
const int BackwardPin = 5;  // Pin for backward motor control
const int MaxSpeed = 200;    // Maximum speed (0-255)
const int ledPin = A0;
const int PIN_Button1 = 8;
int animateCount = 0;
int timercount = 0;

#define N_LEDS 8
#define LEDBRIGTHNESS 20 //128
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, ledPin, NEO_RGB + NEO_KHZ800);


void setup()
{
  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
  pinMode(PIN_Button1, INPUT_PULLUP);

  strip.begin();

  setLightWHITE();

  FPSerial.begin(9600);
  Serial.begin(115200);
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30

  

}

static void setLightWHITE()
{
  int r = 255;
  int g = 0;
  int b = 100;

  for (uint16_t i = 0; i < N_LEDS; i=i+2) {
    strip.setPixelColor(i, strip.Color(r,g,b));
  }
  r = 255;
  g = 100;
  b = 0;
  for (uint16_t i = 1; i < N_LEDS; i=i+2) {
    strip.setPixelColor(i, strip.Color(r,g,b));
  }
  strip.setBrightness(LEDBRIGTHNESS);
  strip.show(); 
  delay(30);
}

static void animateLight()
{
  int r;
  int g;
  int b;

  switch (animateCount) {
      case 0:
        r = 0;
        g = 255;
        b = 0;
        for (uint16_t i = 0; i < N_LEDS; i=i+2) {
          strip.setPixelColor(i, strip.Color(r,g,b));
        }
        r = 0;
        g = 0;
        b = 255;
        for (uint16_t i = 1; i < N_LEDS; i=i+2) {
          strip.setPixelColor(i, strip.Color(r,g,b));
        }
        animateCount++;
        break;

      case 1:
        r = 100;
        g = 100;
        b = 0;
        for (uint16_t i = 0; i < N_LEDS; i=i+2) {
          strip.setPixelColor(i, strip.Color(r,g,b));
        }
        r = 100;
        g = 0;
        b = 100;
        for (uint16_t i = 1; i < N_LEDS; i=i+2) {
          strip.setPixelColor(i, strip.Color(r,g,b));
        }
        animateCount++;
        break;

      case 2:
        r = 255;
        g = 0;
        b = 100;
        for (uint16_t i = 0; i < N_LEDS; i=i+2) {
          strip.setPixelColor(i, strip.Color(r,g,b));
        }
        r = 255;
        g = 100;
        b = 0;
        for (uint16_t i = 1; i < N_LEDS; i=i+2) {
          strip.setPixelColor(i, strip.Color(r,g,b));
        }
        animateCount = 0;
        break;

      default:
        break; // Wird nicht benötigt, wenn Statement(s) vorhanden sind
    }

  strip.show();
  timercount++;
  delay(500);
}

void loop()
{
  delay(50);


  if (digitalRead(PIN_Button1) == LOW)
  {
    myDFPlayer.play(1);  //Play the first mp3
    analogWrite(ForwardPin, MaxSpeed);  // Set forward motor pin to MaxSpeed

    timercount = 0;
    while (timercount < 32)
    {
      animateLight();
    }

    analogWrite(ForwardPin, 0);         // Stop motor
    setLightWHITE();
    delay(1000);                          // Short delay before changing direction
  }

}



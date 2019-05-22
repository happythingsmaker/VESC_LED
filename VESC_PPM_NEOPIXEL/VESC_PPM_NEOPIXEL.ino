
#include <FastLED.h>

#include "makros.h"
#include "debug.h"
#include "RCReceive.h"
/*
  RC_Template.ino - Template for RC Receiver enabled programs interrupt version - Version 0.2
  Copyright (c) 2014 Wilfried Klaas.  All right reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#define LEDPIN  5
#define NUMPIXELS 24
CRGB leds[NUMPIXELS];


const byte PIN_RC = 3;

// Der Empfänger
RCReceive rcReceiver;

void setup() {
  // RC Receiver in Interruptvariante
  rcReceiver.attachInt(PIN_RC);
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, LEDPIN>(leds, NUMPIXELS);


  // put your setup code here, to run once:
}

long sum_ppm = 0;
const int num_sample = 100;
int temp_cnt = 0;
void loop() {
  
  //  brightLED(1);
  // nur wenn der Nullpunkt bestimmt worden ist,
  // und es keinen Fehler gegeben hat soll die eigentliche Arbeit gemacht werden
  if (rcReceiver.hasNP() && !rcReceiver.hasError()) {
    int value = rcReceiver.getValue();
    sum_ppm  += value;
    if ( temp_cnt++ > num_sample) {
      temp_cnt = 0;

      int ppm = (int)sum_ppm / num_sample;
      brightLED(ppm);
      Serial.println(ppm);
      sum_ppm = 0;

    }
  }
  else if (rcReceiver.hasError()) {
    // Fehlerbehandlung failsafe oder sowas...
  }


}

const int blingking_thresh = 5;
int blink_count = 0;
bool toggle  = true;
void brightLED(int ppm) {
  // Turn the LED on, then pause
  //  leds[0] = CRGB::Red;
  //  FastLED.show();
  //  //  delay(500);
  //  // Now turn the LED off, then pause
  //  leds[0] = CRGB::Black;
  if (blink_count++ >  blingking_thresh) {
    blink_count = 0;
    toggle = !toggle;
  }

  const int MIN_BRAKE = 70;
  const int MAX_THROTLE = 200;
  for (int i = 0; i < NUMPIXELS; i ++) {
    if (ppm < MIN_BRAKE) {

      if (toggle) {

        leds[i][0] = 255;
        leds[i][1] = 0;
        leds[i][2] = 0;  //b
      } else {
        leds[i][0] = 0;
        leds[i][1] = 0;
        leds[i][2] = 0;  //b
      }


    } else if (ppm > MAX_THROTLE) {
      leds[i][0] = 0;
      leds[i][1] = 100;
      leds[i][2] = 255;  //b

    } 
//    else if (ppm > 110 && ppm < 140) {
//      leds[i][0] = 0;
//      leds[i][1] = 0;
//      leds[i][2] = 255;  //b
//    }

    else {
      leds[i][0] = map(ppm, MIN_BRAKE, MAX_THROTLE, 255, 0); //r
      leds[i][1] = 30;
      //      leds[i][1] = map(ppm, 80, 200, 0, 255); //g
      //      leds[i][2] = 0;  //b
      leds[i][2] = map(ppm, MIN_BRAKE, MAX_THROTLE, 0, 255); //g

    }
  }
  FastLED.show();
  delay(10);
  
  //  delay(500);



}

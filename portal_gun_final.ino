#include <SPI.h>
#include <SD.h>
#define SD_ChipSelectPin 4  //using digital pin 4 on Arduino nano 328

#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>
TMRpcm tmrpcm;

#include <FastLED.h>
#define LED_PIN     6
#define NUM_LEDS    21
CRGB leds[NUM_LEDS];

#define DUO_TOGGLE 2
#define BLU_BUTT 3
#define ORG_BUTT 5
bool duo_line = true;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(ORG_BUTT, INPUT);
  pinMode(BLU_BUTT, INPUT); 
  pinMode(DUO_TOGGLE, INPUT);

  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
  tmrpcm.volume(5);
  tmrpcm.quality(1);

  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD Failed.");
    return;   // don't do anything more if not
  }
  Serial.println("SD Connected.");


  fill_solid(leds, NUM_LEDS, CRGB (0, 0, 0));
  tmrpcm.play("PWR.wav");
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CRGB (0,36,135);
    FastLED.show();
    delay(60);
  }
  delay(2500);
  
  if(digitalRead(DUO_TOGGLE) == LOW) {
    tmrpcm.play("GLA.wav");
    delay(7000);
  }
}

void shoot_blue(){
  fill_solid(leds, NUM_LEDS, CRGB (0,36,135));
  FastLED.show();
  tmrpcm.play("BLU.wav");
  delay(1200);
}

void shoot_orange(){
  fill_solid(leds, NUM_LEDS, CRGB (85,13,0));
  FastLED.show();
  tmrpcm.play("ORG.wav");
  delay(1200);
}

void error() {
  fill_solid(leds, NUM_LEDS, CRGB (0,9,31));
  FastLED.show();
  tmrpcm.play("FAL.wav");
  delay(1000);
  fill_solid(leds, NUM_LEDS, CRGB (0,36,135));
  FastLED.show();
}


void loop(){

  if(digitalRead(DUO_TOGGLE) == LOW) {
    fill_solid(leds, NUM_LEDS, CRGB (0,36,135));
    FastLED.show();

    if(digitalRead(BLU_BUTT) == HIGH) {
      shoot_blue();
    }

    if(digitalRead(ORG_BUTT) == HIGH) {
      error();
    }
  }

  if(digitalRead(DUO_TOGGLE) == HIGH) {
    if(duo_line) {
      tmrpcm.play("DOS.wav");
      delay(5500);
      duo_line = false;
    }

    if(digitalRead(ORG_BUTT) == HIGH) {
      shoot_orange();
    }

    if(digitalRead(BLU_BUTT) == HIGH) {
      shoot_blue();
    }
  }
}
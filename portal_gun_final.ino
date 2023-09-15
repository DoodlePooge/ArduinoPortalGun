#include <SPI.h>
#include <SD.h>
#define SD_ChipSelectPin 4  //using digital pin 4 on Arduino nano 328

#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>
TMRpcm tmrpcm;

#include <Adafruit_NeoPixel.h>
#define LED_PIN     6
#define NUM_LEDS    30
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define DUO_TOGGLE A2
#define COL_TOGGLE A3
#define BUTTON A4
bool duo_line = true;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
  pinMode(COL_TOGGLE, INPUT); 
  pinMode(DUO_TOGGLE, INPUT);
  strip.begin();
  strip.clear();

  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
  tmrpcm.volume(5);
  tmrpcm.quality(1);

  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD Failed.");
    return;   // don't do anything more if not
  }
  Serial.println("SD Connected.");



  tmrpcm.play("PWR.wav");
  delay(500);

  int j = NUM_LEDS;
  for (int i = 0; i <= NUM_LEDS; i++) {
    strip.setPixelColor(i, 10,30,255);
    strip.show();
    delay(15);
    strip.setPixelColor(j, 10,30,255);
    j--;
    strip.show();
    delay(15);
  }
  delay(2500);
  
  if(digitalRead(DUO_TOGGLE) == LOW) {
    tmrpcm.play("GLA.wav");
    delay(7000);
  }
}

void shoot_blue(){
  setColor(10,30,255);
  strip.show();
  tmrpcm.play("BLU.wav");
  delay(1200);
}

void shoot_orange(){
  setColor(255,40,0);
  strip.show();
  tmrpcm.play("ORG.wav");
  delay(1200);
}

void error() {
  setColor(0,9,31);
  strip.show();
  tmrpcm.play("FAL.wav");
  delay(1000);
  setColor(10,30,255);
  strip.show();
}

void setColor(int r, int g, int b) {
  for (int i = 0; i <= NUM_LEDS; i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

void wait() {
  while(digitalRead(BUTTON) == LOW){
    if(duo_line && digitalRead(DUO_TOGGLE) == HIGH){
      tmrpcm.play("DOS.wav");
      delay(5500);
      duo_line = false;
    } else {
      setColor(10,30,255);
      strip.show();
    }
  }
}


void loop(){

  wait();

  if(digitalRead(COL_TOGGLE) == LOW) {
    shoot_blue();
  } else {
    if(digitalRead(DUO_TOGGLE) == LOW) {
      error();
    } else {
      shoot_orange();
    }
  }
}
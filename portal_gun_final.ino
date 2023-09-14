#include <SPI.h>
#include <SD.h>
#define SD_ChipSelectPin 4  //using digital pin 4 on Arduino nano 328

#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>
TMRpcm tmrpcm;

#include <Adafruit_NeoPixel.h>
#define LED_PIN     6
#define NUM_LEDS    200
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define DUO_TOGGLE A1
#define BLU_BUTT A2
#define ORG_BUTT A3
bool duo_line = true;

void setup() {
  Serial.begin(9600);
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


  strip.begin();
  strip.setBrightness(40);
  strip.clear();

  tmrpcm.play("PWR.wav");
  delay(500);

  for (int i = 0; i <= NUM_LEDS; i++) {
    strip.setPixelColor(i, 0,36,135);
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
  strip.Color(0,36,135);
  strip.show();
  tmrpcm.play("BLU.wav");
  delay(1200);
}

void shoot_orange(){
  strip.Color(85,13,0);
  strip.show();
  tmrpcm.play("ORG.wav");
  delay(1200);
}

void error() {
  strip.Color(0,9,31);
  strip.show();
  tmrpcm.play("FAL.wav");
  delay(1000);
  strip.Color(0,36,135);
  strip.show();
}


void loop(){

  if(digitalRead(DUO_TOGGLE) == LOW) {
    strip.Color(0,36,135);
    strip.show();

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

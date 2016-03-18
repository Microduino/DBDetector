#include "userDef.h"
#include <U8glib.h> 
#include "oled.h"
#include <Microduino_Key.h>
#include <Adafruit_NeoPixel.h>

Key KeyButton(key_pin, INPUT_PULLUP);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(mic_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(PIN, OUTPUT);
  strip.begin();
  strip.show();
}

void loop() {
  getDB();
  analyticDB();
  updateOLED();
  updateButton();
  speakerDoing();
}

void updateOLED() {
  //OLED display
  if (OLEDShowTime > millis()) OLEDShowTime = millis();
  if(millis()-OLEDShowTime>INTERVAL_OLED) {
    OLEDShow(); //调用显示库
    OLEDShowTime = millis();
  } 
}


void buzzer() {
  if (millis() - timer > 10) {
    if (!add) {
      i++;
      if (i >= 800)
        add = true;
    } else {
      i--;
      if (i <= 200) {
        add = false;
      }
    }
    tone(buzzer_pin, i);
    timer = millis();
  }
}


void getDB() {
  voice_data = analogRead(mic_pin);
  voice_data=map(voice_data,0,1023,0,5);
  db = (20. * log(10)) * (voice_data / 1.0);
  if(db>recodeDB) {
    recodeDB=db;
  }
  Serial.println(db);
}

void analyticDB() {
    if(db > voice) {
    numNoise++;
    Serial.println(numNoise);
  }
  if (analytic_time > millis()) analytic_time = millis();
  if (millis() - analytic_time > INTERVAL_LCD) {
    if(numNoise>maxNoise) {
        buzzer_speak = true;
        i = 200;
    }
//     Serial.print(numNoise);
//    Serial.print("\t");
//    Serial.println(maxNoise);
    numNoise=0;
    analytic_time = millis();
  }
}

void updateButton() {
  if(KeyButton.read()==SHORT_PRESS) {
    delay(15);
    buzzer_speak = false;
    recodeDB=0;
  }

}

void speakerDoing() {
  if (buzzer_speak) {
    buzzer();
    strip.setPixelColor(0, strip.Color(125, 125, 125));
    strip.show();
  } else {
    noTone(buzzer_pin);
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.show();
  }
}

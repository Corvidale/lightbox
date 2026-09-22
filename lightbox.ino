#include <Adafruit_NeoPixel.h>

#define LED_PIN A0
#define NUM_LEDS 30
int pos = 0;
int speed = 30;
bool reverse = false;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.clear();
  strip.show();
  Serial.begin(9600);
}

void loop() {
  
    strip.setBrightness(255);

    //strip.setPixelColor(i, strip.Color((255/30)*i, 255-(255/30)*i, 255-(255/30)*i));

  
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i == pos) {
      strip.setPixelColor(i, strip.Color(random(255),0, random(255)));
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
  if (reverse) {
    pos--;
  } else {
    pos++;
  }
  if (pos > 29) {
    Player2Punch(random(40));
  }
  if (pos < 0) {
    Player1Punch(random(40));
  }
  
  strip.show();
  delay(speed);
  Serial.println(pos);
}
void Player1Punch(int intensity) { //positive
  speed = 50 - intensity;
  reverse = false;
}
void Player2Punch(int intensity) { //negative
  speed = 50 - intensity;
  reverse = true;
}

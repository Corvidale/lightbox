#include <Adafruit_NeoPixel.h>
#include <7Semi_BNO055.h>

BNO055_7Semi imu;
#define LED_PIN A0
#define NUM_LEDS 30

int pos = 0;
int speed = 30;
bool reverse = false;
int max = 11000;
int frames = 5;
bool readMaxxing = false;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.clear();
  strip.show();
  Serial.begin(115200);
  delay(1000);

    if (!imu.begin())
    {
        Serial.println("BNO055 not detected!");
        while (true);
    }

    Serial.println("BNO055 OK");
}

void loop() {
  
  AcceCheck();
  
  if (readMaxxing && frames > 0) {
    frames--;

  } else {

  }

  strip.setBrightness(255);

  //strip.setPixelColor(i, strip.Color((255/30)*i, 255-(255/30)*i, 255-(255/30)*i));

  
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i == pos) {
      strip.setPixelColor(i, 255,0, 0);
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
  if (pos < 0 ) {pos = 0;}
  
  strip.show();
  delay(speed);
}
void Player1Punch(int intensity) { //positive
  speed = 50 - intensity;
  reverse = false;
}
void Player2Punch(int intensity) { //negative
  speed = 50 - intensity;
  reverse = true;
}

void AcceCheck(){
  int x, y, z;

    imu.readAccel(x, y, z);

    int impact = abs(x)+abs(y)+abs(z);

    if (impact > 3000) {
      int output = map(impact, 3000, max, 0, 30);
      output = constrain(output, 0, 30);
      Player1Punch(output);

      Serial.print("HIT!!!! OWIEEE, it hurt about, ");
      Serial.print(impact);
      Serial.print(" : ");
      Serial.print(max);
      Serial.print(" or ");
      Serial.print(output);
      Serial.print(" : ");
      Serial.println(30);

      
  }
}

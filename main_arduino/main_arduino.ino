#include <7Semi_BNO055.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

BNO055_7Semi imu;

#define LED_PIN A0
#define NUM_LEDS 30

SoftwareSerial mySerial(2, 3);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


        //CONST DATA
const int button = 5;
const int buttonLed = button-1;



        //GLOBAL DATA
int speed = 100;
int base = 10;
int pos = 0;
bool reverse = false;
int max = 11000;
int frames = 5;

int state = 1; // 1 = spiller, 2 = player 1 vandt, 3 = player 2 vandt

            // start alt det shet
void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  mySerial.setTimeout(10); // stop parseInt() fra at stoppe programmet i 1000ms hver frame
  pinMode(button, INPUT);
  pinMode(buttonLed, OUTPUT);
  LightStart();

  if (!imu.begin()) {
    Serial.println("BNO055 on MAIN not detected!");
    while (true);
  }
  Serial.println("MAIN ready. Listening for SECONDARY...");
}


        //loopy lui
void loop() {
  if (CheckReset()) {
    state = 1;
    speed = 100;
    base = 10;
    pos = 0;
    reverse = false;
    digitalWrite(buttonLed, HIGH);
  } else {
    digitalWrite(buttonLed, LOW);
  }

  switch (state) {
    case 1:
      Game();
    break;
    case 2:
      GameEnded(1);
    break;
    case 3:
      GameEnded(2);
    break;
  }
}


void CheckPlayer1Punch(int data) {
  if (base > pos) {
    if (data > 3000) {
      int punch = ConvertImpact(data);
      Player1Punch(punch);
    }
  }
}
void CheckPlayer2Punch(int data) {
  if (NUM_LEDS - base < pos) {
    if (data > 3000) {
      int punch = ConvertImpact(data);
      Player2Punch(punch);
    }
  }
}




void Player1Punch(int intensity) { //positive
  speed = 30 - intensity;
  reverse = false;
}
void Player2Punch(int intensity) { //negative
  speed = 30 - intensity;
  reverse = true;
}

float ConvertImpact(int impact) {
  int output = map(impact, 3000, max, 0, 30);
  return constrain(output, 0, 30);
}

float ReceiveLocalData() {
  int x, y, z;
  imu.readAccel(x, y, z);
  
  return abs(x) + abs(y) + abs(z);
}
float ReceiveSecondaryData() {
  if (mySerial.available()) {
    int incomingData = mySerial.parseInt();
    
    Serial.print("MAIN received: ");
    Serial.println(incomingData);
    return incomingData;
  } else {
    return 1000;
  }
}


void LightStart() {
  strip.begin();
  strip.setBrightness(255);
  strip.clear();
  strip.show();
  delay(1000);
}



void Color1Light(int position) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i == position) {
      strip.setPixelColor(i, 255,0, 0);
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
}
bool CheckReset() {
  if (digitalRead(button) == LOW) {
    return true;
  } else {
    return false;
  }
}




void Game() {

  int p1Data = ReceiveLocalData();
  int p2Data = ReceiveSecondaryData();
  
  Color1Light(pos);

  if (reverse) { //bevæg den vej du bevæger dig :)
    if (pos < 0) {
      //player 2 won
      state = 3;
    }
    CheckPlayer1Punch(p1Data);

    pos--; // end game if pos hits -1
  } else {
    if (pos >= 30) {
      //player 1 won
      state = 2;
    }
    CheckPlayer2Punch(p2Data);
    pos++; // end game if pos hits 29

  }

  strip.show();
  delay(speed);
}

void GameEnded(int player) {
  switch(player) {
    case 1:
      ColorAll(255,0,0);
    break;
    case 2:
    ColorAll(0,255,0);
    break;
  }
  strip.show();
  delay(50);
}
void ColorAll(int r, int g, int b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, r,g, b);
  }
}

#include <7Semi_BNO055.h>
#include <SoftwareSerial.h>
BNO055_7Semi imu;
SoftwareSerial mySerial (2,3);
void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  
  if (!imu.begin())
    {
        Serial.println("BNO055 not detected!");
        while (true);
    }
}
void loop() {
  if (mySerial.available() > 0) {
    int chowa = mySerial.read();
    Serial.println(chowa);
  }
  delay(250);
}
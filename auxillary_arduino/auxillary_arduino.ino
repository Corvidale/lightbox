#include <7Semi_BNO055.h>

BNO055_7Semi imu;

void setup() {
  Serial.begin(115200);
  if (!imu.begin())
    {
        Serial.println("BNO055 not detected!");
        while (true);
    }
}
void loop() {
  int x, y, z;

  imu.readAccel(x, y, z);
  int output = abs(x)+abs(y)+abs(z);
  Serial.println(output);
  delay(250);
}
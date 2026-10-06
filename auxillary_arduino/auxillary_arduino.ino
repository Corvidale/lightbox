#include <7Semi_BNO055.h>
#include <SoftwareSerial.h>

BNO055_7Semi imu;

// SoftwareSerial(rxPin, txPin)
// Pin 2 = RX (unused), Pin 3 = TX (sends to MAIN)
SoftwareSerial mySerial(2, 3);

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  if (!imu.begin()) {
    Serial.println("BNO055 on SECONDARY not detected!");
    while (true);
  }
  Serial.println("SECONDARY initialized and transmitting...");
}

void loop() {
  int x, y, z;
  imu.readAccel(x, y, z);
  
  int output = abs(x) + abs(y) + abs(z);
  
  // Transmit to MAIN
  mySerial.println(output);
  
  // Debug output to PC Serial Monitor
  Serial.print("SECONDARY sent: ");
  Serial.println(output);

  delay(250);
}
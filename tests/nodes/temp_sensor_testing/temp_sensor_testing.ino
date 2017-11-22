#include <Wire.h>
#define TEMP_ADDR 0x48

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = getTemp();
  Serial.println(temp);
  delay(1500);
}

float getTemp()
{
  Wire.requestFrom(TEMP_ADDR, 2);
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  int sum = ( (MSB << 8) | LSB ) >> 4;
  return sum*0.0625;
}


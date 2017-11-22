#include <SoftwareSerial.h>
#define BAUDRATE 9600
#define RX 3
#define TX 2

SoftwareSerial xbee(RX, TX);
int i;
void setup()
{
  // put your setup code here, to run once:
  i = 0;
  xbee.begin(BAUDRATE);
}

void loop()
{
  // put your main code here, to run repeatedly:
  xbee.print(i++);
  delay(1000);
}

/*
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
*/
#include <SoftwareSerial.h>
#include <Printers.h>
#include <XBee.h>
#include <Wire.h>   //header for tmp102

#define rxPin 2
#define txPin 3

SoftwareSerial xbeeSerial(rxPin, txPin); // RX(D2)(xbee out), TX(D3) (xbee in)
int tmp102Address = 0x48;
// create the XBee object
XBee xbee = XBee();
uint8_t payload[1];

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x409BD794);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void setup() {
  Wire.begin();
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(115200);
  // set the data rate for the SoftwareSerial port
  xbeeSerial.begin(115200);
  xbee.setSerial(xbeeSerial); 
}

float getTemperature(){
  Wire.requestFrom(tmp102Address,2); 

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 
  float celsius = TemperatureSum*0.0625;
  return celsius;
}

void loop() {
    float celsius = getTemperature();
    //Serial.print("Celsius: "); Serial.println(celsius);

    uint8_t f = (1.8 * celsius) + 32;  
    Serial.print("Fahrenheit: "); Serial.println(f);
    payload[0] = f;
    xbeeSerial.println(payload[0]);
    xbee.send(zbTx);
    
    delay(3000);
}




/*
 Created by: Jonathan Moriarty
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
*/
#include <SoftwareSerial.h>
#include <Printers.h>
#include <XBee.h>
#include <Wire.h>   //header for tmp102

#define rxPin 2
#define txPin 3
#define RED 6
#define GREEN 7
//32-bit hub/destination address
#define HUB_ADDR_H 0x0013A200
#define HUB_ADDR_L 0x409BD794

SoftwareSerial xbeeSerial(rxPin, txPin); // RX(D2)(xbee out), TX(D3) (xbee in)
int tmp102Address = 0x48;
uint8_t temp = 0;
// create the XBee object
XBee xbee = XBee();
uint8_t payload[1];

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(HUB_ADDR_H, HUB_ADDR_L);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void setup() {
  Wire.begin();
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting up!");
  // set the data rate for the SoftwareSerial port
  xbeeSerial.begin(115200);
  xbee.setSerial(xbeeSerial); 
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
}

uint8_t getTemperature(){
  Wire.requestFrom(tmp102Address, 2); 
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 
  float celsius = TemperatureSum*0.0625;
  //Serial.print("Celsius: "); Serial.println(celsius);
  float tempF = (1.8 * celsius) + 32;  //conver to farenheit
  return tempF;
}
void blink(int pin) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
}
void loop() {
    temp = getTemperature();
    Serial.print("Fahrenheit: "); Serial.println(temp);
    payload[0] = temp;
    //xbeeSerial.println(temp); //not needed
    xbee.send(zbTx);

    //Control light colors based on temp. reading
    if (temp >= 81) {
      digitalWrite(GREEN, LOW);
      blink(RED);
    }
    else if (temp < 81) {
      digitalWrite(RED, LOW);
      blink(GREEN);
    }
    delay(1000);
}




/*
 Created by: Jonathan Moriarty
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
*/
#include <SoftwareSerial.h>
//#include <Printers.h>
//#include <XBee.h>
#include <Wire.h>   //header for tmp102

#define rxPin 2
#define txPin 3
#define RED 9
#define GREEN 11
#define MYID -1
//32-bit hub/destination address
#define HUB_ADDR_H 0x0013A200
#define HUB_ADDR_L 0x409BD794
#define THRESHOLDTEMP 81
#define BUFFERSIZE 200

SoftwareSerial xbeeSerial(rxPin, txPin); // RX(D2)(xbee out), TX(D3) (xbee in)
int tmp102Addr0 = 0x48;
int temp102Addr1 = 0x49;
float temp0 = 0;
float temp1 = 0;
char frameBuff[BUFFERSIZE+1]; //buffersize + '\n'
// create the XBee object
//XBee xbee = XBee();
//uint8_t payload[1];

// SH + SL Address of receiving XBee
//XBeeAddress64 addr64 = XBeeAddress64(HUB_ADDR_H, HUB_ADDR_L);
//ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
//ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void setup() {
  Wire.begin();
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting up!");
  // set the data rate for the SoftwareSerial port
  xbeeSerial.begin(115200);
  //xbee.setSerial(xbeeSerial); 
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  // initialize pins
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
}

void getTemperature(){
  // get temp from first sensor
  Wire.requestFrom(tmp102Addr0, 2); 
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  int tempSum = ((MSB << 8) | LSB) >> 4; 
  float celsius = tempSum*0.0625;
  //Serial.print("Celsius: "); Serial.println(celsius);
  float temp0 = (1.8 * celsius) + 32;  //conver to farenheit
  
  // get temp from second sensor
  Wire.requestFrom(tmp102Addr1, 2); 
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  int tempSum = ((MSB << 8) | LSB) >> 4; 
  float celsius = tempSum*0.0625;
  //Serial.print("Celsius: "); Serial.println(celsius);
  float temp0 = (1.8 * celsius) + 32;  //conver to farenheit
}

void blink(int pin) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
}

void loop() {
// ##### I2C scanner ######
  byte error, address;
  int nDevices = 0;
  Serial.println(); Serial.println("Scanning...");
  for(address = 0x48; address < 0x4C; address++ )
  {
    // The i2c_scanner uses the return value of the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.print(address,HEX); Serial.println("!");
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknown error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
// ##### end I2C scanner ######

//    ### RECIEVE LOGIC ###
  memset(frameBuff, 0, sizeof(frameBuff));
  if (xbeeSerial.available() >= BUFFERSIZE) {
    for (int i = 0; i < BUFFERSIZE; i++) {
      frameBuff[i] = xbeeSerial.read();
    }
    frameBuff[i+1] = "\n";
    String frameStr(frameBuff);
    
    // 1) identify node => n<node#>(i.e. n0-n5)
    // 2) parse out instruction
    // 3/4) send response => frameToSend<nodeid, temp0Status, temp0, temp1Status, temp1>
    //OR carryout command
    
    // get indexes of delimiters (i.e. commas) within frame
    int delim0 = frameStr(frameBuff).indexOf(',');
    //int delim1 = frameStr(frameBuff).indexOf(',', delim0 + 1);    
    
    // frameStr="nodeid, instruction" -> instr. 0 = update request; 1 = command
    int nodeID = frameStr.substring(0, delim0).toInt();
    int instruction = frameStr.substring(delim0 + 1).toInt(); 
    
    if (nodeID = MYID) {
      switch (instruction) {
        case 0:   //update request => frameToSend<nodeid, temp0Status, temp0, temp1Status, temp1>
          getTemperature();
          Serial.print("Temp sensor 0(F): "); Serial.println(temp0);
          Serial.print("Temp sensor 1(F): "); Serial.println(temp1);
          //payload[0] = temp0&1;
          //xbee.send(zbTx);
                   
          String frameToSend = String(MYID) + /*temp0Stat +*/ + String(temp0) + /*temp1Stat +*/ + String(temp1);
          xbeeSerial.println(temp);
          break;
        
        case 1:   //command
          Serial.println("Got a command :P");
          //...
          break;
        
        default:
          Serial.println("Nada");
          break;
      }//end switch
    }
    else {
      continue;
    }







    
    

    //Control light colors based on temp. reading
    if (temp >= THRESHOLDTEMP) {
      digitalWrite(GREEN, LOW);
      blink(RED);
    }
    else if (temp < THRESHOLDTEMP) {
      digitalWrite(RED, LOW);
      blink(GREEN);
    }
    delay(1000);
  }//end .available()
}




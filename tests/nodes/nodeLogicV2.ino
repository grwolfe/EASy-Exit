/*
 Created by: Jonathan Moriarty
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
*/
#include <SoftwareSerial.h>
#include <Wire.h>           //header for tmp102

// Pin definitions:
#define THRESHOLDTEMP 80
#define MAXREDLEDS 3
#define MAXGRNLEDS 3 

#define RED0 4  
#define GRN0 5
#define RED1 6
#define GRN1 7
#define RED2 8
#define GRN2 9
#define RED3 10
#define GRN3 11
#define RED4 12
#define GRN4 13
#define RED5 14
#define GRN5 15
#define RED6 16
#define GRN6 17

const String MYID = "n8";
// TODO:  switch pin numbers for actual hub
const byte rxPin = 2;
const byte txPin = 3;
SoftwareSerial xbeeSerial(rxPin, txPin);	// RX(D2)(xbee out), TX(D3) (xbee in)

//const int REDLEDS[] = {RED0,RED1,RED2,RED3,RED4,RED5,RED6};
//const int GRNLEDS[] = {GRN0,GRN1,GRN2,GRN3,GRN4,GRN5,GRN6};
const int REDLEDS[] = {RED0, RED1, RED2};
const int GRNLEDS[] = {GRN0, GRN1, GRN2};
int redIndex = 0;
int grnIndex = 0;
int tmp102Addr0 = 0x48;   // (add0=GND)
int tmp102Addr1 = 0x49;   // (add1=VCC)
float temp0 = 0;
float temp1 = 0;
int temp0Stat = -1;
int temp1Stat = -1;
String frameTX = "";			// frame transmitted to hub
String frameRX = "";		// frame received from hub
String nodeID = "-1";
int instruction = -1;


void setup() {
	Wire.begin();
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	Serial.begin(9600);
	Serial.println("Starting up!");
	xbeeSerial.begin(9600);
// set pin mode:
	for (redIndex = 0; redIndex < MAXREDLEDS; redIndex++) {
		pinMode(REDLEDS[redIndex], OUTPUT);
	}
	for (grnIndex = 0; grnIndex < MAXGRNLEDS; grnIndex++) {
		pinMode(GRNLEDS[grnIndex], OUTPUT);
	}
// initialize pins:
	ledOFF();
// scan for temp sensors at startup	
	scanI2C();   
	blinkINIT();
}

void loop() {
	recMessage();
	if (MYID == nodeID) {
		switch (instruction) {
			case 0:		// update request => frameTX<nodeid, temp0Status, temp0, temp1Status, temp1>
				getTemperature();
				frameTX = String(MYID) + ',' + String(temp0Stat) + ',' + String(temp0) + ',' + String(temp1Stat) + ',' + String(temp1) ;
				xbeeSerial.println(frameTX);
				break;
			
			case 1: 
				Serial.println("Off command XD");
				xbeeSerial.println("Turning off LEDs");
				ledOFF();
				delay(1000);
				break;

			case 2:   //command
				Serial.println("Got a blink command :P");
				xbeeSerial.println("Blinking red LEDs");
				blinkRED();
				break;
				
			case 3:
				Serial.println("Got a blink command :P");
				xbeeSerial.println("Blinking green LEDs");
				blinkGRN();
				break;

			default:
				Serial.println("Nada");
				blinkINIT();
				break;
		}
	}
	else {
		getTemperature();
		Serial.print("Temp sensor 0(F): "); Serial.println(temp0);
		Serial.print("Temp sensor 1(F): "); Serial.println(temp1);
		
// temp0 affects first half of LEDs
		if (temp0 >= THRESHOLDTEMP) {	
			for (grnIndex = 0; grnIndex < MAXGRNLEDS/2; grnIndex++) {
				digitalWrite(GRNLEDS[grnIndex], LOW);
			}
			for (redIndex = 0; redIndex < MAXREDLEDS/2; redIndex++) {
				digitalWrite(REDLEDS[redIndex], HIGH);
			}
		}
		else if (temp0 < THRESHOLDTEMP) {
			for (redIndex = 0; redIndex < MAXREDLEDS/2; redIndex++) {
				digitalWrite(REDLEDS[redIndex], LOW);
			}
			for (grnIndex = 0; grnIndex < MAXGRNLEDS/2; grnIndex++) {
				digitalWrite(GRNLEDS[grnIndex], HIGH);
			}
		}
// temp1 affects second half of LEDs		
		if (temp1 >= THRESHOLDTEMP) {
			for (; grnIndex < MAXGRNLEDS; grnIndex++) {
				digitalWrite(GRNLEDS[grnIndex], LOW);
			}
			for (; redIndex < MAXREDLEDS; redIndex++) {
				digitalWrite(REDLEDS[redIndex], HIGH);
			}
		}
		else if (temp1 < THRESHOLDTEMP) {
			for (; redIndex < MAXREDLEDS; redIndex++) {
				digitalWrite(REDLEDS[redIndex], LOW);
			}
			for (; grnIndex < MAXGRNLEDS; grnIndex++) {
				digitalWrite(GRNLEDS[grnIndex], HIGH);
			}
		}
		else {
			for (grnIndex = 0; grnIndex < MAXGRNLEDS; grnIndex++) {
				digitalWrite(GRNLEDS[grnIndex], LOW);
			}
			for (redIndex = 0; redIndex < MAXREDLEDS; redIndex++) {
				digitalWrite(REDLEDS[redIndex], LOW);	
			}
		}
	}// end else
	delay(1000);
}// end loop

void recMessage() {		// RECIEVE LOGIC  --> sets "nodeID" and "instruction"
	int i = 0;
	nodeID = "-1";
	instruction = -1;
	char c;
	
	if ( xbeeSerial.available() ) {
		while ( xbeeSerial.available() > 0 && (xbeeSerial.peek() != '\n' || xbeeSerial.peek() != '\r') ) { 
			c = xbeeSerial.read();
			frameRX += c;
		}
		xbeeSerial.read();		//flushing '\n' OR '\r' from RX buffer
	   
		// get indexes of delimiters (i.e. commas) within frame
		int delim0 = frameRX.indexOf(',');
		//int delim1 = frameRX(frameBuff).indexOf(',', delim0 + 1);    
		
		// Parse message: frameRX="nodeid, instruction" -> instr. 0 = update request; 1 = command
		nodeID = frameRX.substring(0, delim0);
		instruction = frameRX.substring(delim0 + 1).toInt(); 
		Serial.print("nodeID: "); Serial.println(nodeID); Serial.print("instruction: "); Serial.println(instruction); 
		frameRX = "";
	}
}// end recMessage()

void scanI2C() {
// ##### I2C scanner ######
	byte error, address;
	int nDevices = 0;
	Serial.println(); Serial.println("Scanning...");
	for(address = 0x48; address < 0x4C; address++ ) {
// The i2c_scanner uses the return value of the Write.endTransmisstion to see if
// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (address==0x48)
			temp0Stat = error;
		if (address==0x49)
			temp1Stat = error;

		if (error == 0) {
			Serial.print("I2C device found at address 0x");
			if (address<16) 
				Serial.print("0");
			Serial.print(address,HEX); Serial.println("!");
			nDevices++;
		}
		else if (error==4) {
			Serial.print("Unknown error at address 0x");
			if (address<16) 
				Serial.print("0");
			Serial.println(address,HEX);
		}    
	}// end for loop
	if (nDevices == 0)
		Serial.println("No I2C devices found\n");
}
// ### end I2C scanner ####

void getTemperature() {
// get temp from first sensor (temp0)
	Wire.requestFrom(tmp102Addr0, 2); 
	byte MSB = Wire.read();
	byte LSB = Wire.read();
	int tempSum = ((MSB << 8) | LSB) >> 4; 
	float celsius = tempSum*0.0625;
	//Serial.print("Celsius: "); Serial.println(celsius);
	temp0 = (1.8 * celsius) + 32;  //conver to farenheit

// get temp from second sensor (temp1)
	Wire.requestFrom(tmp102Addr1, 2); 
	MSB = Wire.read();
	LSB = Wire.read();
	tempSum = ((MSB << 8) | LSB) >> 4; 
	celsius = tempSum*0.0625;
	//Serial.print("Celsius: "); Serial.println(celsius);
	temp1 = (1.8 * celsius) + 32;  //conver to farenheit
}

void ledOFF() {		// turn off all leds  
	for (redIndex = 0; redIndex < MAXREDLEDS; redIndex++) {
	   digitalWrite(REDLEDS[redIndex], LOW);
	}
	for (grnIndex = 0; grnIndex < MAXGRNLEDS; grnIndex++) {
	   digitalWrite(GRNLEDS[grnIndex], LOW);
	}
}
void blinkRED() {      
// turn off all leds
	ledOFF();
// turn on reds
	for (redIndex = 0; redIndex < MAXREDLEDS; redIndex++) {
		digitalWrite(REDLEDS[redIndex], HIGH);
		delay(500);
	}
	for (redIndex = 0; redIndex < MAXREDLEDS; redIndex++) {
		digitalWrite(REDLEDS[redIndex], LOW);
		delay(500);
	}
}
void blinkGRN() {    
// turn off all leds  
	ledOFF();
// turn on greens
	for (grnIndex = 0; grnIndex < MAXGRNLEDS; grnIndex++) {
		digitalWrite(GRNLEDS[grnIndex], HIGH);
		delay(500);
	}
	for (grnIndex = 0; grnIndex < MAXGRNLEDS; grnIndex++) {
		digitalWrite(GRNLEDS[grnIndex], LOW);
		delay(500);
	}
}
void blinkINIT() {    
	ledOFF();
// turn on reds
	for (redIndex = 0; redIndex < MAXREDLEDS; redIndex++) {
		digitalWrite(REDLEDS[redIndex], HIGH);
		delay(200);
	}
	ledOFF();
// turn on greens
	for (grnIndex = 0; grnIndex < MAXGRNLEDS; grnIndex++) {
		digitalWrite(GRNLEDS[grnIndex], HIGH);
		delay(200);
	}
	ledOFF();
	delay(200);
}


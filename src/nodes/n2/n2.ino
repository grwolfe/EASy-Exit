/*
 Created by: Jonathan Moriarty
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
*/
#include <SoftwareSerial.h>
#include <Wire.h>           //header for tmp102

// Pin definitions:
#define MAXLEDS 7

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

const String MYID = "n2";
// TODO:  switch pin numbers for actual hub
const byte rxPin = 3;
const byte txPin = 2;
SoftwareSerial xbeeSerial(rxPin, txPin);  // RX(D2)(xbee out), TX(D3) (xbee in)

const int REDLEDS[] = {RED0,RED1,RED2,RED3,RED4,RED5,RED6};
const int GRNLEDS[] = {GRN0,GRN1,GRN2,GRN3,GRN4,GRN5,GRN6};

int ledIndex = 0;
int tmp102Addr0 = 0x48;   // (add0=GND)
int tmp102Addr1 = 0x49;   // (add1=VCC)
float temp0 = 0;
float temp1 = 0;
int temp0Stat = -1;
int temp1Stat = -1;
String frameTX = "";      // frame transmitted to hub
String frameRX = "";    // frame received from hub
String nodeID = "-1";
int instruction = -1;
int pattern = -1;


void setup() {
	Wire.begin();
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	xbeeSerial.begin(9600);
	// set pin mode:
	for (ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
		pinMode(REDLEDS[ledIndex], OUTPUT);
		pinMode(GRNLEDS[ledIndex], OUTPUT);
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
		if (instruction != 0) {
			pattern = instruction;
			switch (pattern) {
				case 1:					// all OFF
					xbeeSerial.println("1");
					ledOFF();
					break;

				case 2:   				// Red ON
					xbeeSerial.println("1");
					redON();
					break;

				case 3:					// Grn ON
					xbeeSerial.println("1");
					grnON();
					break;
					
				case 4:					// Undulate red
					xbeeSerial.println("1");
					ledOFF();
					delay(250);
					for(ledIndex = 3; ledIndex < 6; ledIndex++) {
						digitalWrite(REDLEDS[ledIndex], HIGH);	
						delay(250);
					}
					for(ledIndex = 2; ledIndex >= 0; ledIndex--) {
						digitalWrite(REDLEDS[ledIndex], HIGH);	
						delay(250);
					}
					break;
					
				case 5:					// Undulate green
					xbeeSerial.println("1");
					ledOFF();
					delay(250);
					for(ledIndex = 3; ledIndex < 6; ledIndex++) {
						digitalWrite(GRNLEDS[ledIndex], HIGH);	
						delay(250);
					}
					for(ledIndex = 2; ledIndex >= 0; ledIndex--) {
						digitalWrite(GRNLEDS[ledIndex], HIGH);	
						delay(250);
					}
					break;

				default:
					xbeeSerial.println("0");
					break;
			}
		}
		else {
			frameTX = String(MYID) + ',' + String(temp0Stat) + ',' + String(temp0) + ',' + String(temp1Stat) + ',' + String(temp1) ;
			xbeeSerial.println(frameTX);
		}
	}
	else {
		getTemperature();
	}
}// end loop

void recMessage() {   // RECIEVE LOGIC  --> sets "nodeID" and "instruction"
	nodeID = "-1";
	instruction = -1;
	char c;

	if ( xbeeSerial.available() ) {
		while ( xbeeSerial.available() > 0 && xbeeSerial.peek() != '\n' && xbeeSerial.peek() != '\r' ) { 
			c = xbeeSerial.read();
			frameRX += c;
		}
		xbeeSerial.read();    //flushing '\n' OR '\r' from RX buffer
		 
		// get indexes of delimiters (i.e. commas) within frame
		int delim = frameRX.indexOf(',');

		// Parse message: frameRX="nodeid, instruction" -> instr. 0 = update request; 1 = command
		if( delim >= 0 ) {
			nodeID = frameRX.substring(0, delim);
			instruction = frameRX.substring(delim + 1).toInt();  
		}
		frameRX = "";
	}
}// end recMessage()

void scanI2C() {
	Wire.beginTransmission(tmp102Addr0);
	temp0Stat = Wire.endTransmission();
	Wire.beginTransmission(tmp102Addr1);
	temp1Stat = Wire.endTransmission();
}

void getTemperature() {
	if (temp0Stat == 0) {
		// get temp from first sensor (temp0)
		Wire.requestFrom(tmp102Addr0, 2); 
		byte MSB = Wire.read();
		byte LSB = Wire.read();
		int tempSum = ((MSB << 8) | LSB) >> 4; 
		float celsius = tempSum*0.0625;
		temp0 = (1.8 * celsius) + 32;  //conver to farenheit
	} 
	else {
		temp0 = 0;
	}
	
	if (temp1Stat == 0) {
		// get temp from second sensor (temp1)
		Wire.requestFrom(tmp102Addr1, 2); 
		byte MSB = Wire.read();
		byte LSB = Wire.read();
		int tempSum = ((MSB << 8) | LSB) >> 4; 
		float celsius = tempSum*0.0625;
		temp1 = (1.8 * celsius) + 32;  //conver to farenheit
	}
	else {
		temp1 = 0;
	}
}

void ledOFF() {		// turn off all leds  
	for (ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
		digitalWrite(GRNLEDS[ledIndex], LOW);
		digitalWrite(REDLEDS[ledIndex], LOW);
	}
}

void redON() {		// turn on all red leds
	for(ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
		digitalWrite(GRNLEDS[ledIndex], LOW);
		digitalWrite(REDLEDS[ledIndex], HIGH);		
	}
}
void grnON() {		// turn on all red leds
	for(ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
		digitalWrite(REDLEDS[ledIndex], LOW);
		digitalWrite(GRNLEDS[ledIndex], HIGH);		
	}
}

void blinkINIT() {    
	ledOFF();
	// turn on reds
	for( int i = 0; i < 2; i++ ) {
		for (ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
			digitalWrite(REDLEDS[ledIndex], HIGH);	
		}
		delay(100);
		ledOFF();
		delay(100);
	}
	// turn on greens
	for (ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
		digitalWrite(GRNLEDS[ledIndex], HIGH);
	}
	delay(150);
	ledOFF();
}

void redBLINK() {      
	// turn off all leds
	ledOFF();
	// turn on reds
	for (ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
		digitalWrite(REDLEDS[ledIndex], HIGH);
		delay(500);
	}
	ledOFF();
	delay(500);
}
void grnBLINK() {    
	// turn off all leds  
	ledOFF();
	// turn on greens
	for (ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
		digitalWrite(GRNLEDS[ledIndex], HIGH);
		delay(500);
	}
	ledOFF();
	delay(500);
}

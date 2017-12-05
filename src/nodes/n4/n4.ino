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

const String MYID = "n4";
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
					ledOFF();
					break;

				case 2:   				// Red ON
					redON();
					break;

				case 3:					// Grn ON
					grnON();
					break;
					
				case 4:					// Undulate red
					ledOFF();
					delay(250);
					for(ledIndex = 2; ledIndex < 5; ledIndex++) {
						digitalWrite(REDLEDS[ledIndex], HIGH);	
						delay(250);
					}
					digitalWrite(REDLEDS[5], HIGH);
					digitalWrite(REDLEDS[1], HIGH);
					delay(250);
					digitalWrite(REDLEDS[6], HIGH);
					digitalWrite(REDLEDS[0], HIGH);
					delay(250);
					break;
					
				case 5:					// Undulate green
					ledOFF();
					delay(250);
					for(ledIndex = 2; ledIndex < 5; ledIndex++) {
						digitalWrite(GRNLEDS[ledIndex], HIGH);	
						delay(250);
					}
					digitalWrite(GRNLEDS[5], HIGH);
					digitalWrite(GRNLEDS[1], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[6], HIGH);
					digitalWrite(GRNLEDS[0], HIGH);
					delay(250);
					break;
					
				case 6:				// Vertical hall RED
					ledOFF();
					delay(250);
					vertHall('r');
					break;
					
				case 7:				// Vertical hall GRN
					ledOFF();
					delay(250);
					vertHall('g');
					break;
					
				case 8:					// UP
					ledOFF();
					delay(250);
					digitalWrite(GRNLEDS[0], HIGH);
					digitalWrite(GRNLEDS[6], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[1], HIGH);
					digitalWrite(GRNLEDS[5], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[2], HIGH);
					digitalWrite(GRNLEDS[4], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[3], HIGH);	
					delay(250);
					break;
					
				case 9:				// DWON
					ledOFF();
					delay(250);
					digitalWrite(GRNLEDS[3], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[2], HIGH);	
					digitalWrite(GRNLEDS[4], HIGH);	
					delay(250);
					digitalWrite(GRNLEDS[1], HIGH);	
					digitalWrite(GRNLEDS[5], HIGH);	
					delay(250);
					digitalWrite(GRNLEDS[0], HIGH);	
					digitalWrite(GRNLEDS[6], HIGH);	
					delay(250);
					break;
					
				case 10:				// LFET
					ledOFF();
					delay(250);
					digitalWrite(GRNLEDS[6], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[5], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[0], HIGH);
					digitalWrite(GRNLEDS[4], HIGH);	
					delay(250);
					digitalWrite(GRNLEDS[1], HIGH);
					digitalWrite(GRNLEDS[3], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[2], HIGH);
					delay(250);
					break;
					
				case 11:				// RIHGT
					ledOFF();
					delay(250);
					for(ledIndex = 0; ledIndex < 7; ledIndex++) {
						digitalWrite(GRNLEDS[ledIndex], HIGH);	
						delay(250);
					}
					break;
					
				case 12:				// Drill #1 (towards nearest exit)
					ledOFF();
					delay(250);
					digitalWrite(GRNLEDS[2], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[1], HIGH);
					digitalWrite(GRNLEDS[3], HIGH);
					digitalWrite(GRNLEDS[4], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[5], HIGH);
					delay(50);
					digitalWrite(GRNLEDS[0], HIGH);
					delay(250);
					digitalWrite(GRNLEDS[6], HIGH);
					delay(250);

				default:
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
	delay(50);
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
		delay(150);
		ledOFF();
		delay(150);
	}
	// turn on greens
	for (ledIndex = 0; ledIndex < MAXLEDS; ledIndex++) {
		digitalWrite(GRNLEDS[ledIndex], HIGH);
	}
	delay(200);
	ledOFF();
}

void vertHall(char color) {
	if (color == 'r') {
		digitalWrite(REDLEDS[6], HIGH);
		digitalWrite(REDLEDS[0], HIGH);
		delay(50);
		digitalWrite(REDLEDS[5], HIGH);
		digitalWrite(REDLEDS[1], HIGH);		
		delay(50);
	}
	if (color == 'g') {
		digitalWrite(GRNLEDS[6], HIGH);	
		digitalWrite(GRNLEDS[0], HIGH);
		delay(50);
		digitalWrite(GRNLEDS[5], HIGH);
		digitalWrite(GRNLEDS[1], HIGH);
		delay(50);
	}
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

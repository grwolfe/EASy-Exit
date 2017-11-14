/*
 Created by: Jonathan Moriarty
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
*/
#include <SoftwareSerial.h>
#include <Wire.h>           //header for tmp102

// Pin definitions:
#define THRESHOLDTEMP 81
#define BUFFERSIZE 200
#define RED 9
#define GREEN 11
/* #define RED0
 * #define RED1
 * #define RED2
 * #define RED3
 * #define GREEN0
 * #define GREEN1
 * #define GREEN2
 * #define GREEN3 
 */

const String MYID = "n8";
const byte rxPin = 2;
const byte txPin = 3;
const int REDLEDS[7];
const int GRNLEDS[7];
int ledIndex = 0;
SoftwareSerial xbeeSerial(rxPin, txPin); // RX(D2)(xbee out), TX(D3) (xbee in)
int tmp102Addr0 = 0x48;   //(add0=GND)
int tmp102Addr1 = 0x49;   //(add1=VCC)
float temp0 = 0;
float temp1 = 0;
int temp0Stat = -1;
int temp1Stat = -1;
char frameBuff[BUFFERSIZE+1]; //buffersize + '\n'
String frameToSend;
String nodeID = "n?";
int instruction = -1;

void setup() {
	Wire.begin();
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	Serial.begin(115200);
	Serial.println("Starting up!");
	// set the data rate for the SoftwareSerial port
	xbeeSerial.begin(115200);
	//set pin mode
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	/*
	* for(ledIndex = 0; i < sizeof(REDLEDS); i++){
	*    pinMode(REDLEDS[i], OUTPUT);
	* {
	* for(ledIndex = 0; i < sizeof(GRNLEDS); i++){
	*    pinMode(GRNLEDS[i], OUTPUT);
	* {
	*/
	// initialize pins
	digitalWrite(RED, LOW);
	digitalWrite(GREEN, LOW);
	/*
	* for(ledIndex = 0; i < sizeof(REDLEDS); i++){
	*    digitalWrite(REDLEDS[i], LOW);
	* {
	* for(ledIndex = 0; i < sizeof(GRNLEDS); i++){
	*    digitalWrite(GRNLEDS[i], LOW);
	* {
	*/
	scanI2C();   //scan for temp sensors at startup
	//xbeeSerial.println("Hello XCTU!");
}

void loop() {
	nodeID = "n?";
	recMessage();
	if (MYID == nodeID) {
		switch (instruction) {
			case 0:                 //update request => frameToSend<nodeid, temp0Status, temp0, temp1Status, temp1>
				getTemperature();
				frameToSend = String(MYID) + ',' + String(temp0Stat) + ',' + String(temp0) + ',' + String(temp1Stat) + ',' + String(temp1) + '\n';
				xbeeSerial.println(frameToSend);
				break;

			case 1:   //command
				Serial.println("Got a command :P");
				//...
				break;

			default:
				Serial.println("Nada");
				break;
		}// end switch
	}
	else {
		getTemperature();
		Serial.print("Temp sensor 0(F): "); Serial.println(temp0);
		Serial.print("Temp sensor 1(F): "); Serial.println(temp1);
		delay(2500);
		if (temp0 >= THRESHOLDTEMP) {
			digitalWrite(GREEN, LOW);
			/*
			 * for(ledIndex = 0; i < sizeof(GRNLEDS); i++){
				digitalWrite(GRNLEDS[i], LOW);
			 */
			onLed(RED);
			/*for(ledIndex = 0; i < sizeof(REDLEDS); i++){
				onLed(REDLEDS[i]);
			  }
			*/
		}
		else if (temp0 < THRESHOLDTEMP) {
			digitalWrite(RED, LOW);
			/*
			 * for(ledIndex = 0; i < sizeof(REDLEDS); i++){
				digitalWrite(REDLEDS[i], LOW);
			 */
			onLed(GREEN);
			/*for(ledIndex = 0; i < sizeof(GRNLEDS); i++){
				onLed(GRNLEDS[i]);
			  }
			*/
		}
	//  if (temp1 >= THRESHOLDTEMP) {
	//    digitalWrite(GREEN, LOW);
	//    blink(RED);
	//  }
	//  else if (temp1 < THRESHOLDTEMP) {
	//    digitalWrite(RED, LOW);
	//    blink(GREEN);
	//  }
		else {
			digitalWrite(RED, LOW);
			digitalWrite(GREEN, LOW);
		}
	}
}// end loop

void recMessage() {		// RECIEVE LOGIC 
	memset(frameBuff, 0, sizeof(frameBuff));
	int i = 0;
	if (xbeeSerial.available() > 0) {
		delay(2);
		//while (xbeeSerial.available() > 0 && (xbeeSerial.peek() != '\n')) { 
		while (xbeeSerial.peek() != '\n') { 
			if ( xbeeSerial.available() )
				frameBuff[i++] = xbeeSerial.read();
		}
		xbeeSerial.read();		//flushing '\n' from RX buffer
		frameBuff[i] = '\0';    //null terminate frame buffer string
		String frameStr(frameBuff);
	   
		// get indexes of delimiters (i.e. commas) within frame
		int delim0 = frameStr.indexOf(',');
		//int delim1 = frameStr(frameBuff).indexOf(',', delim0 + 1);    
		
		// Parse message: frameStr="nodeid, instruction" -> instr. 0 = update request; 1 = command
		nodeID = frameStr.substring(0, delim0);
		instruction = frameStr.substring(delim0 + 1).toInt(); 
		Serial.print("nodeID: "); Serial.println(nodeID); Serial.print("instruction: "); Serial.println(instruction); 
		Serial.print("Whole packet:"); Serial.print(frameStr);
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
	// get temp from first sensor
	Wire.requestFrom(tmp102Addr0, 2); 
	byte MSB = Wire.read();
	byte LSB = Wire.read();
	int tempSum = ((MSB << 8) | LSB) >> 4; 
	float celsius = tempSum*0.0625;
	//Serial.print("Celsius: "); Serial.println(celsius);
	temp0 = (1.8 * celsius) + 32;  //conver to farenheit

// get temp from second sensor
	Wire.requestFrom(tmp102Addr1, 2); 
	MSB = Wire.read();
	LSB = Wire.read();
	tempSum = ((MSB << 8) | LSB) >> 4; 
	celsius = tempSum*0.0625;
	//Serial.print("Celsius: "); Serial.println(celsius);
	temp1 = (1.8 * celsius) + 32;  //conver to farenheit

}

void onLed(int pin) {      // turn on led and wait
    digitalWrite(pin, HIGH);
    delay(1000);
}
void offLed(int pin) {      // turn on led and wait
    digitalWrite(pin, LOW);
    delay(1000);
}


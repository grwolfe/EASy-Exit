#ifndef HUB_H
#define HUB_H

#include "mbed.h"

#define PC_BAUD     115200
#define XB_BAUD     9600
#define PC_BUFFSIZE 256
#define XB_BUFFSIZE 200
#define READY       true
#define NOT_READY   false
#define PC_UPRATE   1

/* SERIAL COMMUNICATIONS BUFFERS */
char pc_buff[PC_BUFFSIZE];  // buffer to hold messages from PC
char xb_buff[XB_BUFFSIZE];  // buffer to hold messages from XBee

/* SERIAL COMMUNICATION FLAGS */
volatile bool pc_msg = NOT_READY;   // PC message ready flag
volatile bool xb_msg = NOT_READY;   // XBee message ready flag

/* MBED OBJECT DEFINITIONS */
Ticker timer;
Serial pc( USBTX, USBRX, PC_BAUD ); // connection to PC / GUI
Serial xbee( p13, p14, XB_BAUD );   // connection to XBee / Nodes

/* FUNCTION DEFINITIONS */
void PC_RX_ISR();
void XB_RX_ISR();
void init();
void reset();
void update_gui();

#endif /* HUB_H */
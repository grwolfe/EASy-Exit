#ifndef HUB_H
#define HUB_H

#include "mbed.h"
#include "Node.h"

#define NUM_NODES   6
#define PC_BAUD     9600
#define XB_BAUD     9600
#define PC_BUFFSIZE 256
#define XB_BUFFSIZE 200
#define READY       true
#define NOT_READY   false
#define PC_UPRATE   1

bool emergency = false;

/* SERIAL COMMUNICATIONS BUFFER & FLAG */
char pc_buff[PC_BUFFSIZE];
volatile bool pc_msg = NOT_READY;

/* MBED OBJECT DEFINITIONS */
Ticker timer;
Serial pc( USBTX, USBRX, PC_BAUD ); // connection to PC / GUI
Serial xbee( p13, p14, XB_BAUD );   // connection to XBee / Nodes

/* NODE OBJECT DEFINITIONS */
Node nodes[] = 
{
    Node(0),
    Node(1),
    Node(2),
    Node(3),
    Node(4),
    Node(5)
};

/* FUNCTION DEFINITIONS */
void PC_RX_ISR();
void XB_RX_ISR();
void init();
void reset();
void update_gui();

#endif /* HUB_H */
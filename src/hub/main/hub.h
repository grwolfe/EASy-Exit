#ifndef HUB_H
#define HUB_H

#include "mbed.h"
#include "Node.h"

#define NUM_NODES   6
#define PC_BAUD     9600
#define XB_BAUD     9600
#define PC_BUFFSIZE 256
#define READY       true
#define NOT_READY   false

//TODO: make these defines into an enum
#define UPDATE  0
#define OFF     1
#define RED     2
#define GRN     3
#define UP      8
#define DOWN    9
#define LEFT    10
#define RIGHT   11
#define DRILL   12

/* SERIAL COMMUNICATIONS BUFFER & FLAG */
char pc_buff[PC_BUFFSIZE];
volatile bool pc_rdy = NOT_READY;

/* MBED OBJECT DEFINITIONS */
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
bool emergencies[NUM_NODES];

/* FUNCTION DEFINITIONS */
void PC_RX_ISR();
void init();
void reset();
void command_all( const int c );
void process_command();
void quit();
void update_gui();
void update_temps();
void emergency( int source );
void drill();
void basic_drill();

#endif /* HUB_H */
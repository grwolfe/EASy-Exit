#include "mbed.h"
#define BAUDRATE 115200

Serial pc( USBTX, USBRX, BAUDRATE );
void echo(void)
{
    // echo whatever you get
    char command = pc.getc();
    pc.printf( "Received: %c\r\n", command );
}

int main()
{
    pc.printf( "Here...\r\n" );
    pc.attach( &echo );  // setup RX interrupt handler
    while (1);
}
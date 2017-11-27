#include "mbed.h"
Serial xbee( p13, p14, 9600 );   // connection to XBee / Nodes
Serial pc( USBTX, USBRX, 9600 );
void XB_RX_ISR()
{
    pc.printf("%c",xbee.getc());
}

int main()
{ 
    xbee.attach(&XB_RX_ISR);
    for( int i = 0; i < 6; i++ )
    {
        pc.printf("\r\nSending update request...\r\n");
        xbee.printf("n%d,0\n", i);
        wait(5);
        pc.printf("\r\nSending red on command...\r\n");
        xbee.printf("n%d,2\n", i);
        wait(5);
        pc.printf("\r\nSending off command...\r\n");
        xbee.printf("n%d,1\n", i);
        wait(5);
        pc.printf("\r\nSending green on command...\r\n");
        xbee.printf("n%d,3\n", i);
        wait(5);
        pc.printf("\r\nSending off command...\r\n");
        xbee.printf("n%d,1\n", i);
        wait(5);
    }
    pc.printf("\r\nterminating...\r\n");
    while(1);
}
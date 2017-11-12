#include "hub.h"

int main()
{
    init();
    while (1)
    {
        // do stuff
    }

    return 0; /* UNREACHABLE */
}

void init()
{
    pc.printf("%s\n", "Initializing system...");
    memset( pc_buff, 0, sizeof(pc_buff) );
    memset( xb_buff, 0, sizeof(xb_buff) );
    xbee.attach( &XB_RX_ISR );
    pc.attach( &PC_RX_ISR );
    timer.attach( &update_gui, PC_UPRATE );
}

void PC_RX_ISR()
{
    int i = 0;
    char byte;
    do
    {
        byte = pc.getc();
        pc_buff[i++] = byte;
    } while( byte != '\n' && i < PC_BUFFSIZE );
    pc_msg = READY;
}

void XB_RX_ISR()
{
    int i = 0;
    char byte;
    do
    {
        byte = xbee.getc();
        xb_buff[i++] = byte;
    } while( byte != '\n' && i < XB_BUFFSIZE );
    xb_msg = READY;
}

void update_gui()
{
    // send an update to the PC
}

void reset()
{
    pc.printf( "Resetting system...\n" );
    NVIC_SystemReset();
}
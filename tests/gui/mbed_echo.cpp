#include "mbed.h"
#define BAUDRATE 115200
#define PC_BUFFER_SIZE 256

Serial pc( USBTX, USBRX, BAUDRATE );
char pc_buff[PC_BUFFER_SIZE];
volatile bool pc_msg = false;  // valid message recieved from PC

// ISR to read Serial message from the PC bus
void echo(void)
{
    // echo whatever you get
    int i = 0;
    char p;
    do
    {
        p = pc.getc();
        pc_buff[i++] = p;
    }
    while ( p != '\n' && i < PC_BUFFER_SIZE );
    pc_msg = true;
}

int main()
{
    pc.attach( &echo );  // setup RX interrupt handler
    while (1)
    {
        if (msg)
        {
            pc.printf("%s\r\n", pc_buff);
            msg = false;
            memset(pc_buff, 0, sizeof(pc_buffer));  // clear the buffer
        }
    }
}
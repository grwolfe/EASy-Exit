#include "hub.h"

int main()
{
    init();
    
    pc.printf("Sending RED command to all nodes...\r\n");
    command_all( RED );
    wait(1);
    
    pc.printf("Sending GRN command to all nodes...\r\n");
    command_all( GRN );
    wait(1);
    
    pc.printf("Sending OFF command to all nodes...\r\n");
    command_all( OFF );
    wait(1);
    
    while(1)
    {
        if( pc_rdy == READY )
            process_command();
    }
}

void init()
{
    pc.printf("Initializing system...\r\n");
    memset( pc_buff, 0, sizeof(pc_buff) );
    pc.attach( &PC_RX_ISR );
}

void command_all( const int c )
{
    for( int i = 0; i < NUM_NODES; i++ )
        nodes[i].command( &xbee, c );
}

void process_command()
{
    if( strcmp( pc_buff, "update") == 0 )
        update_gui();
    else if( strcmp( pc_buff, "quit" ) == 0 )
        quit();
    else if( strcmp( pc_buff, "reset" ) == 0 )
        reset();
    else
        pc.printf("Echo: %s\r\n", pc_buff);

    pc_rdy = NOT_READY;
    memset( pc_buff, 0, sizeof(pc_buff) );
}

void quit()
{
    pc.printf("Termination command: \"%s\" received.\r\nShutting down...\r\n", pc_buff);
    memset( pc_buff, 0, sizeof(pc_buff) );
    while(true)
    {
        if( pc_rdy )
        {
            if( strcmp(pc_buff, "reset") == 0 )
                reset();
            pc_rdy = NOT_READY;
            memset( pc_buff, 0, sizeof(pc_buff) );
        }
        wait(0.5);
    }
}

void reset()
{
    pc.printf( "\r\nResetting system...\r\n" );
    wait(0.5);
    NVIC_SystemReset();
}

void update_gui()
{
    for( int i = 0; i < NUM_NODES; i++ )
    {
        printf("Node %d:\r\nT0 - %d: %.2f\r\nT1 - %d: %.2f\r\n", i,
            nodes[i].getStatus(0), nodes[i].getTemp(0),
            nodes[i].getStatus(1), nodes[i].getTemp(1)
        );
        pc.printf("\r\n");
    }
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
    pc_buff[i - 1] = '\0'; // replace newline character with NULL terminator
    pc_rdy = READY;
}
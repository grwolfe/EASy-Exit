#include "hub.h"

int main()
{
    init();    
    while( true )
    {
        if( pc_rdy == READY )
            process_command();
        
        for( int i = 0; i < NUM_NODES; i++ )
        {
            nodes[i].setTemp( &xbee );
            if( nodes[i].emergency() )
                emergency(i);
        }

        update_gui();
    }
}

void init()
{
    pc.printf("Initializing system...\r\n");
    memset( pc_buff, 0, sizeof(pc_buff) );
    pc.attach( &PC_RX_ISR );
    
    command_all( RED );
    wait(1);
    command_all( GRN );
    wait(1);
    command_all( OFF );
    wait(1);
}

void command_all( const int c )
{
    for( int i = 0; i < NUM_NODES; i++ )
        nodes[i].command( &xbee, c );
}

void emergency( int source )
{
    pc.printf("Emergency detected at node %d!!\r\n", source);

    command_all( GRN );
    nodes[source].command( &xbee, RED );

    // remain in state of emergency until reset
    while( true )
    {
        for( int i = 0; i < NUM_NODES; i++ )
        {
            nodes[i].setTemp( &xbee );
            if( nodes[i].emergency() )
                nodes[i].command( &xbee, RED );
        }
        update_gui();
        wait(0.5);
    }
}

void drill()
{
    pc.printf("Executing a drill..\r\n");
    nodes[0].command( &xbee, RED );
    while( true )
    {
        for( int i = 1; i < NUM_NODES; i++ )
        {
            nodes[i].command( &xbee, DRILL );
            wait(0.3);
        }
        wait(0.5);

        for( int i = 1; i < NUM_NODES; i++ )
        {
            nodes[i].command( &xbee, OFF );
            wait(0.3);
        }
        wait(1.5);
    }
}

void slow_drill()
{
    pc.printf("Executing a drill...\r\n");
    nodes[0].command( &xbee, RED );
    while( true )
    {
        for( int i = 1; i < NUM_NODES; i++ )
        {
            nodes[i].command( &xbee, DRILL );
            wait(1.5);
        }
        for( int i = 1; i < NUM_NODES; i++ )
        {
            nodes[i].command( &xbee, OFF );
        }
        wait(2);
    }
}

void process_command()
{
    if( strcmp( pc_buff, "update") == 0 )
        update_gui();
    else if( strcmp( pc_buff, "drill") == 0 )
        drill();
    else if( strcmp( pc_buff, "quit" ) == 0 )
        quit();
    else if( strcmp( pc_buff, "reset" ) == 0 )
        reset();
    else
        pc.printf("Unknown: %s\r\n", pc_buff);

    pc_rdy = NOT_READY;
    memset( pc_buff, 0, sizeof(pc_buff) );
}

void quit()
{
    pc.printf("Termination command: \"%s\" received.\r\nShutting down...\r\n", pc_buff);
    memset( pc_buff, 0, sizeof(pc_buff) );
    while( true );
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
    
    if( strcmp(pc_buff, "reset") == 0 )
        reset();
}
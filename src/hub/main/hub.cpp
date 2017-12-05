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

            /* IF THE NODE IS TRIGGERED AND HASN'T BEEN HANDLED YET */
            if( nodes[i].emergency() && emergencies[i] == false )
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
    
    for( int i = 0; i < NUM_NODES; i++ )
        emergencies[i] = false;
    
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
    nodes[source].command( &xbee, RED );
    pc.printf("Emergency detected at node %d!!\r\n", source);

    /* UPDATE THE CONSISTENT DATA STRUCTURE */
    emergencies[source] = true;

    /* BASED ON CONSISTENT DATA STRUCTURE OF STATE COMMAND NODES */
    for( int i = 0; i < NUM_NODES; i++ )
    {
        if( emergencies[i] )
            continue;

        // can assume if this block is reached than the current node is safe
        switch( i )
        {
            case 0:
                nodes[i].command( &xbee, LEFT );
                break;
            case 1:
                if( emergencies[3] == false )
                    nodes[i].command( &xbee, RIGHT );
                else
                    nodes[i].command( &xbee, UP );
                break;
            case 2:
                if( emergencies[0] == false )
                    nodes[i].command( &xbee, LEFT );
                else if( emergencies[3] == false )
                    nodes[i].command( &xbee, DOWN );
                else
                    nodes[i].command( &xbee, RIGHT );
                break;
            case 3:
                nodes[i].command( &xbee, LEFT );
                break;
            case 4:
                if( emergencies[5] == false )
                    nodes[i].command( &xbee, DOWN );
                else
                    nodes[i].command( &xbee, LEFT );
                break;
            case 5:
                nodes[i].command( &xbee, RIGHT );
        }
    }
}

void basic_drill()
{
    pc.printf("Executing a drill..\r\n");
    command_all( OFF );
    wait(0.5);

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

void process_command()
{
    if( strcmp( pc_buff, "update") == 0 )
        update_gui();
    else if( strcmp( pc_buff, "drill") == 0 )
        basic_drill();
    else if( strcmp( pc_buff, "quit" ) == 0 )
        quit();
    else if( strncmp( pc_buff, "nodes", 5 ) == 0 )
    {
        // grab the target node from the end of the command and execute
        int target = atoi( &pc_buff[strlen(pc_buff) - 1] );
        if( target < 6 && target >= 0 )
            emergency( target );
    }
    else
        pc.printf("Unknown: %s\r\n", pc_buff);

    pc_rdy = NOT_READY;
    memset( pc_buff, 0, sizeof(pc_buff) );
}

void quit()
{
    pc.printf("Termination command: \"%s\" received.\r\nShutting down...\r\n", pc_buff);
    memset( pc_buff, 0, sizeof(pc_buff) );
    while( true )
    {
        /* IGNORE ANY COMMANDS, BUT CLEAR BUFFER */
        if( pc_rdy )
            memset( pc_buff, 0, sizeof(pc_buff) );
    }
}

void reset()
{
    pc.printf( "\r\nResetting system...\r\n" );
    wait(0.2);
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
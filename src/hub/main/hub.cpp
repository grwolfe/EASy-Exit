#include "hub.h"

int main()
{
    init();
    nodes[0].setTemp( &xbee );
    nodes[0].command( &xbee, 2);
    while(1)
    {
        pc.printf("N%d\r\nT0Status: %d\tTemp: %f\r\nT1Status: %d\tTemp: %f\r\n", 
            nodes[0].getAddr(), 
            nodes[0].getStatus(0), nodes[0].getTemp(0),
            nodes[0].getStatus(1), nodes[0].getTemp(1));
        wait(2);
    }
    // while (1)
    // {
    //     while( emergency )
    //     {
    //         // tell all nodes to get it
    //         pc.printf("Emergency!\r\n");
    //         for( int i = 0; i < 6; i++ )
    //         {
    //             if( nodes[i].getTemp(0) > 80 || nodes[i].getTemp(1) > 80 )
    //             {
    //                 xbee.printf("n%d,2\n", nodes[i].getAddr());
    //             } else {
    //                 xbee.printf("n%d,3\n", nodes[i].getAddr());
    //             }
    //         }
    //         wait(2);
    //     }
    //     // get updates from Nodes
    //     for( int i = 0; i < NUM_NODES; i++ )
    //     {
    //         xbee.printf("n%d,0\n", nodes[i].getAddr());
    //         while( xb_msg == NOT_READY );   //wait for reply
    //         pc.printf("%s\r\n", xb_buff);
            
    //         char *token = strtok( xb_buff, "," );
    //         token = strtok( NULL, "," );    // t0 status
    //         token = strtok( NULL, "," );    // t0
    //         float temp0 = atof(token);   // convert t0 to float                
    //         token = strtok(NULL, ",");  // t1 status
    //         token = strtok( NULL, "," );    // t1
    //         float temp1 = atof(token);

    //         nodes[i].setTemp(0, temp0);
    //         nodes[i].setTemp(1, temp1);

    //         if( temp0 > 80 || temp1 > 80 )
    //             emergency = true;
            
    //         memset( xb_buff, 0, sizeof(xb_buff) );
    //         wait(2);
    //     }
    //     wait(1);
    // }
}

void init()
{
    pc.printf("%s\r\n", "Initializing system...");
    memset( pc_buff, 0, sizeof(pc_buff) );
    // memset( xb_buff, 0, sizeof(xb_buff) );
    // xbee.attach( &XB_RX_ISR );
    pc.attach( &PC_RX_ISR );
    // timer.attach( &update_gui, PC_UPRATE );
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

// void XB_RX_ISR()
// {
//     int i = 0;
//     char byte;
//     do
//     {
//         byte = xbee.getc();
//         xb_buff[i++] = byte;
//     } while( byte != '\n' && i < XB_BUFFSIZE );
//     xb_msg = READY;
// }

void update_gui()
{
    // send an update to the PC
    for( int i = 0; i < NUM_NODES; i++ )
        pc.printf( "N%d: %f %f\r\n",
            i, nodes[i].getTemp(0), nodes[i].getTemp(1) );
}

void reset()
{
    pc.printf( "Resetting system...\n" );
    NVIC_SystemReset();
}
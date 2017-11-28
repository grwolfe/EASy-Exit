#include "Node.h"

Node::Node( const int address ): addr(address)
{
    this->pattern = OFF;
}

Node::~Node()
{
    // destructor
}

int Node::getAddr() const
{ return this->addr; }

float Node::getTemp( const int num ) const
{ return this->temp[num]; }

int Node::getStatus( const int num ) const
{ return this->stats[num]; }

int Node::getPattern() const
{ return this->pattern; }

void Node::command( Serial *xbee, const int c )
{
    // if trying to send a 0 then perform update request
    if( c == 0 )
        setTemp( xbee );
    else
    {
        xbee->printf("n%d,%d\n", this->addr, c); // send the command to the node
        this->pattern = c;                      // update the pattern for this node object
    }
}

void Node::setTemp( Serial *xbee )
{
    // declare and initialize variables
    int i = 0;
    char byte;
    char buffer[25], s_addr[5];
    memset( buffer, 0, sizeof(buffer) );

    // send command and receive response
    xbee->printf( "n%d,0\n", this->addr );
    while( !xbee->readable() );  // wait for response
    do
    {
        if( xbee->readable() )
        {
            byte = xbee->getc();
            buffer[i++] = byte;
        }
    }
    while( byte != '\n' && byte != '\r' && i < 25 );

    // parse the response and update class fields
    // RESPONSE: <NODE_ADDR>,<T0Status>,<T0>,<T1Status>,<T1>\n
    sprintf( s_addr, "n%d", this->addr );
    if( strcmp( s_addr, strtok(buffer, ",") ) != 0 )
    {
        // error not the correct node responded
    } else {
        this->stats[0] = atoi( strtok( NULL, "," ) );
        this->temp[0]  = atof( strtok( NULL, ",") );

        this->stats[1] = atoi( strtok( NULL, "," ) );
        this->temp[1]  = atof( strtok( NULL, "," ) );        
    }
}

void Node::setStatus( const int num, const int status )
{ this->stats[num] = status; }

void Node::setPattern( const int pattern )
{ this->pattern = pattern; }
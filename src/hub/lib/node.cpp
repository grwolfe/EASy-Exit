#include "Node.h"

Node::Node( const int address )
{
    // constructor
}

Node::~Node()
{
    // destructor
}

int Node::getAddr()
{ return addr; }

float Node::getTemp( const int num ) const
{ return temp[num]; }

void Node::setTemp( const int num, const float temp )
{ temp[num] = temp; }

int Node::getStatus( const int num ) const
{ return stats[num]; }

void Node::setStatus( const int num, const int status )
{ stats[num] = status; }

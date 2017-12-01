#ifndef NODE_H
#define NODE_H

#include "mbed.h"
#include <stdlib.h>

#define THRESHOLD 80.0

class Node
{
private:
    const int addr;
    int
        stats[2],
        pattern;
    float
        temp[2];
public:
    Node( const int address );
    int getAddr() const;
    float getTemp( const int num ) const;
    int getStatus( const int num ) const;
    int getPattern() const;
    int emergency() const;
    int setTemp( Serial *xbee );
    void setStatus( const int num, const int status );
    void setPattern( const int pattern );
    void command( Serial *xbee, const int c );
};

#endif /* NODE_H */
#ifndef NODE_H
#define NODE_H

class Node
{
private:
    int
        addr,
        stats[2],
        pattern;
    float
        temp[2];
public:
    Node( const int address );
    ~Node();
    int getAddr() const;
    float getTemp( const int num ) const;
    int getStatus( const int num ) const;
    void setTemp(  const int num, const float temp );
    void setStatus( const int num, const int status );
};

#endif /* NODE_H */

#ifndef CONNECT_H
#define CONNECT_H

#include <vector>

class Element;
class Line;
class Wire;



class Connect
{
public:
    Connect();
    virtual ~Connect();

    void Trace( Line* line );

    int GetValue();
    void SetValue( const int value );

    void AddElement( Element* element );
    void RemoveElement( Element* element );
    void ClearElements();

    bool IsEmpty() const;
    void ClearAll();

    void AddWire( Wire* wire );
    void RemoveWire( Wire* wire );
    std::vector< Wire* > GetWires();

    void Merge( Connect* connect );

private:
    std::vector< Element* > m_Elements;
    int m_Value;
    std::vector< Wire* > m_Wires;
};



#endif // CONNECT_H

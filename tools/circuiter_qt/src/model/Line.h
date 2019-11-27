#ifndef LINE_H
#define LINE_H

#include <vector>

class Connect;
class Element;



class Line
{
public:
    Line();
    virtual ~Line();

    void SetValue( const int value );

    void AddConnect( Connect* connect );
    void UpdateConnects();

private:
    int m_Value;

    std::vector< Connect* > m_AffectedConnect;
};



#endif // ELEMENT_H

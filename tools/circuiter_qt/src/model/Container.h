#ifndef CONTAINER_H
#define CONTAINER_H

#include "Element.h"

#include <vector>

class Connect;
class Line;
class Wire;



class Container : public Element
{
public:
    Container( Container* parent );
    virtual ~Container();

    Element* Copy();

    QRectF boundingRect() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    void Update();
    void Trace( Line* line, Connect* connect );
    void SetValue( const int value, Connect* connect );

    std::vector< Element* >& GetElements();
    std::vector< Wire* >& GetWires();

    void InsertElement( Element* element );
    void RemoveElement( Element* element );
    void InsertConnect( Connect* connect );
    void RemoveConnect( Connect* connect );
    void InsertWire( Wire* wire );
    void RemoveWire( Wire* wire );

private:
    std::vector< Element* > m_Elements;
    std::vector< Connect* > m_Connects;
    std::vector< Wire* > m_Wires;
};



#endif // CONTAINER_H

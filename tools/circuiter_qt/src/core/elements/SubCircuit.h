#ifndef SUB_CIRCUIT_H
#define SUB_CIRCUIT_H

#include "../Element.h"

class Connect;
class Line;



class SubCircuit : public Element
{
public:
    SubCircuit();
    virtual ~SubCircuit();

    Element* Copy();

    QRectF boundingRect() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    void Update();
    void Trace( Line* line, Connect* connect );
    void SetValue( const int value, Connect* connect );

protected:
    std::vector< Element* > m_Elements;
    std::vector< Element* > m_UpdateCurrent;
    std::vector< Element* > m_UpdateNext;
    std::vector< Connect* > m_Connects;
    std::vector< Wire* > m_Wires;
};



#endif // SUB_CIRCUIT_H

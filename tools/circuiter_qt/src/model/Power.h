#ifndef POWER_H
#define POWER_H

#include "Element.h"

class Connect;
class Container;



class Power : public Element
{
public:
    Power( Container* parent );
    virtual ~Power();

    Element* Copy( Container* parent );

    QRectF boundingRect() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    void Update();
    void Trace( Line* line, Connect* connect );
    void SetValue( const int value, Connect* connect );

private:
    int m_Value;
};



#endif // POWER_H

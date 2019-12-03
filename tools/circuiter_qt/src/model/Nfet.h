#ifndef NFET_H
#define NFET_H

#include "Element.h"

class Connect;
class Container;
class Line;



class Nfet : public Element
{
public:
    Nfet( Container* parent );
    virtual ~Nfet();

    Element* Copy();

    QRectF boundingRect() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    void Update();
    void Trace( Line* line, Connect* connect );
    void SetValue( const int value, Connect* connect );

protected:
    int m_Switch;
};



#endif // NFET_H

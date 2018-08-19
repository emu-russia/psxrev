#ifndef PFET_H
#define PFET_H

#include "../Element.h"

class Connect;
class Line;



class Pfet : public Element
{
public:
    Pfet();
    virtual ~Pfet();

    Element* Copy();

    QRectF boundingRect() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    void Update();
    void Trace( Line* line, Connect* connect );
    void SetValue( const int value, Connect* connect );

protected:
    int m_Switch;
};



#endif // PFET_H

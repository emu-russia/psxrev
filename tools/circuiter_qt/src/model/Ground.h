#ifndef GROUND_H
#define GROUND_H

#include "Element.h"

class Connect;
class Container;


class Ground : public Element
{
public:
    Ground( Container* parent );
    virtual ~Ground();

    Element* Copy();

    QRectF boundingRect() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    void Update();
    void Trace( Line* line, Connect* connect );
    void SetValue( const int value, Connect* connect );

private:
    int m_Value;
};



#endif // GROUND_H

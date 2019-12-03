#ifndef PIN_H
#define PIN_H

#include "Element.h"

class Connect;
class Container;



class Pin : public Element
{
public:
    Pin( Container* parent );
    virtual ~Pin();

    Element* Copy( Container* parent );

    QRectF boundingRect() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    void Update();
    void Trace( Line* line, Connect* connect );
    void SetValue( const int value, Connect* connect );

    void UserSetValue( const int value );

protected:
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) override;

private:
    int m_Value;
};



#endif // PIN_H

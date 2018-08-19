#ifndef WIRE_H
#define WIRE_H

#include <vector>

#include "GraphicsItem.h"

class Connect;



class Wire : public GraphicsItem
{
public:
    Wire();
    virtual ~Wire();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    int GetValue();
    void SetValue( const int value );

    void SetLine( const QLine& line );
    const QLine& GetLine();

    void AddConnectPoint( const int num );

    void SetConnect( Connect* connect );
    Connect* GetConnect();

private:
    Connect* m_Connect;
    int m_Value;
    QPolygonF m_DrawPoly;
    QLine m_Line;
    bool m_ConnectPoint1;
    bool m_ConnectPoint2;
};



#endif // WIRE_H

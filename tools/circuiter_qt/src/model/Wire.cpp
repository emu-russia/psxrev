#include "Connect.h"
#include "../gui/GraphicsScene.h"
#include "Wire.h"

#include <QtWidgets>



Wire::Wire():
    m_Value( -1 ),
    m_Connect( 0 ),
    m_ConnectPoint1( false ),
    m_ConnectPoint2( false )
{
    m_ItemType = IT_WIRE;
    setZValue( 1 );
}



Wire::~Wire()
{
}



QRectF
Wire::boundingRect() const
{
    return m_DrawPoly.boundingRect();
}



QPainterPath
Wire::shape() const
{
    QPainterPath path;
    path.addPolygon( m_DrawPoly );
    return path;
}



void
Wire::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED( widget );
    if( m_Value == 1 )
    {
        painter->setPen( QPen( WIRE_HIGH_COLOR, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
        painter->setBrush( WIRE_HIGH_COLOR );
    }
    else if( m_Value == 0 )
    {
        painter->setPen( QPen( WIRE_LOW_COLOR, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
        painter->setBrush( WIRE_LOW_COLOR );
    }
    else
    {
        painter->setPen( QPen( WIRE_Z_COLOR, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
        painter->setBrush( WIRE_Z_COLOR );
    }

    painter->drawLine( m_Line );

    if( m_ConnectPoint1 == true )
    {
        QPoint pos = m_Line.p1();
        painter->drawEllipse( pos.x() - 3, pos.y() - 3, 6, 6 );
    }
    if( m_ConnectPoint2 == true )
    {
        QPoint pos = m_Line.p2();
        painter->drawEllipse( pos.x() - 3, pos.y() - 3, 6, 6 );
    }

    if( option->state & QStyle::State_Selected )
    {
        painter->setPen( QPen( Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
        painter->setBrush( SELECT_COLOR );
        painter->drawPolygon( m_DrawPoly );
    }
}



int
Wire::GetValue()
{
    return m_Value;
}



void
Wire::SetValue( const int value )
{
    m_Value = value;
}



void
Wire::SetLine( const QLine& line )
{
    m_Line = line;

    qreal length = QLineF( line ).length();
    QPolygonF poly = QPolygonF( QRectF( -3, -3, 6, length + 6 ) );
    qreal angle = QLineF( line ).angle();

    QTransform transform;
    transform.translate( line.x1(), line.y1() );
    transform.rotate( 270 - angle );
    m_DrawPoly = transform.map( poly );
}



const QLine&
Wire::GetLine()
{
    return m_Line;
}



void
Wire::AddConnectPoint( const int num )
{
    if( num == 1 )
    {
        m_ConnectPoint1 = true;
    }
    else if( num == 2 )
    {
        m_ConnectPoint2 = true;
    }
}



void
Wire::SetConnect( Connect* connect )
{
    m_Connect = connect;
}



Connect*
Wire::GetConnect()
{
    return m_Connect;
}

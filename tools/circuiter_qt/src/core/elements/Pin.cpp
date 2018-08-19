#include "Pin.h"

#include "../Circuit.h"
#include "../Connect.h"
#include "../Line.h"

#include <QtWidgets>



Pin::Pin():
    m_Value( -1 )
{
    m_ItemType = IT_PIN;

    Contact con;
    con.point = QPoint( 0, 0 );
    con.connect = 0;
    m_Contacts.push_back( con );
}



Pin::~Pin()
{
}



Element*
Pin::Copy()
{
    Element* element = new Pin();
    return element;
}



QRectF
Pin::boundingRect() const
{
    return QRectF( -33, -18, 36, 36 );
}



void
Pin::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED( widget );
    painter->setPen( QPen( Qt::black, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    painter->setBrush( Qt::NoBrush );
    painter->drawRect( -30, -15, 30, 30 );

    if( m_Value == 1 )
    {
        painter->setPen( QPen( WIRE_HIGH_COLOR ) );
        painter->setBrush( WIRE_HIGH_COLOR );
    }
    else if( m_Value == 0 )
    {
        painter->setPen( QPen( WIRE_LOW_COLOR ) );
        painter->setBrush( WIRE_LOW_COLOR );
    }
    else
    {
        painter->setPen( QPen( WIRE_Z_COLOR ) );
        painter->setBrush( WIRE_Z_COLOR );
    }
    painter->drawEllipse( -24, -9, 18, 18 );

    painter->setPen( QPen( CONNECT_COLOR ) );
    painter->setBrush( CONNECT_COLOR );
    painter->drawEllipse( -3, -3, 6, 6 );

    if( option->state & QStyle::State_Selected )
    {
        painter->setPen( QPen( Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
        painter->setBrush( SELECT_COLOR );
        painter->drawRect( boundingRect() );
    }
}



void
Pin::Update()
{
    if( m_Contacts[ 0 ].connect != 0 )
    {
        Line* line = new Line();
        line->SetValue( m_Value );
        m_Visited = true;
        m_Contacts[ 0 ].connect->Trace( line );
        line->UpdateConnects();
        delete line;
    }
}



void
Pin::Trace( Line* line, Connect* connect )
{
    line->SetValue( m_Value );
    m_Visited = true;
}



void
Pin::SetValue( const int value, Connect* connect )
{
    m_Visited = false;
}



void
Pin::UserSetValue( const int value )
{
    if( m_Value != value )
    {
        m_Circuit->AddToUpdate( this );
        m_Value = value;
    }
}




void
Pin::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
    if( event->button() == Qt::LeftButton )
    {
        if( isSelected() == false )
        {
            UserSetValue( ( m_Value == 1 ) ? 0 : 1 );
            update();
        }
    }
}

#include "Power.h"

#include "../Circuit.h"
#include "../Connect.h"
#include "../Line.h"

#include <QtWidgets>



Power::Power():
    m_Value( 1 )
{
    m_ItemType = IT_POWER;

    Contact con;
    con.point = QPoint( 0, 0 );
    con.connect = 0;
    m_Contacts.push_back( con );

}



Power::~Power()
{
}



Element*
Power::Copy()
{
    Element* element = new Power();
    return element;
}



QRectF
Power::boundingRect() const
{
    return QRectF( -14, -13, 28, 19 );
}



void
Power::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED( widget );
    painter->setPen( QPen( Qt::black, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    painter->setBrush( Qt::NoBrush );
    painter->drawLine( 0, 0, 0, -5 );
    painter->drawLine( 0, -9, -10, 2 );
    painter->drawLine( 0, -9, 10, 2 );
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
Power::Update()
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
Power::Trace( Line* line, Connect* connect )
{
    line->SetValue( m_Value );
    m_Visited = true;
}



void
Power::SetValue( const int value, Connect* connect )
{
    m_Visited = false;
}

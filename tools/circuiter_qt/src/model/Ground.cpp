#include "Ground.h"

#include "../gui/GraphicsScene.h"
#include "Connect.h"
#include "Line.h"

#include <QtWidgets>



Ground::Ground( Container* parent ):
    Element( parent ),
    m_Value( 0 )
{
    m_ItemType = IT_GROUND;

    Contact con;
    con.point = QPoint( 0, 0 );
    con.connect = 0;
    m_Contacts.push_back( con );

}



Ground::~Ground()
{
}



Element*
Ground::Copy()
{
    Element* element = new Ground( GetContainer() );
    return element;
}



QRectF
Ground::boundingRect() const
{
    return QRectF( -14, -4, 28, 8 );
}



void
Ground::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED( widget );
    painter->setPen( QPen( Qt::black, 6, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    painter->setBrush( Qt::NoBrush );
    painter->drawLine( -10, 0, 10, 0 );
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
Ground::Update()
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
Ground::Trace( Line* line, Connect* connect )
{
    line->SetValue( m_Value );
    m_Visited = true;
}



void
Ground::SetValue( const int value, Connect* connect )
{
    m_Visited = false;
}

#include "Pfet.h"

#include "../gui/GraphicsScene.h"
#include "Connect.h"
#include "Line.h"

#include <QtWidgets>



Pfet::Pfet( Container* parent ):
    Element( parent ),
    m_Switch( -1 )
{
    m_ItemType = IT_PFET;

    Contact con;
    con.point = QPoint( -15, 15 );
    con.connect = 0;
    m_Contacts.push_back( con );
    con.point = QPoint( 15, 15 );
    m_Contacts.push_back( con );
    con.point = QPoint( 0, 0 );
    m_Contacts.push_back( con );
}



Pfet::~Pfet()
{
}



Element*
Pfet::Copy( Container* parent )
{
    Element* element = new Pfet( parent );
    return element;
}



QRectF
Pfet::boundingRect() const
{
    return QRectF( -18, -3, 36, 21 );
}



void
Pfet::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED( widget );
    painter->setPen( QPen( Qt::black, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    painter->setBrush( Qt::white );
    painter->drawLine( -15, 7, 15, 7 );
    painter->drawLine( -10, 7, -10, 15 );
    painter->drawLine( 10, 7, 10, 15 );
    painter->drawLine( -15, 15, -10, 15 );
    painter->drawLine( 10, 15, 15, 15 );
    painter->setPen( QPen( Qt::black, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    painter->drawEllipse( -3, 2, 6, 6 );
    painter->setPen( QPen( CONNECT_COLOR ) );
    painter->setBrush( CONNECT_COLOR );
    painter->drawEllipse( -3, -3, 6, 6 );
    painter->drawEllipse( -18, 12, 6, 6 );
    painter->drawEllipse( 12, 12, 6, 6 );

    painter->setPen( QPen( Qt::gray, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    if( m_Switch == 0 )
    {
        painter->drawLine( -10, 22, 10, 22 );
    }
    else
    {
        painter->drawLine( -10, 22, -5, 22 );
        painter->drawLine( -5, 22, 5, 18 );
        painter->drawLine( 5, 22, 10, 22 );
    }


    if( option->state & QStyle::State_Selected )
    {
        painter->setPen( QPen( Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
        painter->setBrush( QColor( 0, 0, 255, 100 ) );
        painter->drawRect( boundingRect() );
    }
}



void
Pfet::Update()
{
    if( ( m_Contacts[ 0 ].connect != 0 ) && ( m_Contacts[ 1 ].connect != 0 ) )
    {
        if( m_Switch == 0 )
        {
            Line* line = new Line();
            m_Visited = true;
            m_Contacts[ 0 ].connect->Trace( line );
            m_Contacts[ 1 ].connect->Trace( line );
            line->UpdateConnects();
            delete line;
        }
        else
        {
            Line* line = new Line();
            m_Visited = true;
            m_Contacts[ 0 ].connect->Trace( line );
            line->UpdateConnects();
            delete line;

            line = new Line();
            m_Visited = true;
            m_Contacts[ 1 ].connect->Trace( line );
            line->UpdateConnects();
            delete line;
        }
    }

    if( ( m_Switch == 1 ) && ( m_Contacts[ 2 ].connect != 0 ) )
    {
        Line* line = new Line();
        line->SetValue( m_Switch );
        m_Visited = true;
        m_Contacts[ 2 ].connect->Trace( line );
        line->UpdateConnects();
        delete line;
    }
}



void
Pfet::Trace( Line* line, Connect* connect )
{
    m_Visited = true;

    if( connect == m_Contacts[ 2 ].connect )
    {
        if( m_Switch == 1 )
        {
            line->SetValue( 1 );
        }
    }
    else if( m_Switch == 0 )
    {
        if( ( connect == m_Contacts[ 0 ].connect ) && ( m_Contacts[ 1 ].connect != 0 ) )
        {
            m_Contacts[ 1 ].connect->Trace( line );
        }
        else if( ( connect == m_Contacts[ 1 ].connect ) && ( m_Contacts[ 0 ].connect != 0 ) )
        {
            m_Contacts[ 0 ].connect->Trace( line );
        }
    }
}



void
Pfet::SetValue( const int value, Connect* connect )
{
    m_Visited = false;

    if( m_Contacts[ 2 ].connect == connect )
    {
        if( value != -1 )
        {
            if( m_Switch != value )
            {
                m_Switch = value;
            }
        }
    }
}

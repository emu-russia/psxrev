#include "Container.h"

#include "../gui/GraphicsScene.h"

#include <QtWidgets>



Container::Container( Container* parent, const QString& def ):
    Element( parent ),
    m_Def( def )
{
    m_ItemType = IT_CONTAINER;

    Contact con;
    con.connect = 0;
    con.point = QPoint( -15, -45 );
    m_Contacts.push_back( con );
    con.point = QPoint( 15, -45 );
    m_Contacts.push_back( con );
    con.point = QPoint( 0, 45 );
    m_Contacts.push_back( con );
}



Container::~Container()
{
    for( size_t i = 0; i < m_Elements.size(); ++i )
    {
        delete m_Elements[ i ];
    }

    for( size_t i = 0; i < m_Connects.size(); ++i )
    {
        delete m_Connects[ i ];
    }

    for( size_t i = 0; i < m_Wires.size(); ++i )
    {
        delete m_Wires[ i ];
    }
}



Element*
Container::Copy( Container* parent )
{
    Element* element = new Container( parent, m_Def );
    return element;
}



QRectF
Container::boundingRect() const
{
    return QRectF( -50, -50, 100, 100 );
}



void
Container::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED( widget )
    painter->setPen( QPen( Qt::black, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    painter->setBrush( QColor( 219, 214, 121, 255 ) );
    painter->drawRect( QRectF( -45, -45, 90, 90 ) );

    // draw connects
    painter->setPen( QPen( CONNECT_COLOR ) );
    painter->setBrush( CONNECT_COLOR );
    for( size_t i = 0; i < m_Contacts.size(); ++i )
    {
        painter->drawEllipse( m_Contacts[ i ].point.x() - 3, m_Contacts[ i ].point.y() - 3, 6, 6 );
    }

    QFont font( "Times", 10 );
    font.setStyleStrategy( QFont::ForceOutline );
    painter->setFont( font );
    painter->save();
    painter->scale( 1, 1 );
    painter->drawText( -20, 0, m_Def );
    painter->restore();

    if( option->state & QStyle::State_Selected )
    {
        painter->setPen( QPen( Qt::black, 0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
        painter->setBrush( QColor( 0, 0, 255, 100 ) );
        painter->drawRect( boundingRect() );
    }
}



void
Container::Update()
{
    for( size_t i = 0; i < m_Elements.size(); ++i )
    {
        m_Elements[ i ]->Update();
    }
}



void
Container::Trace( Line* line, Connect* connect )
{
    m_Visited = true;
}



void
Container::SetValue( const int value, Connect* connect )
{
    m_Visited = false;
}



QString
Container::GetDef() const
{
    return m_Def;
}



std::vector< Element* >&
Container::GetElements()
{
    return m_Elements;
}



std::vector< Wire* >&
Container::GetWires()
{
    return m_Wires;
}



void
Container::InsertElement( Element* element )
{
    m_Elements.push_back( element );
}



void
Container::RemoveElement( Element* element )
{
    m_Elements.erase( std::remove( m_Elements.begin(), m_Elements.end(), element ), m_Elements.end() );
}



void
Container::InsertConnect( Connect* connect )
{
    m_Connects.push_back( connect );
}



void
Container::RemoveConnect( Connect* connect )
{
    m_Connects.erase( std::remove( m_Connects.begin(), m_Connects.end(), connect ), m_Connects.end() );
}



void
Container::InsertWire( Wire* wire )
{
    m_Wires.push_back( wire );
}



void
Container::RemoveWire( Wire* wire )
{
    m_Wires.erase( std::remove( m_Wires.begin(), m_Wires.end(), wire ), m_Wires.end() );
}

#include "Container.h"

#include "../gui/GraphicsScene.h"

#include <QtWidgets>



Container::Container( Container* parent ):
    Element( parent ),
    m_Scene( NULL )
{
    m_ItemType = IT_CONTAINER;

    counter = 0;

    Contact con;
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
}



Element*
Container::Copy()
{
    Element* element = new Container( GetContainer() );
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
    painter->drawText( 0, 0, QString("Counter: %1").arg( counter) );
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
    ++counter;

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



void
Container::SetScene( GraphicsScene* scene )
{
    m_Scene = scene;

    for( size_t i = 0; i < m_Elements.size(); ++i )
    {
        m_Scene->addItem( m_Elements[ i ] );
    }
}



void
Container::UnsetScene()
{
    for( size_t i = 0; i < m_Elements.size(); ++i )
    {
        m_Scene->removeItem( m_Elements[ i ] );
    }

    m_Scene = NULL;
}



void
Container::InsertContainer()
{
    Element* element = new Container( this );
    m_Elements.push_back( element );

    if( m_Scene != NULL )
    {
        m_Scene->addItem( element );
    }
}

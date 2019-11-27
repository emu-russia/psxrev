#include "Connect.h"

#include "Container.h"
#include "Element.h"

#include <QtWidgets>



Element::Element( Container* parent ):
    m_Container( parent ),
    m_Visited( false )
{
    setZValue( 2 );
}



Element::~Element()
{
}



bool
Element::IsVisited()
{
    return m_Visited;
}



void
Element::AddConnect( Connect* connect, const unsigned int cont_id )
{
    if( cont_id < m_Contacts.size() )
    {
        m_Contacts[ cont_id ].connect = connect;
        if( connect != 0 )
        {
            connect->AddElement( this );
        }
    }
}



void
Element::UpdateConnect( Connect* old_c, Connect* new_c )
{
    for( unsigned int i = 0; i < m_Contacts.size(); ++i )
    {
        if( m_Contacts[ i ].connect == old_c )
        {
            m_Contacts[ i ].connect = new_c;
        }
    }
}



void
Element::RemoveConnect( Connect* connect )
{
    if( connect != 0 )
    {
        for( unsigned int i = 0; i < m_Contacts.size(); ++i )
        {
            if( m_Contacts[ i ].connect == connect )
            {
                m_Contacts[ i ].connect = 0;
            }
        }
    }
}



void
Element::ClearConnects()
{
    for( unsigned int i = 0; i < m_Contacts.size(); ++i )
    {
        if( m_Contacts[ i ].connect != 0 )
        {
            m_Contacts[ i ].connect->RemoveElement( this );
            m_Contacts[ i ].connect = 0;
        }
    }
}



std::vector< Element::Contact >&
Element::GetContacts()
{
    return m_Contacts;
}



Container*
Element::GetContainer()
{
    return m_Container;
}



void
Element::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
    if( isSelected() == true )
    {
        event->accept();
    }
}



void
Element::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
}



void
Element::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
}

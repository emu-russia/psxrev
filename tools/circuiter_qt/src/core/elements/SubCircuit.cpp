#include "SubCircuit.h"

#include "../Circuit.h"
#include "../Connect.h"
#include "../Line.h"

#include <QtWidgets>



SubCircuit::SubCircuit()
{
    m_ItemType = IT_SUB_CURCUIT;
}



SubCircuit::~SubCircuit()
{
}



Element*
SubCircuit::Copy()
{
    SubCircuit* element = new SubCircuit();

    for( unsigned int i = 0; i < m_Elements.size(); ++i )
    {
        Element* el = m_Elements[ i ]->Copy();
        el->setPos( m_Elements[ i ]->pos() );
        el->setRotation( m_Elements[ i ]->rotation() );
        element->ConnectElement( el );
    }

    for( unsigned int i = 0; i < m_Wires.size(); ++i )
    {
        Wire* wire = new Wire();
        wire->SetLine( m_Wires[ i ]->GetLine() );
        wire->setPos( m_Wires[ i ]->pos() );
        element->ConnectWire( wire );
    }

    return ( Element* )element;
}



QRectF
SubCircuit::boundingRect() const
{
    return QRectF( -18, -3, 36, 21 );
}



void
SubCircuit::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED( widget );
}



void
SubCircuit::Update()
{
    int counter = 0;
    while( m_UpdateNext.size() > 0 )
    {
        m_UpdateCurrent = m_UpdateNext;
        m_UpdateNext.clear();

        for( unsigned int i = 0; i < m_UpdateCurrent.size(); ++i )
        {
            m_UpdateCurrent[ i ]->Update();
        }

        ++counter;
        if( counter > 1000 )
        {
            return;
        }
    }
}



void
SubCircuit::Trace( Line* line, Connect* connect )
{
    m_Visited = true;

    if( connect == m_Contacts[ 2 ].connect )
    {
        if( m_Switch == 1 )
        {
            line->SetValue( 1 );
        }
    }
    else if( m_Switch == true )
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
SubCircuit::SetValue( const int value, Connect* connect )
{
    m_Visited = false;

    if( m_Contacts[ 2 ].connect == connect )
    {
        if( value != -1 )
        {
            if( m_Switch != value )
            {
                if( m_Circuit != 0 )
                {
                    m_Circuit->AddToUpdate( this );
                }
                m_Switch = value;
            }
        }
    }
}

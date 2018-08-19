#include "Connect.h"
#include "Element.h"
#include "Wire.h"
#include "Line.h"



Connect::Connect():
    m_Value( -1 )
{
}



Connect::~Connect()
{
}



void
Connect::Trace( Line* line )
{
    line->AddConnect( this );

    for( unsigned int i = 0; i < m_Elements.size(); ++i )
    {
        if( m_Elements[ i ]->IsVisited() == false )
        {
            m_Elements[ i ]->Trace( line, this );
        }
    }
}



int
Connect::GetValue()
{
    return m_Value;
}



void
Connect::SetValue( const int value )
{
    m_Value = value;
    for( unsigned int i = 0; i < m_Wires.size(); ++i )
    {
        m_Wires[ i ]->SetValue( value );
    }

    for( unsigned int i = 0; i < m_Elements.size(); ++i )
    {
        if( m_Elements[ i ]->IsVisited() == true )
        {
            m_Elements[ i ]->SetValue( value, this );
        }
    }
}



void
Connect::AddElement( Element* element )
{
    m_Elements.push_back( element );
}



void
Connect::RemoveElement( Element* element )
{
    m_Elements.erase( std::remove( m_Elements.begin(), m_Elements.end(), element ), m_Elements.end() );

    if( m_Elements.size() == 0 )
    {
        m_Value = -1;
        for( unsigned int i = 0; i < m_Wires.size(); ++i )
        {
            m_Wires[ i ]->SetValue( -1 );
        }
    }
}



void
Connect::ClearElements()
{
    for( unsigned int i = 0; i < m_Elements.size(); ++i )
    {
        m_Elements[ i ]->RemoveConnect( this );
    }
    m_Elements.clear();
    m_Value = -1;
    for( unsigned int i = 0; i < m_Wires.size(); ++i )
    {
        m_Wires[ i ]->SetValue( -1 );
    }
}



bool
Connect::IsEmpty() const
{
    if( m_Wires.size() == 0 )
    {
        return m_Elements.size() <= 1;
    }

    return false;
}



void
Connect::ClearAll()
{
    ClearElements();
    m_Wires.clear();
}



void
Connect::AddWire( Wire* wire )
{
    m_Wires.push_back( wire );
    wire->SetConnect( this );
    wire->SetValue( m_Value );
}



void
Connect::RemoveWire( Wire* wire )
{
    m_Wires.erase( std::remove( m_Wires.begin(), m_Wires.end(), wire ), m_Wires.end() );
}



std::vector< Wire* >
Connect::GetWires()
{
    return m_Wires;
}



void
Connect::Merge( Connect* connect )
{
    std::vector< Wire* > wires = connect->GetWires();
    // relink all wires
    for( unsigned int i = 0; i < wires.size(); ++i )
    {
        wires[ i ]->SetConnect( this );
    }
    m_Wires.insert( m_Wires.end(), wires.begin(), wires.end() );

    // relink all elements to this connect
    for( unsigned int i = 0; i < connect->m_Elements.size(); ++i )
    {
        connect->m_Elements[ i ]->UpdateConnect( connect, this );
    }
    m_Elements.insert( m_Elements.end(), connect->m_Elements.begin(), connect->m_Elements.end() );
}

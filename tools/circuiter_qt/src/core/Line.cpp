#include "Line.h"

#include <stdio.h>
#include "Connect.h"



Line::Line():
    m_Value( -1 )
{
}



Line::~Line()
{
}



void
Line::SetValue( const int value )
{
    if( m_Value != 0 )
    {
        if( value == 0 )
        {
            m_Value = 0;
        }
        else if( value == 1 )
        {
            m_Value = 1;
        }
    }
}



void
Line::AddConnect( Connect* connect )
{
    int size = m_AffectedConnect.size();
    m_AffectedConnect.push_back( connect );
}



void
Line::UpdateConnects()
{
    for( unsigned int i = 0; i < m_AffectedConnect.size(); ++i )
    {
        m_AffectedConnect[ i ]->SetValue( m_Value );
    }
}

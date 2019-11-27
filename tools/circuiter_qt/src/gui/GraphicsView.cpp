#include "GraphicsView.h"
#include "Frame.h"

#include <QtWidgets>



GraphicsView::GraphicsView( View *v ):
    QGraphicsView(),
    m_View( v )
{
    setRenderHint( QPainter::Antialiasing, true );
    setDragMode( QGraphicsView::RubberBandDrag );
    setOptimizationFlags( QGraphicsView::DontSavePainterState );
    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    ResetView();
}



void
GraphicsView::ResetView()
{
    m_Scale = 1.0f;
    m_Translate = QPoint( 0, 0 );
    SetupMatrix();
}



void
GraphicsView::SetupMatrix()
{
    QMatrix matrix;
    matrix.translate( m_Translate.x(), m_Translate.y() );
    matrix.scale( m_Scale, m_Scale );
    setMatrix( matrix );
}



void
GraphicsView::Scale( const qreal scale )
{
    m_Scale = scale;
    m_Scale = ( m_Scale > 10.0f ) ? 10.0f: m_Scale;
    m_Scale = ( m_Scale < 0.1f ) ? 0.1f: m_Scale;
    setTransformationAnchor( QGraphicsView::NoAnchor );
    SetupMatrix();
    setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
}



void
GraphicsView::wheelEvent( QWheelEvent* event )
{
    // enable ancor for scale
    if( event->delta() > 0 )
    {
        m_Scale *= 1.1f;
        m_Scale = ( m_Scale > 10.0f ) ? 10.0f: m_Scale;
    }
    else
    {
        m_Scale /= 1.1f;
        m_Scale = ( m_Scale < 0.1f ) ? 0.1f: m_Scale;
    }
    m_View->ScaleToSlider( m_Scale );
    SetupMatrix();
    event->accept();
}



void
GraphicsView::mousePressEvent( QMouseEvent* event )
{
    if( event->button() == Qt::RightButton )
    {
        m_PrevTrans = event->globalPos();
    }
    else
    {
        QGraphicsView::mousePressEvent( event );
    }
    event->accept();
}



void
GraphicsView::mouseReleaseEvent( QMouseEvent* event )
{
    QGraphicsView::mouseReleaseEvent( event );
    event->accept();
}




void
GraphicsView::mouseMoveEvent( QMouseEvent *event )
{
    if( event->buttons() & Qt::RightButton )
    {
        m_Translate += event->globalPos() - m_PrevTrans;
        m_PrevTrans = event->globalPos();
        setTransformationAnchor( QGraphicsView::NoAnchor );
        SetupMatrix();
        setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
    }
    else
    {
        QGraphicsView::mouseMoveEvent( event );
    }
    event->accept();
}



void
GraphicsView::mouseDoubleClickEvent( QMouseEvent* event )
{
    event->accept();
}

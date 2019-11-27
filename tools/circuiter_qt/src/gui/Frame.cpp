#include "Frame.h"

#include "GraphicsView.h"

#include <QtWidgets>
#include <qmath.h>



View::View( QWidget* parent ):
    QFrame( parent )
{
    setFrameStyle( Sunken | StyledPanel );
    graphicsView = new GraphicsView( this );

    int size = style()->pixelMetric( QStyle::PM_ToolBarIconSize );
    QSize iconSize( size, size );

    QToolButton *zoomInIcon = new QToolButton();
    zoomInIcon->setAutoRepeat( true );
    zoomInIcon->setAutoRepeatInterval( 33 );
    zoomInIcon->setAutoRepeatDelay( 0 );
    zoomInIcon->setIcon( QPixmap( ":/images/zoomin.png" ) );
    zoomInIcon->setIconSize( iconSize );
    QToolButton *zoomOutIcon = new QToolButton();
    zoomOutIcon->setAutoRepeat( true );
    zoomOutIcon->setAutoRepeatInterval( 33 );
    zoomOutIcon->setAutoRepeatDelay( 0 );
    zoomOutIcon->setIcon( QPixmap( ":/images/zoomout.png" ) );
    zoomOutIcon->setIconSize( iconSize );

    m_ZoomSlider = new QSlider();
    m_ZoomSlider->setMinimum( 1 );
    m_ZoomSlider->setMaximum( 100 );
    m_ZoomSlider->setValue( 10 );
    m_ZoomSlider->setTickPosition( QSlider::TicksRight );

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout();
    zoomSliderLayout->addWidget( zoomInIcon );
    zoomSliderLayout->addWidget( m_ZoomSlider );
    zoomSliderLayout->addWidget( zoomOutIcon );

    resetButton = new QToolButton();
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout();
    label = new QLabel("labeslll");
    labelLayout->addWidget(label);

    QGridLayout* topLayout = new QGridLayout();
    topLayout->addLayout( labelLayout, 0, 0 );
    topLayout->addWidget( graphicsView, 1, 0 );
    topLayout->addLayout( zoomSliderLayout, 1, 1 );
    topLayout->addWidget( resetButton, 2, 1 );
    setLayout( topLayout );

    connect( m_ZoomSlider, SIGNAL( valueChanged( int ) ), this, SLOT( ScaleBySlider() ) );
    connect( zoomInIcon, SIGNAL( clicked() ), this, SLOT( ZoomIn() ) );
    connect( zoomOutIcon, SIGNAL( clicked() ), this, SLOT( ZoomOut() ) );
}



QGraphicsView*
View::view() const
{
    return static_cast< QGraphicsView* >( graphicsView );
}



void
View::ScaleBySlider()
{
    graphicsView->Scale( ( qreal )m_ZoomSlider->value() / 10.0f );
}



void
View::ScaleToSlider( const qreal scale )
{
    m_ZoomSlider->blockSignals( true );
    m_ZoomSlider->setValue( scale * 10 );
    m_ZoomSlider->blockSignals( false );
}



void
View::ZoomIn()
{
    m_ZoomSlider->setValue( m_ZoomSlider->value() + 1 );
}



void
View::ZoomOut()
{
    m_ZoomSlider->setValue( m_ZoomSlider->value() - 1 );
}

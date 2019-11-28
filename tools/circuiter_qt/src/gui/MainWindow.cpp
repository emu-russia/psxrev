#include "MainWindow.h"

#include <QtWidgets>
#include <QHBoxLayout>
#include <QSplitter>
#include <QXmlSimpleReader>

#include "GraphicsItem.h"
#include "Frame.h"
#include "GraphicsScene.h"
#include "model/Container.h"
#include "model/Ground.h"
#include "model/Nfet.h"
#include "model/Pfet.h"
#include "model/Pin.h"
#include "model/Power.h"
#include "model/Wire.h"



MainWindow::MainWindow( QWidget *parent ):
    QMainWindow( parent )
{
    m_OpenAct = new QAction( tr( "&Open..." ), this );
    m_OpenAct->setShortcuts( QKeySequence::Open );
    connect( m_OpenAct, SIGNAL( triggered() ), this, SLOT( Open() ) );
    m_SaveAsAct = new QAction( tr( "&Save As..." ), this );
    m_SaveAsAct->setShortcuts( QKeySequence::SaveAs );
    connect( m_SaveAsAct, SIGNAL( triggered() ), this, SLOT( SaveAs() ) );
    m_ExitAct = new QAction( tr( "E&xit" ), this );
    m_ExitAct->setShortcuts( QKeySequence::Quit );
    connect( m_ExitAct, SIGNAL( triggered() ), this, SLOT( close() ) );

    m_FileMenu = menuBar()->addMenu( tr( "&File" ) );
    m_FileMenu->addAction( m_OpenAct );
    m_FileMenu->addAction( m_SaveAsAct );
    m_FileMenu->addAction( m_ExitAct );

    m_ToolBar = addToolBar( tr( "Elements" ) );
    QAction* insert_container = new QAction( QIcon( QPixmap( ":/images/container.png" ) ), tr( "&InsertContainer" ), this );
    insert_container->setStatusTip( tr( "Inserts new Container element." ) );
    connect( insert_container, &QAction::triggered, this, &MainWindow::InsertContainer );
    m_ToolBar->addAction( insert_container );
    QAction* insert_pin = new QAction( QIcon( QPixmap( ":/images/pin.png" ) ), tr( "&InsertPin" ), this );
    insert_pin->setStatusTip( tr( "Inserts new Pin element." ) );
    connect( insert_pin, &QAction::triggered, this, &MainWindow::InsertPin );
    m_ToolBar->addAction( insert_pin );
    QAction* insert_ground = new QAction( QIcon( QPixmap( ":/images/ground.png" ) ), tr( "&InsertGround" ), this );
    insert_ground->setStatusTip( tr( "Inserts new Ground element." ) );
    connect( insert_ground, &QAction::triggered, this, &MainWindow::InsertGround );
    m_ToolBar->addAction( insert_ground );
    QAction* insert_power = new QAction( QIcon( QPixmap( ":/images/power.png" ) ), tr( "&InsertPower" ), this );
    insert_power->setStatusTip( tr( "Inserts new Power element." ) );
    connect( insert_power, &QAction::triggered, this, &MainWindow::InsertPower );
    m_ToolBar->addAction( insert_power );
    QAction* insert_nfet = new QAction( QIcon( QPixmap( ":/images/nfet.png" ) ), tr( "&InsertNfet" ), this );
    insert_nfet->setStatusTip( tr( "Inserts new Nfet element." ) );
    connect( insert_nfet, &QAction::triggered, this, &MainWindow::InsertNfet );
    m_ToolBar->addAction( insert_nfet );
    QAction* insert_pfet = new QAction( QIcon( QPixmap( ":/images/pfet.png" ) ), tr( "&InsertPfet" ), this );
    insert_pfet->setStatusTip( tr( "Inserts new Pfet element." ) );
    connect( insert_pfet, &QAction::triggered, this, &MainWindow::InsertPfet );
    m_ToolBar->addAction( insert_pfet );

    m_Splitter = new QSplitter();
    m_Splitter->setOrientation( Qt::Horizontal );

    m_Scene = new GraphicsScene( this );
    m_Scene->setSceneRect( -32000, -32000, 64000, 64000 );

    m_View = new View();
    m_Splitter->addWidget( m_View );
    m_View->view()->setScene( m_Scene );
    setCentralWidget( m_Splitter );

    setWindowTitle( tr( "Circuter" ) );

    resize( QDesktopWidget().availableGeometry( this ).size() * 0.7 );

    m_CircuitTimer.start( 15 );
    connect( &m_CircuitTimer, SIGNAL( timeout() ), this, SLOT( CircuitUpdate() ) );
}



MainWindow::~MainWindow()
{
    if( m_Scene != 0 )
    {
        delete m_Scene;
    }
}





void
MainWindow::Open()
{
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open GraphicsScene File" ), QDir::currentPath(), tr( "GraphicsScene Files (*.xml)" ) );
    if( filename.isEmpty() )
    {
        return;
    }

    if( m_Scene != 0 )
    {
        delete m_Scene;
    }

    m_Scene = new GraphicsScene( this );
    m_Scene->setSceneRect( -32000, -32000, 64000, 64000 );
    m_View->view()->setScene( m_Scene );

    QFile file( filename );
    if( !file.open( QFile::ReadOnly | QFile::Text ) )
    {
        QMessageBox::warning( this, tr( "GraphicsScene Load Error" ), tr( "Cannot read file %1:\n%2." ).arg( filename ).arg( file.errorString() ) );
        return;
    }

    Wire* wire;
    Element* el;
    QXmlStreamReader xml( &file );
    if( xml.readNextStartElement() )
    {
        if( xml.name() == "circuit" )
        {
            while( xml.readNextStartElement() )
            {
                if( xml.name() == "wire")
                {
                    QStringRef str = xml.attributes().value( "line" );
                    QVector< QStringRef > vec = str.split( " " );
                    if( vec.size() >= 4 )
                    {
                        wire = new Wire();
                        wire->SetLine( QLine( vec[ 0 ].toInt(), vec[ 1 ].toInt(), vec[ 2 ].toInt(), vec[ 3 ].toInt() ) );
                        m_Scene->ConnectWire( wire );
                    }
                }
                else if( xml.name() == "element" )
                {
                    QStringRef str = xml.attributes().value( "pos" );
                    QVector< QStringRef > vec = str.split( " " );
                    if( vec.size() >= 2 )
                    {
                        el = 0;
                        QString type = xml.attributes().value( "type" ).toString();
                        if( type == "pin" )
                        {
                            el = new Pin( NULL );
                        }
                        else if( type == "ground" )
                        {
                            el = new Ground( NULL );
                        }
                        else if( type == "power" )
                        {
                            el = new Power( NULL );
                        }
                        else if( type == "nfet" )
                        {
                            el = new Nfet( NULL );
                        }
                        else if( type == "pfet" )
                        {
                            el = new Pfet( NULL );
                        }
                        el->setPos( QPointF( vec[ 0 ].toInt(), vec[ 1 ].toInt() ) );
                        el->setRotation( xml.attributes().value( "rot" ).toInt() );
                        m_Scene->ConnectElement( el );
                    }
                }
                xml.skipCurrentElement();
            }
        }
        else
        {
            xml.raiseError( QObject::tr( "The file is not circuit file. No <circuit> in root." ) );
        }
    }

    if( xml.error() )
    {
        QMessageBox::warning( this, tr( "GraphicsScene Load Error" ), tr( "Parse error in file %1:\n\n%2" ).arg( filename ).arg( xml.errorString() ) );
    }
    else
    {
        statusBar()->showMessage( tr( "File %1 loaded" ).arg( filename ), 2000 );
    }

    m_Scene->UpdateAll();
}



void
MainWindow::SaveAs()
{
    QString filename = QFileDialog::getSaveFileName( this, tr( "Save GraphicsScene File" ), QDir::currentPath(), tr( "GraphicsScene Files (*.xml)" ) );
    if( filename.isEmpty() )
    {
        return;
    }

    QFile file( filename );
    if( !file.open( QFile::WriteOnly | QFile::Text ) )
    {
        QMessageBox::warning( this, tr( "GraphicsScene Save Error" ), tr( "Cannot write file %1:\n%2." ).arg( filename ).arg( file.errorString() ) );
        return;
    }

    QXmlStreamWriter xml( &file );
    xml.setAutoFormatting( true );
    xml.writeStartDocument();
    xml.writeStartElement( "circuit" );
    if( m_Scene != 0 )
    {
        std::vector< Wire* > wires = m_Scene->GetCurrentContainer()->GetWires();
        for( unsigned int i = 0; i < wires.size(); ++i )
        {
            QLine line = wires[ i ]->GetLine();
            QPoint pos = wires[ i ]->pos().toPoint();
            line.translate( pos );
            xml.writeStartElement( "wire" );
            xml.writeAttribute( "line", tr( "%1 %2 %3 %4" ).arg( line.x1() ).arg( line.y1() ).arg( line.x2() ).arg( line.y2() ) );
            xml.writeEndElement();
        }

        std::vector< Element* > elements = m_Scene->GetCurrentContainer()->GetElements();
        for( unsigned int i = 0; i < elements.size(); ++i )
        {
            Element* el = elements[ i ];
            xml.writeStartElement( "element" );
            switch( el->GetType() )
            {
                case GraphicsItem::IT_PIN: xml.writeAttribute( "type", "pin" ); break;
                case GraphicsItem::IT_GROUND: xml.writeAttribute( "type", "ground" ); break;
                case GraphicsItem::IT_POWER: xml.writeAttribute( "type", "power" ); break;
                case GraphicsItem::IT_NFET: xml.writeAttribute( "type", "nfet" ); break;
                case GraphicsItem::IT_PFET: xml.writeAttribute( "type", "pfet" ); break;
            }
            xml.writeAttribute( "pos", tr( "%1 %2" ).arg( el->pos().x() ).arg( el->pos().y() ) );
            xml.writeAttribute( "rot", tr( "%1" ).arg( el->rotation() ) );
            xml.writeEndElement();
        }
    }

    xml.writeEndDocument();
    statusBar()->showMessage( tr( "File saved" ), 2000 );
}



void
MainWindow::CircuitUpdate()
{
    if( m_Scene != 0 )
    {
        m_Scene->DoStep();
        m_Scene->update( m_Scene->sceneRect() );
    }
}



void
MainWindow::InsertContainer()
{
    if( m_Scene != NULL )
    {
        m_Scene->InsertContainer();
    }
}



void
MainWindow::InsertPin()
{
    if( m_Scene != 0 )
    {
        m_Scene->InsertPin();
    }
}



void
MainWindow::InsertGround()
{
    if( m_Scene != 0 )
    {
        m_Scene->InsertGround();
    }
}



void
MainWindow::InsertPower()
{
    if( m_Scene != 0 )
    {
        m_Scene->InsertPower();
    }
}



void
MainWindow::InsertNfet()
{
    if( m_Scene != 0 )
    {
        m_Scene->InsertNfet();
    }
}



void
MainWindow::InsertPfet()
{
    if( m_Scene != 0 )
    {
        m_Scene->InsertPfet();
    }
}

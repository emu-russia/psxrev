#include "GraphicsScene.h"

#include "../model/Container.h"
#include "../model/Connect.h"
#include "../model/Element.h"
#include "../model/Wire.h"
#include "../model/Ground.h"
#include "../model/Nfet.h"
#include "../model/Pfet.h"
#include "../model/Pin.h"
#include "../model/Power.h"

#include <QGraphicsItem>
#include <QtWidgets>



bool SortWireContact( GraphicsScene::WireContact con1, GraphicsScene::WireContact con2 )
{
    if( con1.pos.x() == con2.pos.x() )
    {
        return con1.pos.y() < con2.pos.y();
    }
    else
    {
        return con1.pos.x() < con2.pos.x();
    }
}



GraphicsScene::GraphicsScene( QObject* parent ):
    QGraphicsScene( parent ),
    m_WireDraw( false ),
    m_WireMode( GraphicsScene::WIRE_HORIZONTAL ),
    m_MoveStart( false ),
    m_MoveOffset( QPointF( 0, 0 ) ),
    m_MovedHold( false )
{
    m_RootContainer = new Container( NULL );
    m_CurrentContainer = m_RootContainer;
}



GraphicsScene::~GraphicsScene()
{
    for( unsigned int i = 0; i < m_MovedItem.size(); ++i )
    {
        delete m_MovedItem[ i ];
    }

    for( unsigned int i = 0; i < m_CopiedItem.size(); ++i )
    {
        delete m_CopiedItem[ i ];
    }

    for( unsigned int i = 0; i < m_ContainerDefs.size(); ++i )
    {
        delete m_ContainerDefs[ i ];
    }
}



void
GraphicsScene::RemoveAll()
{
    QList< QGraphicsItem* > item_list = items();

    for( size_t i = 0; i < item_list.size(); i++ )
    {
        removeItem( item_list[ i ] );
    }

    for( size_t i = 0; i < m_MovedItem.size(); ++i )
    {
        delete m_MovedItem[ i ];
    }
}



void
GraphicsScene::ConnectElement( Element* element )
{
    m_CurrentContainer->InsertElement( element );
    addItem( element );

    std::vector< Wire* > wires_for_update;

    // calculate if this element connects with wires
    std::vector< WireContact > contacts;
    CalculateElementWiresIntersect( element, contacts );
    for( unsigned int i = 0; i < contacts.size(); ++i )
    {
        if( contacts[ i ].wire_point == 0 )
        {
            wires_for_update.push_back( m_CurrentContainer->GetWires()[ contacts[ i ].wire_id ] );
        }
        element->AddConnect( m_CurrentContainer->GetWires()[ contacts[ i ].wire_id ]->GetConnect(), contacts[ i ].self_contact_id );
    }

    // calculate if this element connects with other elements directly
    // there can be connection with wire as well so we need to look if connection exist
    contacts.clear();
    CalculateElementElementsIntersect( element, contacts );
    for( unsigned int i = 0; i < contacts.size(); ++i )
    {
        Connect* connect = m_CurrentContainer->GetElements()[ contacts[ i ].element_id ]->GetContacts()[ contacts[ i ].contact_id ].connect;
        if( connect == 0 )
        {
            connect = new Connect();
            m_CurrentContainer->InsertConnect( connect );
            m_CurrentContainer->GetElements()[ contacts[ i ].element_id ]->AddConnect( connect, contacts[ i ].contact_id );
        }
        element->AddConnect( connect, contacts[ i ].self_contact_id );
    }

    // if some wires was splitted by addition
    for( unsigned int i = 0; i < wires_for_update.size(); ++i )
    {
        wires_for_update[ i ]->GetConnect()->RemoveWire( wires_for_update[ i ] );
        m_CurrentContainer->RemoveWire( wires_for_update[ i ] );
        ConnectWire( wires_for_update[ i ] );
    }
}



void
GraphicsScene::ConnectWire( Wire* wire )
{
    // merge given wire with existed wires
    // this deletes old wire and we need to add new - merged one
    wire = MergeWire( wire );

    // split merged wire into parts by connect points
    std::vector< WireContact > contacts;
    std::vector< Wire* > wires;
    QLine line = wire->GetLine();
    QPoint pos = wire->pos().toPoint();
    line.translate( pos );
    QPoint p1 = line.p1();
    QPoint p2 = line.p2();
    if( p1.y() > p2.y() || p1.x() > p2.x() )
    {
        QPoint t = p1;
        p1 = p2;
        p2 = t;
    }
    CalculateLineElementsIntersect( line, contacts );
    CalculateLineWiresIntersect( line, contacts );
    std::sort( contacts.begin(), contacts.end(), SortWireContact );
    if( contacts.size() > 0 )
    {
        QPoint pc = contacts[ 0 ].pos;
        if( pc.y() > p1.y() || pc.x() > p1.x() )
        {
            Wire* sub_wire = new Wire();
            sub_wire->SetLine( QLine( p1, pc ) );
            wires.push_back( sub_wire );
        }

        for( unsigned int cont_id = 0; cont_id < contacts.size(); ++cont_id )
        {
            QPoint pc1 = contacts[ cont_id ].pos;
            QPoint pc2 = ( cont_id + 1 < contacts.size() ) ? contacts[ cont_id + 1 ].pos : p2;
            if( pc1 != pc2 )
            {
                Wire* sub_wire = new Wire();
                sub_wire->SetLine( QLine( pc1, pc2 ) );
                wires.push_back( sub_wire );
            }
        }
        delete wire;
    }
    else
    {
        wires.push_back( wire );
    }

    std::vector< Wire* > wires_for_update;
    for( unsigned int sub_id = 0; sub_id < wires.size(); sub_id++ )
    {
        wire = wires[ sub_id ];
        m_CurrentContainer->InsertWire( wire );
        addItem( wire );

        // calculate if this wire connects to something and connect it to it
        // if we connect to some wire and this is not end of wire - then draw point of connection
        Connect* connect = 0;
        std::vector< WireContact > contacts;
        CalculateWireWiresIntersect( wire, contacts );
        bool con_p1 = false;
        bool con_p2 = false;
        for( unsigned int i = 0; i < contacts.size(); ++i )
        {
            if( contacts[ i ].wire_point == 0 )
            {
                wires_for_update.push_back( m_CurrentContainer->GetWires()[ contacts[ i ].wire_id ] );
            }

            // add point on wire for drawing
            // add only on one wire because expect to remove and add all connected wire on remove
            // point needs to be added to wire because want to draw it in wire colour
            if( contacts[ i ].self_point == 1 )
            {
                if( con_p1 == true )
                {
                    wire->AddConnectPoint( 1 );
                }
                con_p1 = true;
            }
            if( contacts[ i ].self_point == 2 )
            {
                if( con_p2 == true )
                {
                    wire->AddConnectPoint( 2 );
                }
                con_p2 = true;
            }

            if( connect == 0 )
            {
                connect = m_CurrentContainer->GetWires()[ contacts[ i ].wire_id ]->GetConnect();
            }
            else
            {
                Connect* existed = m_CurrentContainer->GetWires()[ contacts[ i ].wire_id ]->GetConnect();
                if( connect != existed )
                {
                    connect->Merge( existed );
                    delete existed;
                    m_CurrentContainer->RemoveConnect( existed );
                }
            }
        }

        // check connection of new wire with elements
        // if this element already has connection (in case of direct connection)
        // use it and don't create new connection
        contacts.clear();
        CalculateWireElementsIntersect( wire, contacts );
        for( unsigned int i = 0; i < contacts.size(); ++i )
        {
            Connect* existed = m_CurrentContainer->GetElements()[ contacts[ i ].element_id ]->GetContacts()[ contacts[ i ].contact_id ].connect;
            if( existed == 0 && connect == 0 )
            {
                connect = new Connect();
                m_CurrentContainer->InsertConnect( connect );
            }
            else if( existed != 0 && connect == 0 )
            {
                connect = existed;
            }
            else if( existed != 0 && connect != 0 )
            {
                if( connect != existed )
                {
                    connect->Merge( existed );
                    delete existed;
                    m_CurrentContainer->RemoveConnect( existed );
                }
            }
            m_CurrentContainer->GetElements()[ contacts[ i ].element_id ]->AddConnect( connect, contacts[ i ].contact_id );
        }

        // if this new wire don't connected to other wires
        // or elements then create new connect anyway
        if( connect == 0 )
        {
            connect = new Connect();
            m_CurrentContainer->InsertConnect( connect );
        }

        // add this wire to collection of wires in connect to update
        connect->AddWire( wire );
    }

    // if some wires was splitted by addition
    for( unsigned int i = 0; i < wires_for_update.size(); ++i )
    {
        wires_for_update[ i ]->GetConnect()->RemoveWire( wires_for_update[ i ] );
        m_CurrentContainer->RemoveWire( wires_for_update[ i ] );
        ConnectWire( wires_for_update[ i ] );
    }
}



Wire*
GraphicsScene::MergeWire( Wire* wire )
{
    Wire* ret = wire;

    std::vector< Wire* > wires = m_CurrentContainer->GetWires();

    // go through all wire and search for matching
    for( size_t i = 0; i < wires.size(); ++i )
    {
        // return line can update during merge
        QLine line_new = ret->GetLine();
        QPoint pos_new = ret->pos().toPoint();
        line_new.translate( pos_new );

        if( ret != wires[ i ] )
        {
            QLine line_old = wires[ i ]->GetLine();
            QPoint pos_old = wires[ i ]->pos().toPoint();
            line_old.translate( pos_old );
            QPoint p_new1 = line_new.p1();
            QPoint p_new2 = line_new.p2();
            QPoint p_old1 = line_old.p1();
            QPoint p_old2 = line_old.p2();
            bool p_new1_old = IsLinePointIntersect( line_old, p_new1 );
            bool p_new2_old = IsLinePointIntersect( line_old, p_new2 );
            bool p_old1_new = IsLinePointIntersect( line_new, p_old1 );
            bool p_old2_new = IsLinePointIntersect( line_new, p_old2 );

            // if both points of given line on old line
            if( p_new1_old == true && p_new2_old == true )
            {
                delete ret;
                ret = wires[ i ];
                ret->GetConnect()->RemoveWire( ret );
                removeItem( ret );
                m_CurrentContainer->RemoveWire( wires[ i ] );
            }
            // if both points of old line on given line
            else if( p_old1_new == true && p_old2_new == true )
            {
                wires[ i ]->GetConnect()->RemoveWire( wires[ i ] );
                removeItem( wires[ i ] );
                m_CurrentContainer->RemoveWire( wires[ i ] );
                delete wires[ i ];
            }
            // if one point from both line on other line
            else if( ( p_new1_old == true || p_new2_old == true ) && ( p_old1_new == true || p_old2_new == true ) && (
                     ( p_new1.x() == p_new2.x() && p_new2.x() == p_old1.x() && p_new2.x() == p_old2.x() ) || // on same x
                     ( p_new1.y() == p_new2.y() && p_new2.y() == p_old1.y() && p_new2.y() == p_old2.y() ) ) ) // on same y
            {
                ret->SetLine( QLine( ( p_new2_old == true ) ? p_new1 : p_new2, ( p_old2_new == true ) ? p_old1 : p_old2 ) );
                ret->setPos( QPoint( 0, 0 ) );
                wires[ i ]->GetConnect()->RemoveWire( wires[ i ] );
                removeItem( wires[ i ] );
                m_CurrentContainer->RemoveWire( wires[ i ] );
                delete wires[ i ];
            }
        }
    }

    return ret;
}



void
GraphicsScene::DisconnectItems( std::vector< GraphicsItem* >& items )
{
    // get unique connects from disconnect items
    std::vector< Connect* > connects;
    for( unsigned int i = 0; i < items.size(); ++i )
    {
        if( items[ i ]->GetType() == GraphicsItem::IT_WIRE )
        {
            connects.push_back( ( ( Wire* )items[ i ] )->GetConnect() );
        }
        else
        {
            m_CurrentContainer->RemoveElement( ( Element* )items[ i ] );
            removeItem( items[ i ] );

            std::vector< Element::Contact > contacts = ( ( Element* )( items[ i ] ) )->GetContacts();
            for( unsigned int j = 0; j < contacts.size(); ++j )
            {
                if( contacts[ j ].connect != 0 )
                {
                    ( ( Element* )( items[ i ] ) )->AddConnect( 0, j );
                    connects.push_back( contacts[ j ].connect );
                }
            }
        }
    }
    std::sort( connects.begin(), connects.end());
    connects.erase( std::unique( connects.begin(), connects.end() ), connects.end());

    // save all wires from connections
    std::vector< Wire* > con_wires;
    for( unsigned int i = 0; i < connects.size(); ++i )
    {
        std::vector< Wire* > new_wires = connects[ i ]->GetWires();
        con_wires.insert( con_wires.end(), new_wires.begin(), new_wires.end() );
        connects[ i ]->ClearAll();
        m_CurrentContainer->RemoveConnect( connects[ i ] );
        delete connects[ i ];
    }

    // remove all wires that was connected to removed connects
    for( unsigned int i = 0; i < con_wires.size(); ++i )
    {
        m_CurrentContainer->RemoveWire( con_wires[ i ] );
        removeItem( con_wires[ i ] );
    }

    // remove disconnect wires
    for( unsigned int i = 0; i < items.size(); ++i )
    {
        if( items[ i ]->GetType() == GraphicsItem::IT_WIRE )
        {
            con_wires.erase( std::remove( con_wires.begin(), con_wires.end(), items[ i ] ), con_wires.end() );
        }
    }

    // add connected wires again except disconnected wires
    for( unsigned int i = 0; i < con_wires.size(); ++i )
    {
        ConnectWire( con_wires[ i ] );
    }
}



void
GraphicsScene::DoStep()
{
    m_RootContainer->Update();
}



void
GraphicsScene::UpdateAll()
{
    DoStep();
}



void
GraphicsScene::SetCurrentContainer( Container* container )
{
    RemoveAll();

    m_CurrentContainer = container;

    std::vector< Element* > elements = m_CurrentContainer->GetElements();
    for( size_t i = 0; i < elements.size(); ++i )
    {
        addItem( elements[ i ] );
    }
    std::vector< Wire* > wires = m_CurrentContainer->GetWires();
    for( size_t i = 0; i < wires.size(); ++i )
    {
        addItem( wires[ i ] );
    }
}



Container*
GraphicsScene::GetCurrentContainer()
{
    return m_CurrentContainer;
}



void
GraphicsScene::InsertElement( GraphicsItem* element )
{
    m_MovedHold = true;
    QList< QGraphicsView* > view = views();
    QPoint origin = view[ 0 ]->mapFromGlobal( QCursor::pos() );
    QPointF relative_origin = view[ 0 ]->mapToScene( origin );
    element->setPos( PointAlign( relative_origin.toPoint() ) );
    element->setSelected( true );
    m_MovedItem.push_back( element );
    m_MoveOffset = QPointF( 0, 0 );
    addItem( element );
}



void
GraphicsScene::AddContainerDef( Container* container )
{
    m_ContainerDefs.push_back( container );
}



void
GraphicsScene::InsertContainer()
{
    GraphicsItem* element = new Container( m_CurrentContainer );

    if( m_ContainerDefs.size() > 0 )
    {
        Container* temp = m_CurrentContainer;

        SetCurrentContainer( ( Container* )element );

        std::vector< Element* > elements = m_ContainerDefs[ 0 ]->GetElements();
        for( size_t i = 0; i < elements.size(); ++i )
        {
            Element* element = elements[ i ]->Copy( m_CurrentContainer );
            element->setPos( elements[ i ]->pos() );
            ConnectElement( element );
        }

        std::vector< Wire* > wires = m_ContainerDefs[ 0 ]->GetWires();
        for( size_t i = 0; i < wires.size(); ++i )
        {
            Wire* wire = new Wire();
            wire->SetLine( wires[ i ]->GetLine() );
            ConnectWire( wire );
        }

        SetCurrentContainer( temp );
    }

    InsertElement( element );
}



void
GraphicsScene::InsertPin()
{
    GraphicsItem* element = new Pin( m_CurrentContainer );
    InsertElement( element );
}



void
GraphicsScene::InsertGround()
{
    GraphicsItem* element = new Ground( m_CurrentContainer );
    InsertElement( element );
}



void
GraphicsScene::InsertPower()
{
    GraphicsItem* element = new Power( m_CurrentContainer );
    InsertElement( element );
}



void
GraphicsScene::InsertNfet()
{
    GraphicsItem* element = new Nfet( m_CurrentContainer );
    InsertElement( element );
}



void
GraphicsScene::InsertPfet()
{
    GraphicsItem* element = new Pfet( m_CurrentContainer );
    InsertElement( element );
}



void
GraphicsScene::drawBackground( QPainter* painter, const QRectF& rect )
{
    Q_UNUSED( painter );
    Q_UNUSED( rect );
}



void
GraphicsScene::drawForeground( QPainter* painter, const QRectF& rect )
{
    painter->setPen( QPen( WIRE_DRAW_COLOR, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    for( unsigned int i = 0; i < m_WiresDraw.size(); ++i )
    {
        painter->drawLine( m_WiresDraw[ i ] );
    }
    painter->setPen( QPen( Qt::blue, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin ) );
    painter->setBrush( Qt::NoBrush );
    for( unsigned int i = 0; i < m_WiresConDraw.size(); ++i )
    {
        painter->drawEllipse( m_WiresConDraw[ i ].x() - 5, m_WiresConDraw[ i ].y() - 5, 10, 10 );
    }

    QGraphicsScene::drawForeground( painter, rect );
}



void
GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
    if( event->button() == Qt::LeftButton )
    {
        if( selectedItems().size() > 0 )
        {
            QGraphicsItem* item = itemAt( event->scenePos(), ( ( QGraphicsView* )event->widget()->parentWidget() )->viewportTransform() );
            if( item == 0 )
            {
                clearSelection();
            }
            else if( item != 0 && item->isSelected() == false )
            {
                clearSelection();
            }
            else
            {
                m_MoveOffset = QPointF( 0, 0 );
            }
            event->accept();
        }
        else
        {
            // we can only start draw from wire or contact with element
            QPoint mouse_pos = PointAlign( event->scenePos().toPoint() );
            std::vector< WireContact > contacts;
            CalculateLineElementsIntersect( QLine( mouse_pos, mouse_pos ), contacts );
            CalculateLineWiresIntersect( QLine( mouse_pos, mouse_pos ), contacts );
            if( contacts.size() > 0 )
            {
                m_WireDraw = true;
                m_WireStart = mouse_pos;
                event->accept();
            }
            else
            {
                QPoint mouse_pos = PointAlign( event->scenePos().toPoint() );
                GraphicsItem* item = ( GraphicsItem* )( itemAt( mouse_pos, ( ( QGraphicsView* )event->widget()->parentWidget() )->viewportTransform() ) );
                if( item != 0 )
                {
                    sendEvent( item, event );
                    event->accept();
                }
            }
        }
    }
}



void
GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
    // element mode
    if( m_WireDraw == false )
    {
        if( event->button() == Qt::LeftButton )
        {
            if( m_MovedItem.size() > 0 )
            {
                for( unsigned int i = 0; i < m_MovedItem.size(); ++i )
                {
                    removeItem( m_MovedItem[ i ] );
                    if( m_MovedItem[ i ]->GetType() == GraphicsItem::IT_WIRE )
                    {
                        ConnectWire( ( Wire* )m_MovedItem[ i ] );
                    }
                    else
                    {
                        ConnectElement( ( Element* )m_MovedItem[ i ] );
                    }
                }
                m_MovedHold = false;
                m_MovedItem.clear();
                UpdateAll();
            }
            else
            {
                QGraphicsItem* item = itemAt( event->scenePos(), ( ( QGraphicsView* )event->widget()->parentWidget() )->viewportTransform() );
                if( item != 0 )
                {
                    sendEvent( item, event );
                }
            }
        }
    }
    // wire draw mode
    else
    {
        if( event->button() == Qt::LeftButton )
        {
            m_WireDraw = false;
            if( m_WiresDraw.size() > 0 )
            {
                for( unsigned int i = 0; i < m_WiresDraw.size(); ++i )
                {
                    Wire* wire = new Wire();
                    wire->SetLine( m_WiresDraw[ i ] );
                    ConnectWire( wire );
                }
                m_WiresDraw.clear();
                m_WiresConDraw.clear();
                UpdateAll();
            }
        }
    }
}



void
GraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
    {
        QList< QGraphicsView* > view = views();
        QPoint origin = view[ 0 ]->mapFromGlobal( QCursor::pos() );
        QPointF relative_origin = view[ 0 ]->mapToScene( origin );
        //printf( "%f %f, %f %f\n", relative_origin.x(), relative_origin.y(), event->scenePos().x(), event->scenePos().y() );
    }

    if( m_WireDraw == false )
    {
        QList< QGraphicsView* > view = views();
        QRect rect = view[ 0 ]->rubberBandRect();
        if( rect == QRect() )
        {
            QList< QGraphicsItem* > select = selectedItems();
            if( select.size() == 0 )
            {
                // highlight contacts under mouse
                // this is point where we can start draw of wire
                QPoint mouse_pos = PointAlign( event->scenePos().toPoint() );
                std::vector< WireContact > contacts;
                CalculateLineElementsIntersect( QLine( mouse_pos, mouse_pos ), contacts );
                CalculateLineWiresIntersect( QLine( mouse_pos, mouse_pos ), contacts );
                m_WiresConDraw.clear();
                if( contacts.size() > 0 )
                {
                    m_WiresConDraw.push_back( mouse_pos );
                }
            }
            else
            {
                // send move to all selected element
                if( ( event->buttons() & Qt::LeftButton ) || ( m_MovedHold == true ) )
                {
                    QPointF offset = event->scenePos() - event->lastScenePos();
                    QPointF missed;
                    bool moved = false;

                    std::vector< GraphicsItem* > items;
                    for( unsigned int i = 0; i < select.size(); ++i )
                    {
                        QPointF old_pos = select[ i ]->pos();
                        QPointF req_pos = old_pos + m_MoveOffset + offset;
                        QPointF new_pos = PointAlign( req_pos.toPoint() );
                        if( old_pos != new_pos )
                        {
                            missed = req_pos - new_pos;
                            moved = true;
                            select[ i ]->setPos( new_pos );
                        }
                        if( std::find( m_MovedItem.begin(), m_MovedItem.end(), select[ i ] ) == m_MovedItem.end() )
                        {
                            items.push_back( ( GraphicsItem* )( select[ i ] ) );
                        }
                    }
                    if( items.size() > 0 )
                    {
                        DisconnectItems( items );
                        for( unsigned int i = 0; i < items.size(); ++i )
                        {
                            m_MovedItem.push_back( items[ i ] );
                            addItem( items[ i ] );
                        }
                    }
                    if( moved == false )
                    {
                        m_MoveOffset += offset;
                    }
                    else
                    {
                        UpdateAll();
                        m_MoveOffset = missed;
                    }
                }
            }
            QGraphicsScene::mouseMoveEvent( event );
        }
    }
    else
    {
        // calculate drawing wire and stop propagating
        // event to view to avoid selection box draw
        m_WireFinish = PointAlign( event->scenePos().toPoint() );
        CalculateWireDraw();
        event->accept();
    }
}



void
GraphicsScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event )
{
    if( event->buttons() & Qt::RightButton )
    {
        Container* container = m_CurrentContainer->GetContainer();
        if( container != NULL )
        {
            SetCurrentContainer( container );
        }
    }
    else if( event->buttons() & Qt::LeftButton )
    {
        Element* item = ( Element* )itemAt( event->scenePos(), ( ( QGraphicsView* )event->widget()->parentWidget() )->viewportTransform() );
        if( item != NULL )
        {
            if( item->GetType() == GraphicsItem::IT_CONTAINER )
            {
                SetCurrentContainer( ( Container* )item );
            }
        }
    }
}



void
GraphicsScene::keyPressEvent( QKeyEvent* event )
{
    QList< QGraphicsItem* > select = selectedItems();
    if( select.size() > 0 )
    {
        switch( event->key() )
        {
            case Qt::Key_R:
            {
                std::vector< GraphicsItem* > items;
                for( unsigned int i = 0; i < select.size(); ++i )
                {
                    if( ( ( GraphicsItem* )( select[ i ] ) )->GetType() != GraphicsItem::IT_WIRE )
                    {
                        select[ i ]->setRotation( select[ i ]->rotation() + 90 );
                        items.push_back( ( GraphicsItem* )( select[ i ] ) );
                    }
                }
                DisconnectItems( items );
                for( unsigned int i = 0; i < items.size(); ++i )
                {
                    ConnectElement( ( Element* )( items[ i ] ) );
                }
                UpdateAll();
            }
            break;

            case Qt::Key_Delete:
            {
                std::vector< GraphicsItem* > items;
                for( unsigned int i = 0; i < select.size(); ++i )
                {
                    items.push_back( ( GraphicsItem* )( select[ i ] ) );
                }
                DisconnectItems( items );
                for( unsigned int i = 0; i < items.size(); ++i )
                {
                    delete items[ i ];
                }
                UpdateAll();
            }
            break;

            case Qt::Key_C:
            {
                if( event->modifiers() & Qt::ControlModifier )
                {
                    for( unsigned int i = 0; i < m_CopiedItem.size(); ++i )
                    {
                        delete m_CopiedItem[ i ];
                    }
                    m_CopiedItem.clear();

                    for( unsigned int i = 0; i < select.size(); ++i )
                    {
                        if( ( ( GraphicsItem* )( select[ i ] ) )->GetType() == GraphicsItem::IT_WIRE )
                        {
                            Wire* wire = new Wire();
                            wire->SetLine( ( ( Wire* )( select[ i ] ) )->GetLine() );
                            wire->setPos( select[ i ]->pos() );
                            m_CopiedItem.push_back( wire );
                        }
                        else
                        {
                            Element* element = ( ( Element* )( select[ i ] ) )->Copy( m_CurrentContainer );
                            element->setPos( select[ i ]->pos() );
                            element->setRotation( select[ i ]->rotation() );
                            m_CopiedItem.push_back( element );
                        }
                    }
                }
            }
            break;
        }
    }

    switch( event->key() )
    {
        case Qt::Key_W:
        {
            m_WireMode = ( WireMode )( ( m_WireMode + 1 ) % ( int )WIRE_MODE_MAX );
            CalculateWireDraw();
        }
        break;

        case Qt::Key_V:
        {
            if( event->modifiers() & Qt::ControlModifier )
            {
                m_WiresConDraw.clear();
                clearSelection();

                for( unsigned int i = 0; i < m_CopiedItem.size(); ++i )
                {
                    if( ( ( GraphicsItem* )m_CopiedItem[ i ] )->GetType() == GraphicsItem::IT_WIRE )
                    {
                        Wire* wire = new Wire();
                        wire->SetLine( ( ( Wire* )m_CopiedItem[ i ] )->GetLine() );
                        wire->setPos( m_CopiedItem[ i ]->pos() + QPoint( 15, 15 ) );
                        m_MoveOffset = QPointF( 0, 0 );
                        m_MovedItem.push_back( wire );
                        wire->setSelected( true );
                        addItem( wire );
                    }
                    else
                    {
                        Element* element = ( ( Element* )m_CopiedItem[ i ] )->Copy( m_CurrentContainer );
                        element->setPos( m_CopiedItem[ i ]->pos() + QPoint( 15, 15 ) );
                        element->setRotation( m_CopiedItem[ i ]->rotation() );
                        m_MoveOffset = QPointF( 0, 0 );
                        m_MovedItem.push_back( element );
                        element->setSelected( true );
                        addItem( element );
                    }
                }
            }
        }
        break;
    }

    event->accept();
}



void
GraphicsScene::CalculateWireDraw()
{
    m_WiresDraw.clear();
    m_WiresConDraw.clear();

    if( m_WireStart != m_WireFinish )
    {
        if( m_WireMode == WIRE_DIAGONAL )
        {
            m_WiresDraw.push_back( QLine( m_WireStart, m_WireFinish ) );
        }
        else
        {
            if( m_WireStart.y() == m_WireFinish.y() )
            {
                m_WireMode = WIRE_HORIZONTAL;
            }
            if( m_WireStart.x() == m_WireFinish.x() )
            {
                m_WireMode = WIRE_VERTICAL;
            }

            if( m_WireMode == WIRE_HORIZONTAL )
            {
                QPoint p = QPoint( m_WireFinish.x(), m_WireStart.y() );
                m_WiresDraw.push_back( QLine( m_WireStart, p ) );
                if( m_WireStart.y() != m_WireFinish.y() )
                {
                    m_WiresDraw.push_back( QLine( p, QPoint( p.x(), m_WireFinish.y() ) ) );
                }
            }
            else if( m_WireMode == WIRE_VERTICAL )
            {
                QPoint p = QPoint( m_WireStart.x(), m_WireFinish.y() );
                m_WiresDraw.push_back( QLine( m_WireStart, p ) );
                if( m_WireStart.x() != m_WireFinish.x() )
                {
                    m_WiresDraw.push_back( QLine( p, QPoint( m_WireFinish.x(), p.y() ) ) );
                }
            }
        }

        // calculate intersection of drawing wire with elements and connects
        // this points where new wires will be connected
        std::vector< WireContact > contacts;
        for( unsigned int i = 0; i < m_WiresDraw.size(); ++i )
        {
            CalculateLineElementsIntersect( m_WiresDraw[ i ], contacts );
            CalculateLineWiresIntersect( m_WiresDraw[ i ], contacts );
        }
        for( unsigned int cont_id = 0; cont_id < contacts.size(); ++cont_id )
        {
            m_WiresConDraw.push_back( contacts[ cont_id ].pos );
        }
    }
    else
    {
        m_WiresConDraw.push_back( m_WireStart );
    }
}



void
GraphicsScene::CalculateWireElementsIntersect( Wire* wire, std::vector< WireContact >& contacts )
{
    QLine line = wire->GetLine();
    QPoint pos = wire->pos().toPoint();
    line.translate( pos );
    CalculateLineElementsIntersect( line, contacts );
}



void
GraphicsScene::CalculateWireWiresIntersect( Wire* wire, std::vector< WireContact >& contacts )
{
    QLine line = wire->GetLine();
    QPoint pos = wire->pos().toPoint();
    line.translate( pos );
    CalculateLineWiresIntersect( line, contacts, wire );
}



void
GraphicsScene::CalculateLineElementsIntersect( const QLine& line, std::vector< WireContact >& contacts )
{
    std::vector< Element* > elements = m_CurrentContainer->GetElements();
    for( unsigned int i = 0; i < elements.size(); ++i )
    {
        std::vector< Element::Contact > cont = elements[ i ]->GetContacts();
        QTransform transform;
        transform.rotate( elements[ i ]->rotation() );
        QPoint pos = elements[ i ]->pos().toPoint();
        for( unsigned int j = 0; j < cont.size(); ++j )
        {
            QPoint contact = transform.map( cont[ j ].point ) + pos;
            if( IsLinePointIntersect( line, contact ) == true )
            {
                WireContact cont;
                cont.pos = contact;
                cont.element_id = i;
                cont.contact_id = j;
                contacts.push_back( cont );
            }
        }
    }
}



void
GraphicsScene::CalculateLineWiresIntersect( const QLine& line, std::vector< WireContact >& contacts, Wire* wire )
{
    std::vector< Wire* > wires = m_CurrentContainer->GetWires();
    for( unsigned int i = 0; i < wires.size(); ++i )
    {
        if( wire != wires[ i ] )
        {
            QLine line2 = wires[ i ]->GetLine();
            QPoint pos = wires[ i ]->pos().toPoint();
            line2.translate( pos );
            QPoint l1p1 = line.p1();
            QPoint l1p2 = line.p2();
            QPoint l2p1 = line2.p1();
            QPoint l2p2 = line2.p2();
            if( IsLinePointIntersect( line2, l1p1 ) == true )
            {
                WireContact cont;
                cont.pos = l1p1;
                cont.wire_id = i;
                cont.wire_point = ( l2p2 == l1p1 ) ? 2 : ( ( l2p1 == l1p1 ) ? 1 : 0 );
                cont.self_point = 1;
                contacts.push_back( cont );
            }
            else if( IsLinePointIntersect( line2, l1p2 ) == true )
            {
                WireContact cont;
                cont.pos = l1p2;
                cont.wire_id = i;
                cont.wire_point = ( l2p2 == l1p2 ) ? 2 : ( ( l2p1 == l1p2 ) ? 1 : 0 );
                cont.self_point = 2;
                contacts.push_back( cont );
            }
            else if( IsLinePointIntersect( line, l2p1 ) == true )
            {
                WireContact cont;
                cont.pos = l2p1;
                cont.wire_id = i;
                cont.wire_point = 1;
                cont.self_point = ( l1p2 == l2p1 ) ? 2 : ( ( l1p1 == l2p1 ) ? 1 : 0 );
                contacts.push_back( cont );
            }
            else if( IsLinePointIntersect( line, l2p2 ) == true )
            {
                WireContact cont;
                cont.pos = l2p2;
                cont.wire_id = i;
                cont.wire_point = 2;
                cont.self_point = ( l1p2 == l2p2 ) ? 2 : ( ( l1p1 == l2p2 ) ? 1 : 0 );
                contacts.push_back( cont );
            }
        }
    }
}



void
GraphicsScene::CalculateElementWiresIntersect( Element* element, std::vector< WireContact >& contacts )
{
    QTransform transform;
    transform.rotate( element->rotation() );
    std::vector< Element::Contact > cont = element->GetContacts();
    QPoint pos = element->pos().toPoint();
    for( unsigned int i = 0; i < cont.size(); ++i )
    {
        std::vector< Wire* > wires = m_CurrentContainer->GetWires();
        QPoint contact = transform.map( cont[ i ].point ) + pos;
        for( unsigned int j = 0; j < wires.size(); ++j )
        {
            QLine check_wire = wires[ j ]->GetLine();
            QPoint pos = wires[ j ]->pos().toPoint();
            check_wire.translate( pos );
            if( IsLinePointIntersect( check_wire, contact ) == true )
            {
                WireContact cont;
                cont.self_contact_id = i;
                cont.wire_id = j;
                cont.wire_point = ( check_wire.p2() == contact ) ? 2 : ( ( check_wire.p1() == contact ) ? 1 : 0 );
                contacts.push_back( cont );
            }
        }
    }
}



void
GraphicsScene::CalculateElementElementsIntersect( Element* element, std::vector< WireContact >& contacts )
{
    QTransform transform;
    transform.rotate( element->rotation() );
    std::vector< Element::Contact > cont = element->GetContacts();
    QPoint pos = element->pos().toPoint();
    for( unsigned int i = 0; i < cont.size(); ++i )
    {
        std::vector< Element* > elements = m_CurrentContainer->GetElements();
        QPoint contact = transform.map( cont[ i ].point ) + pos;
        for( unsigned int j = 0; j < elements.size(); ++j )
        {
            if( elements[ j ] != element )
            {
                QTransform el_trans;
                el_trans.rotate( elements[ j ]->rotation() );
                std::vector< Element::Contact > el_cont = elements[ j ]->GetContacts();
                QPoint el_pos = elements[ j ]->pos().toPoint();
                for( unsigned int k = 0; k < el_cont.size(); ++k )
                {
                    QPoint el_contact = el_trans.map( el_cont[ k ].point ) + el_pos;
                    if( el_contact == contact )
                    {
                        WireContact cont;
                        cont.element_id = j;
                        cont.contact_id = k;
                        cont.self_contact_id = i;
                        contacts.push_back( cont );
                    }
                }
            }
        }
    }
}



bool
GraphicsScene::IsLinePointIntersect( const QLine& line, const QPoint& point )
{
    qreal x1 = line.x1();
    qreal x2 = line.x2();
    qreal y1 = line.y1();
    qreal y2 = line.y2();
    bool diagonal = ( x1 != x2 ) && ( y1 != y1 );

    int x = point.x();
    int y = point.y();

    if( diagonal == false )
    {
        int a = y2 - y1;
        int b = x1 - x2;
        int c = - a * x1 - b * y1;
        if( std::abs( a * x + b * y + c ) > 1 )
        {
            return false;
        }
        else
        {
            return ( ( x >= x1 && x <= x2 ) || ( x <= x1 && x >= x2 ) ) && ( ( y >= y1 && y <= y2 ) || ( y <= y1 && y >= y2 ) );
        }
    }
    else
    {
        return ( x == x1 && y == y1 ) || ( x == x2 && y == y2 );
    }
}




QPoint
GraphicsScene::PointAlign( const QPoint& pos )
{
    QPoint res = pos;
    int rem = pos.x() % 15;
    if( rem > 7 )
    {
        res.rx() += 15 - rem;
    }
    else if( rem < -7 )
    {
        res.rx() -= 15 + rem;
    }
    else
    {
        res.rx() -= rem;
    }
    rem = pos.y() % 15;
    if( rem > 7 )
    {
        res.ry() += 15 - rem;
    }
    else if( rem < -7 )
    {
        res.ry() -= 15 + rem;
    }
    else
    {
        res.ry() -= rem;
    }
    return res;
}

#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <vector>
#include <QGraphicsScene>

class Container;
class Connect;
class GraphicsItem;
class Element;
class Wire;



class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene( QObject* parent );
    virtual ~GraphicsScene();

    void RemoveAll();

    void ConnectElement( Element* element );
    void ConnectWire( Wire* wire );
    Wire* MergeWire( Wire* wire );
    void DisconnectItems( std::vector< GraphicsItem* >& items );
    void InsertElement( GraphicsItem* element );

    void AddContainerDef( Container* container );

    void DoStep();
    void UpdateAll();

    void SetCurrentContainer( Container* container );
    Container* GetCurrentContainer();

    void InsertContainer();
    void InsertPin();
    void InsertGround();
    void InsertPower();
    void InsertNfet();
    void InsertPfet();

    struct WireContact
    {
        QPoint pos;
        int element_id;
        int contact_id;
        int self_contact_id;
        int wire_id;
        int wire_point; // 0 - in middle, 1 - start, 2 - end
        int self_point; // 0 - in middle, 1 - start, 2 - end
    };

protected:
    void drawBackground( QPainter* painter, const QRectF& rect );
    void drawForeground( QPainter* painter, const QRectF& rect );

    void mousePressEvent( QGraphicsSceneMouseEvent* event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event );
    void keyPressEvent( QKeyEvent* event );

    void CalculateWireDraw();
    void CalculateWireElementsIntersect( Wire* wire, std::vector< WireContact >& contacts );
    void CalculateWireWiresIntersect( Wire* wire, std::vector< WireContact >& contacts );
    void CalculateLineElementsIntersect( const QLine& line, std::vector< WireContact >& contacts );
    void CalculateLineWiresIntersect( const QLine& line, std::vector< WireContact >& contacts, Wire* wire = 0 );
    void CalculateElementWiresIntersect( Element* element, std::vector< WireContact >& contacts );
    void CalculateElementElementsIntersect( Element* element, std::vector< WireContact >& contacts );
    bool IsLinePointIntersect( const QLine& line, const QPoint& point );

    QPoint PointAlign( const QPoint& pos );

private:
    Container* m_RootContainer;
    Container* m_CurrentContainer;
    std::vector< Container* > m_ContainerDefs;

    enum WireMode
    {
        WIRE_VERTICAL,
        WIRE_HORIZONTAL,
        WIRE_DIAGONAL,
        WIRE_MODE_MAX
    };
    bool m_WireDraw;
    WireMode m_WireMode;
    QPoint m_WireStart;
    QPoint m_WireFinish;
    std::vector< QLine > m_WiresDraw;
    std::vector< QPoint > m_WiresConDraw;
    bool m_MoveStart;
    QPointF m_MoveOffset;
    bool m_MovedHold;
    std::vector< GraphicsItem* > m_MovedItem;
    std::vector< GraphicsItem* > m_CopiedItem;
};



extern GraphicsScene* test_circuit;



#endif // GRAPHICS_SCENE_H

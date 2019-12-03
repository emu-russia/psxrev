#ifndef GRAPHIC_ITEM_H
#define GRAPHIC_ITEM_H

#include <QGraphicsItem>
#include <QColor>

class GraphicsScene;

#define SELECT_COLOR QColor( 0, 0, 255, 100 )
#define CONNECT_COLOR QColor( 178, 0, 0 )
#define WIRE_LOW_COLOR QColor( 0, 142, 0 )
#define WIRE_HIGH_COLOR QColor( 102, 255, 102 )
#define WIRE_Z_COLOR QColor( 128, 128, 128 )
#define WIRE_DRAW_COLOR QColor( 200, 200, 200 )



class GraphicsItem : public QGraphicsItem
{
public:
    GraphicsItem();
    virtual ~GraphicsItem();

    QRectF boundingRect() const override = 0;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override = 0;

    enum ItemType
    {
        IT_WIRE,
        IT_PIN,
        IT_GROUND,
        IT_POWER,
        IT_NFET,
        IT_PFET,
        IT_CONTAINER
    };
    ItemType GetType() const;

protected:
    ItemType m_ItemType;
};



#endif // GRAPHIC_ITEM_H

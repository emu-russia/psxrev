#ifndef CONTAINER_H
#define CONTAINER_H

#include "Element.h"

#include <vector>

class GraphicsScene;



class Container : public Element
{
public:
    Container( Container* parent );
    virtual ~Container();

    Element* Copy();

    QRectF boundingRect() const override;
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* widget ) override;

    void Update();
    void Trace( Line* line, Connect* connect );
    void SetValue( const int value, Connect* connect );

    void SetScene( GraphicsScene* scene );
    void UnsetScene();

    void InsertContainer();

private:
    std::vector< Element* > m_Elements;

    GraphicsScene* m_Scene;

    int counter;
};



#endif // CONTAINER_H

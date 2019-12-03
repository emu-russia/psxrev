#ifndef ELEMENT_H
#define ELEMENT_H

#include "../gui/GraphicsItem.h"

class Connect;
class Container;
class Line;



class Element : public GraphicsItem
{
public:
    struct Contact
    {
        QPoint point;
        Connect* connect;
    };

    Element( Container* parent );
    virtual ~Element();

    virtual Element* Copy() = 0;

    virtual void Update() = 0;
    virtual void Trace( Line* line, Connect* connect ) = 0;
    virtual void SetValue( const int value, Connect* connect ) = 0;
    bool IsVisited();

    void AddConnect( Connect* connect, const unsigned int cont_id );
    void UpdateConnect( Connect* old_c, Connect* new_c );
    void RemoveConnect( Connect* connect );
    void ClearConnects();
    std::vector< Contact >& GetContacts();

    Container* GetContainer();

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent* event ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* event ) override;
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event ) override;

protected:
    bool m_Visited;
    std::vector< Contact > m_Contacts;

    Container* m_Container;
};



#endif // ELEMENT_H

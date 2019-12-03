#include "GraphicsItem.h"

#include "GraphicsScene.h"

#include <QtWidgets>



GraphicsItem::GraphicsItem()
{
    setFlags( ItemIsSelectable | ItemIsMovable | ItemIsFocusable | ItemSendsGeometryChanges );
}



GraphicsItem::~GraphicsItem()
{
}



GraphicsItem::ItemType
GraphicsItem::GetType() const
{
    return m_ItemType;
}

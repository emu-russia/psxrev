#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H

#include <QGraphicsView>

class View;



class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView( View *v );

    void ResetView();
    void SetupMatrix();
    void Scale( const qreal scale );

protected:
    void wheelEvent( QWheelEvent* event ) override;
    void mousePressEvent( QMouseEvent* event ) override;
    void mouseReleaseEvent( QMouseEvent* event ) override;
    void mouseMoveEvent( QMouseEvent *event ) override;
    void mouseDoubleClickEvent( QMouseEvent* event ) override;

private:
    View* m_View;

    qreal m_Scale;
    QPoint m_Translate;
    QPoint m_PrevTrans;
};



#endif // GRAPHICS_VIEW_H

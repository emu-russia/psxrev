#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include "GraphicsView.h"

class QLabel;
class QSlider;
class QToolButton;



class View : public QFrame
{
    Q_OBJECT

public:
    explicit View( QWidget *parent = 0 );

    QGraphicsView* view() const;

public slots:
    void ScaleBySlider();
    void ScaleToSlider( const qreal scale );
    void ZoomIn();
    void ZoomOut();

private:
    GraphicsView* graphicsView;
    QLabel* label;
    QToolButton* resetButton;

    QSlider* m_ZoomSlider;
};



#endif // VIEW_H

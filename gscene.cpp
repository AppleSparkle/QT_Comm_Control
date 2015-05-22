#include "gscene.h"

GScene::GScene()
{
    this->setBackgroundBrush(Qt::black);
}

GScene::~GScene()
{

}

void GScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

void GScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::RightButton)
    {        
        emit zoom_rect(this->selectionArea());
        return;
    }

}

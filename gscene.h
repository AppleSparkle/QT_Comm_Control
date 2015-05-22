#ifndef GSCENE_H
#define GSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsItem>

#include <QPainter>
#include <QPainterPath>

#include <QStyleOptionGraphicsItem>

#include <QPolygonF>


class GScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GScene();
    ~GScene();

signals:
    void zoom_rect(QPainterPath zRect);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // GSCENE_H

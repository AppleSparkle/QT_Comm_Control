#ifndef DRAWING_H
#define DRAWING_H

#include <QGraphicsScene>
#include <QGraphicsItem>

#include <QtMath>

#include <parser.h>

#include "repair_data.h"

class draw
{
public:
    draw();
    ~draw();

    void draw_all(QGraphicsScene * _scene, parser * _parser,
                          QList <int> * item_match, QList <int> *contour_match,
                  QList <QGraphicsPathItem*> * itemlist);

    void draw_all(QGraphicsScene * _scene, repair_data * _rep);

    QList <QPointF> transfer_elements(QGraphicsScene *source, repair_data *_repair_data);

    void hideD02(QGraphicsScene * _scene);
    void showD02(QGraphicsScene * _scene);

private:

    QPen blackpen;
    QPen redpen;

};

#endif // DRAWING_H

#include "draw.h"


draw::draw()
{
    blackpen.setWidth(50);
    redpen.setWidth(50);
    blackpen.setColor(Qt::white);
    redpen.setColor(Qt::red);
}

draw::~draw()
{

}

void draw::hideD02(QGraphicsScene * _scene)
{
    foreach (QGraphicsItem * itt, _scene->items())
    {


        if( QGraphicsPathItem * path = qgraphicsitem_cast <QGraphicsPathItem *>(itt))
        {
            if(  path->pen().color() == blackpen.color() )
            {
                path->setPen(_scene->backgroundBrush().color());
                itt->setZValue(0);
               }

        }
    }
}

void draw::showD02(QGraphicsScene * _scene)
{
    foreach (QGraphicsItem * itt, _scene->items())
    {
        if( QGraphicsPathItem * path = qgraphicsitem_cast <QGraphicsPathItem *>(itt))
        {
            if(  path->pen().color() == _scene->backgroundBrush().color() )
                path->setPen(blackpen);


        }
    }
}

void draw::draw_all(QGraphicsScene * _scene, parser * _parser,
                    QList<int> *item_match, QList<int> *contour_match,
                    QList<QGraphicsPathItem *> *itemlist)
{
    _scene->clear();
    _scene->update();

    QPainterPath * path = new QPainterPath();

    QPolygonF poly;

    int itemcount = 0;
    int ccount = 0;

    _parser->D01_count = 0;
    _parser->D02_count = 0;

    _parser->d01_pathlength = 0;
    _parser->d02_pathlength = 0;

    poly << QPointF(_parser->x_val_array.at(0), _parser->y_val_array.at(0));

    *item_match<<0;
    *contour_match<<0;

    for (int i=1; i<_parser->x_val_array.length(); i++)
    {
        if(_parser->d_val_array.at(i) != _parser->d_val_array.at(i-1))
        {
            if(_parser->d_val_array.at(i-1) == 2)
            {
                path->addPolygon(poly);
                *itemlist << _scene->addPath(*path,blackpen);
                poly.clear();
                *path = QPainterPath();

                _parser->D02_count++;

                poly<<QPointF(_parser->x_val_array.at(i-1), _parser->y_val_array.at(i-1));

                itemcount++;
                *item_match<<itemcount;    //itemnum
                *contour_match<<ccount;
            }
            else
            {
                path->addPolygon(poly);
                *itemlist << _scene->addPath(*path,redpen);
                poly.clear();
                *path = QPainterPath();

                itemlist->last()->setFlag(QGraphicsItem::ItemIsSelectable, true);

                _parser->D01_count++;

                poly<<QPointF(_parser->x_val_array.at(i-1), _parser->y_val_array.at(i-1));

                itemcount++;
                *item_match<<itemcount;    //itemnum
                ccount++;
                *contour_match<<ccount;
            }
        }
        else
        {
            *item_match<<itemcount;    //itemnum
            *contour_match<<ccount;
        }

        poly << QPointF(_parser->x_val_array.at(i), _parser->y_val_array.at(i));

        if(_parser->d_val_array.at(i) == 2)
        {
            _parser->d02_pathlength += sqrt( pow((_parser->x_val_array.at(i-1)
                                                - _parser->x_val_array.at(i)),2) +
                                             pow((_parser->y_val_array.at(i-1)
                                                - _parser->y_val_array.at(i)),2)
                                            );
        }
        else if (_parser->d_val_array.at(i) == 1)
        {
            _parser->d01_pathlength += sqrt( pow((_parser->x_val_array.at(i-1)
                                                - _parser->x_val_array.at(i)),2) +

                                             pow((_parser->y_val_array.at(i-1)
                                                - _parser->y_val_array.at(i)),2)
                                            );
        }

    }

    if(_parser->d_val_array.last() == 2)
    {

        path->addPolygon(poly);
        *itemlist << _scene->addPath(*path,blackpen);
        poly.clear();
        *path = QPainterPath();


        _parser->D02_count++;
    }
    else
    {
        path->addPolygon(poly);
        *itemlist << _scene->addPath(*path,redpen);
        poly.clear();
        *path = QPainterPath();

        _parser->D01_count++;
    }

    delete path;

}

void draw::draw_all(QGraphicsScene * _scene, repair_data * _rep)
{
    QPainterPath * path = new QPainterPath();

    QPolygonF poly;

    poly << QPointF(_rep->x_val_array.at(0), _rep->y_val_array.at(0));

    for (int i=1; i<_rep->x_val_array.length(); i++)
    {
        if(_rep->d_val_array.at(i) != _rep->d_val_array.at(i-1))
        {
            if(_rep->d_val_array.at(i-1) == 2)
            {
                path->addPolygon(poly);
                _scene->addPath(*path,blackpen);
                poly.clear();
                *path = QPainterPath();

                poly<<QPointF(_rep->x_val_array.at(i-1), _rep->y_val_array.at(i-1));
            }
            else
            {
                path->addPolygon(poly);
                _scene->addPath(*path,redpen);
                poly.clear();
                *path = QPainterPath();

                poly<<QPointF(_rep->x_val_array.at(i-1), _rep->y_val_array.at(i-1));
            }
        }
        else
        {
        }

        poly << QPointF(_rep->x_val_array.at(i), _rep->y_val_array.at(i));

    }

    if(_rep->d_val_array.last() == 2)
    {

        path->addPolygon(poly);
        _scene->addPath(*path,blackpen);
        poly.clear();
        *path = QPainterPath();
    }
    else
    {
        path->addPolygon(poly);
        _scene->addPath(*path,redpen);
        poly.clear();
        *path = QPainterPath();
    }

    delete path;

    QList <QGraphicsItem * > list = _scene->items();
    QPen pen(Qt::red);

    pen.setWidth(1/10);

    foreach( QGraphicsItem * item, list )
    {
        if( QGraphicsPathItem * path = qgraphicsitem_cast <QGraphicsPathItem *>(item))
        {
            pen.setColor( path->pen().color() );
            path->setPen(pen);
        }
    }
}

QList <QPointF> draw::transfer_elements( QGraphicsScene * source, repair_data * _repair_data)
{
    QPen blackpen(Qt::black);
    blackpen.setWidth(50);

    QPen redpen(Qt::red);
    redpen.setWidth(50);

    QList <QGraphicsItem *> listCopiedItems = source->selectedItems();
    QList <QGraphicsItem *> listSortedItems;
    QList <QGraphicsItem *> listGreedySortedItems;

/*    for(int i=0; i< listCopiedItems.length(); i++)
    {
        sink->addItem(listCopiedItems.at(i));
    }
*/
    //Sorting

    //Find element closest to zero x,y = (0,0)

    listSortedItems.append(listCopiedItems.at(0));

    for(int i=1; i< listCopiedItems.length(); i++)
    {
       QPointF point = listCopiedItems.at(i)->shape().pointAtPercent(0);
       QPointF point_prev = listSortedItems.at(0)->shape().pointAtPercent(0);

       if (sqrt( pow(point.x(),2) + pow(point.y(),2)) <
                                            sqrt( pow(point_prev.x(),2) + pow(point_prev.y(),2)))
       {
           listSortedItems.prepend(listCopiedItems.at(i));
       }
       else
       {
           listSortedItems.append(listCopiedItems.at(i));
       }

    }

    //Execute greedy algorithm


    QList<int> dist;
    QList<int> dist_num;
    int dist_min;

    listGreedySortedItems.prepend(listSortedItems.at(0));
    listSortedItems.removeAt(0);

    while(!listSortedItems.isEmpty())
    {
        dist_min = 100000000;
        dist.clear();
        dist_num.clear();
        int j = 0;

        foreach(QGraphicsItem * itt, listSortedItems)
        {
            QPointF p_point = listGreedySortedItems.last()->shape().pointAtPercent(0);
            QPointF point   = listSortedItems.at(j)->shape().pointAtPercent(0);

            dist.append(     sqrt(    pow(   (point.x() - p_point.x()) ,2)
                                   +  pow(   (point.y() - p_point.y()) ,2)
                                ));
            dist_num.append(j);
            j++;
        }

        for(int k = 0; k<dist.length(); k++)
        {
            if(dist.at(k)<dist_min) dist_min = dist.at(k);
        }

        for(int n=0; n<dist.length(); n++)
        {
            if(dist.at(n) == dist_min)
            {
                listGreedySortedItems.append(listSortedItems.at( dist_num.at(n) ));
                listSortedItems.removeAt(dist_num.at(n));
                break;
            }
        }

    }


    //Draw all

    QList <QPointF> list_out;

//    QPolygonF p_d2;
//    QPainterPath add;

    _repair_data->x_val_array.clear();
    _repair_data->y_val_array.clear();
    _repair_data->d_val_array.clear();
    _repair_data->g_val_array.clear();

    for(int i=0; i< listGreedySortedItems.length(); i++)
    {
/*        if(i>0)
        {
                poly<<QPointF(listGreedySortedItems.at(i-1)->shape().pointAtPercent(1));
                poly<<QPointF(listGreedySortedItems.at(i)->shape().pointAtPercent(0));

                path_new.addPolygon(poly);
                sink->addPath(path_new,blackpen);
                poly.clear();

         }
        else if (i==0)
        {
            poly<<QPointF(0,0);
            poly<<QPointF(listGreedySortedItems.at(i)->shape().pointAtPercent(0));

            path_new.addPolygon(poly);
            sink->addPath(path_new,blackpen);
            poly.clear();
        }
*/

        QGraphicsPathItem * path__ =
                qgraphicsitem_cast <QGraphicsPathItem *>(listGreedySortedItems.at(i));

        QPainterPath p_out = path__->path();
        QPainterPath::Element el;

        int N = p_out.elementCount();

        for(int j=0; j<N; j++)
        {
            QPointF point_t;

            el = p_out.elementAt(j);
            point_t.setX(el.x);
            point_t.setY(el.y);

            list_out.append(point_t);

            _repair_data->x_val_array.append(el.x);
            _repair_data->y_val_array.append(el.y);

            if (j == 0 )    _repair_data->d_val_array.append(2);
            else            _repair_data->d_val_array.append(1);

        }

/*        if (i>0)
        {
            p_d2<<list_out.at(list_out.count()-2);
            p_d2<<list_out.last();
            add.addPolygon(p_d2);
            p_d2.clear();
            sink->addPath(add,blackpen);
        }
        else
        {
            p_d2<<QPointF(0,0);
            p_d2<<list_out.last();
            add.addPolygon(p_d2);
            p_d2.clear();
            sink->addPath(add,blackpen);
        }
*/


    }

    return list_out;


}

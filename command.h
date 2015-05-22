#ifndef COMMAND_H
#define COMMAND_H

#include <QByteArray>
#include <QList>

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QGraphicsPolygonItem>

#include <QTcpSocket>
#include <QAbstractSocket>

#include "parser.h"
#include "tcp_client.h"

class command
{

public:
    command();
    ~command();

    QByteArray FormCommand(char cmd, int var1, int var2);

    bool RecieveCommand(QByteArray message);

    QByteArray CMD;
    QByteArray LEN;
    QByteArray DAT;

    int bytecount;

    int paint (QList<QGraphicsPathItem *> *itemlist, QList<int> *matchlist, QByteArray *num, int max);

    void Send_Program(parser * _parser, tcp_client * _client);

private:
    int state;
    int DAT_count;
    QByteArray cmd_receiveBuf;

    unsigned short Fletcher16( QByteArray * data);


};

#endif // COMMAND_H

#include "command.h"

command::command()
{

}

command::~command()
{

}

QByteArray command::FormCommand(char cmd, int var1, int var2)
{
    QByteArray comm_array;
    unsigned short chk;

    comm_array.append(cmd);
    comm_array.append(8);

    comm_array.append((var1&0xFF000000)>>24);	//X
    comm_array.append((var1&0x00FF0000)>>16);
    comm_array.append((var1&0x0000FF00)>>8);
    comm_array.append(var1&0x000000FF);

    comm_array.append((var2&0xFF000000)>>24);	//Y
    comm_array.append((var2&0x00FF0000)>>16);
    comm_array.append((var2&0x0000FF00)>>8);
    comm_array.append(var2&0x000000FF);

    chk = Fletcher16(&comm_array);

    comm_array.append((chk&0xFF00)>>8);
    comm_array.append(chk&0x00FF);

    return comm_array;
}

void command::Send_Program(parser * _parser, tcp_client * _client)
{
    for (int i = 0; i < _parser->x_val_array.count() ; i++)
    {
        QByteArray qb;
        qb = FormCommand(0x1F, _parser->x_val_array.at(i), _parser->y_val_array.at(i));
        _client->write_data(qb);
    }
}

bool command::RecieveCommand(QByteArray message)    //return true if command recieved, false is not
{
    int length;

    bytecount = 0;

    length = message.length();

    //cmd_receiveBuf.append(message);


    //handle response

    while (length > 0)
    {
        switch (state)
            {
                case 0:			//	Wait state
                {
                    if (length > 0) state = 1;
                    break;
                }
                case 1:			//	Get command
                {
                    CMD[0] = message.at(bytecount);
                    bytecount++;
                    length--;
                    state = 2;
                    break;
                }
                case 2:			//	Get data length
                {
                    LEN[0] = message.at(bytecount);
                    bytecount++;
                    length--;
                    DAT_count = 0;
                    state = 3;
                    break;
                }
                case 3:			//	Fill data to length
                {
                    DAT[DAT_count] = message.at(bytecount);
                    DAT_count++;
                    bytecount++;
                    length--;
                    if (DAT_count == LEN[0])	//	Done - (check control sum) and (execute command)
                        {
                            DAT_count = 0;
                            state = 0;

                            return 1;

                            break;
                        }
                    else{break;}
                }
                default:
                {
                    state = 1;
                }
            }

    }

    return 0;
}

int command::paint (QList <QGraphicsPathItem*> * itemlist, QList <int> * matchlist, QByteArray * num, int max)
{
    QGraphicsPathItem *item = itemlist->at(0);

    QPen pen(Qt::blue);
    pen.setWidth(50);

    QDataStream stream(*num);
    int temp;
    stream >> temp;

    if (temp > max) temp = max;

    for (int i = 0; i<temp; i++)
    {
        item = itemlist->at(matchlist->at(i));
        item->setPen(pen);
    }

    return temp;
}

unsigned short command::Fletcher16( QByteArray * data)
{
    int i = 0;
    unsigned short sum1 = 0xff;
    unsigned short sum2 = 0xff;
    int count = data->count();

    while (count)
    {
        int tlen = count > 20 ? 20 : count ;
        count -= tlen;
        do {
                sum2 += sum1 += data->at(i++);
        } while (--tlen);
        sum1 = (sum1 & 0xff) + (sum1 >> 8);
        sum2 = (sum2 & 0xff) + (sum2 >> 8);
    }

    /* Second reduction step to reduce sums to 8 bits */

    sum1 = (sum1 & 0xff) + (sum1 >> 8);
    sum2 = (sum2 & 0xff) + (sum2 >> 8);
    return sum2 << 8 | sum1;

}

/*void command::Send_Confing_Parameter(QTcpSocket * client, int value)
{
    QByteArray ba((const char*)&value, sizeof(value));

    client->write(ba);
}

void command::Send_Confing_Parameter(QTcpSocket * client, double value)
{
    float d = (float) value;
    QByteArray ba((const char*)&d, sizeof(d));

    client->write(ba);
}*/

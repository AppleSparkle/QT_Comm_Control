#ifndef PLC_OMRON_H
#define PLC_OMRON_H

#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>
#include <QTcpSocket>

class plc_omron : public QObject
{
    Q_OBJECT

public:
    plc_omron();
    ~plc_omron();

    bool connect_plc(QString portname, int baudrate);

    void plc_run();
    void plc_stop();

public slots:
    void read_data();

private:

    QSerialPort serialPort_plc;

};

#endif // PLC_OMRON_H

#include "plc_omron.h"

plc_omron::plc_omron()
{
    serialPort_plc.setDataBits(QSerialPort::Data8);
    serialPort_plc.setStopBits(QSerialPort::OneStop);
    serialPort_plc.setParity(QSerialPort::NoParity);
    serialPort_plc.setFlowControl(QSerialPort::NoFlowControl);

    connect(&serialPort_plc, SIGNAL(readyRead()), this, SLOT(read_data()));
}

plc_omron::~plc_omron()
{

}

bool plc_omron::connect_plc(QString portname, int baudrate)
{

    if (serialPort_plc.isOpen())
    {
        serialPort_plc.close();
        return true;
    }

    serialPort_plc.setPortName(portname);
    serialPort_plc.setBaudRate(baudrate);
    serialPort_plc.setDataBits(QSerialPort::Data8);
    serialPort_plc.setStopBits(QSerialPort::OneStop);
    serialPort_plc.setParity(QSerialPort::NoParity);
    serialPort_plc.setFlowControl(QSerialPort::NoFlowControl);

    if(serialPort_plc.open(QIODevice::ReadWrite))
    {
        QByteArray resp;
        while (true)
        {
            QByteArray qb;
            qb[0] = 0xAC;
            qb[1] = 0x01;
            serialPort_plc.write(qb);
            serialPort_plc.waitForBytesWritten(1000);
            resp = serialPort_plc.readAll();
            serialPort_plc.waitForReadyRead(100);
            if (resp[0] == (char) 0xAC) break;
        }

        return true;
    }
    else
    {
        return false;
    }

}

void plc_omron::read_data()
{

}

void plc_omron::plc_run()
{

    if (!serialPort_plc.isOpen()) return;
    else
    {
        QByteArray qb;
        qb[0] = 0xAB;
        qb[1] = 0x00;
        qb[2] = 0x0E;
        qb[3] = 0x80;
        qb[4] = 0x00;
        qb[5] = 0x03;
        qb[6] = 0x00;
        qb[7] = 0x00;
        qb[8] = 0x00;
        qb[9] = 0x00;
        qb[10] = 0x00;
        qb[11] = 0x00;
        qb[12] = 0x00;
        qb[13] = 0x04;
        qb[14] = 0x01;
        qb[15] = 0x01;
        qb[16] = 0x41;

        serialPort_plc.write(qb);
    }
}

void plc_omron::plc_stop()
{
    if (!serialPort_plc.isOpen()) return;
    else
    {
        QByteArray qb;
        qb[0] = 0xAB;
        qb[1] = 0x00;
        qb[2] = 0x0E;
        qb[3] = 0x80;
        qb[4] = 0x00;
        qb[5] = 0x03;
        qb[6] = 0x00;
        qb[7] = 0x00;
        qb[8] = 0x00;
        qb[9] = 0x00;
        qb[10] = 0x00;
        qb[11] = 0x00;
        qb[12] = 0x00;
        qb[13] = 0x04;
        qb[14] = 0x02;
        qb[15] = 0x01;
        qb[16] = 0x42;

        serialPort_plc.write(qb);
    }
}

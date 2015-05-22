#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QObject>

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>

#include <QMessageBox>

class tcp_client : public QObject
{
    Q_OBJECT
public:
    explicit tcp_client(QObject *parent = 0);
    ~tcp_client();

    bool connect_client(QHostAddress address, int port);
    void close_client();
    void write_data(QByteArray data);

signals:

    void tcp_recd(QByteArray recieved_data);

private slots:

    void client_data_recieve();

private:

    QTcpSocket client;

};

#endif // TCP_CLIENT_H

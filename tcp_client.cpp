#include "tcp_client.h"

tcp_client::tcp_client(QObject *parent) : QObject(parent)
{

}

tcp_client::~tcp_client()
{

}

bool tcp_client::connect_client(QHostAddress address, int port)
{

    client.connectToHost(address, port);

    if (!client.waitForConnected(1000))
    {
        QMessageBox(QMessageBox::Warning, "Could not establish connection!", "Timed out.");
    }

    if (client.isOpen())
    {
        connect(
                    &client,
                    SIGNAL( readyRead() ),
                    this,
                    SLOT( client_data_recieve() )
                );

        return true;

    }
    else return false;
}

void tcp_client::close_client()
{
    client.close();
}

void tcp_client::client_data_recieve()
{
    QByteArray recieved_data;

    recieved_data = client.readAll();

    emit tcp_recd(recieved_data);
}

void tcp_client::write_data(QByteArray data)
{
    client.write(data);
    client.waitForBytesWritten(1000);
}

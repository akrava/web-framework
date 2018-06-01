#pragma once
#include <QString>
#include <QTcpSocket>

class Client {
    QTcpSocket * socket;
    QString err;
public:
    Client();
    ~Client();
    QString what();
    bool setupConnection(QHostAddress & address, int port);
    bool sendData(QString & data);
    bool reciveData(QString & data);
    void closeConnection();
};

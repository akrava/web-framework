#include "client.h"
#include <QHostAddress>

Client::Client() {
    socket = nullptr;
}

Client::~Client() {
    if (socket) delete socket;
}

QString Client::what() {
    return err;
}

bool Client::setupConnection(QHostAddress & address, int port) {
    socket = new QTcpSocket();
    socket->connectToHost(address, port);
    if (!socket->waitForConnected(2000)) {
        err = socket->errorString();
        err += address.toString() + QString::number(port);
        delete socket;
        socket = nullptr;
        return false;
    } else {
        return true;
    }
}

bool Client::sendData(QString & data) {
    char * temp = ( char *)(data.toUtf8().constData());
    int len = data.length();
    int cur = 0;
    while (socket->isWritable() && cur < len) {
        cur += socket->write(temp + cur);
        if (cur < 0) {
            err = "Connection lost!";
            return false;
        }
    }
    return true;
}

bool Client::reciveData(QString & data) {
    QByteArray recived;
    int cur = 0;
    int length = 0;
    if (!socket->waitForReadyRead(2000)) {
        err = "Wait for reciving timeout!";
        return false;
    }
    if (socket->isReadable()) {
        QByteArray headers = socket->readAll();
        int headerPos = headers.indexOf("Content-Length: ");
        int headerPosEnd = headers.indexOf("\r\n", headerPos);
        int dataSeperator = headers.indexOf("\r\n\r\n");
        if (dataSeperator <= headerPos || dataSeperator == -1 || headerPos == -1) {
            data = headers;
            return true;
        }
        length = headers.mid(headerPos + 16, headerPosEnd - (headerPos + 16)).toInt();
        recived += headers;
        cur += headers.mid(dataSeperator + 4).length();
    } else {
        err = "Socket is not readable!";
        return false;
    }
    while (socket->isReadable() && cur < length) {
        QByteArray data = socket->readAll();
        recived.append(data);
        cur += data.length();
    }
    data = recived;
    return true;
}

void Client::closeConnection() {
    socket->disconnectFromHost();
    delete socket;
    socket = nullptr;
}

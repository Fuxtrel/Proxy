#include "websocketclient.h"
#include <QDataStream>
#include <QThread>
#include <QFileInfo>
#include <QDebug>


WSocketClient::WSocketClient() {
    QTextStream cin(stdin);
    filename = cin.readLine();
    qDebug() << "connection starting";
    connect(&WebSocket, &QWebSocket::connected, this, &WSocketClient::onConnected);
    connect(&WebSocket, &QWebSocket::disconnected, this, &WSocketClient::closed);
    //connect(&WebSocket,)
    WebSocket.open(QUrl(QStringLiteral("ws://0.0.0.0:40901")));
}

WSocketClient::~WSocketClient() {
    WebSocket.abort();
}


void WSocketClient::onConnected() {
    qDebug() << WebSocket.state();
    connect(&WebSocket, &QWebSocket::textMessageReceived, this, &WSocketClient::onTextMessageRecieved);
    if (i == 0) {
        WebSocket.sendTextMessage("File transmission start");
        i++;
    }
    if (transmission) {
        fileTransmission(filename, LessorIP, port, token);
        transmission = false;
    }
    //qDebug()<<"Connected";
    //WebSocket.sendTextMessage(QStringLiteral("Hello, world!"));

}

void WSocketClient::fileTransmissionPreparation(QString LessorIP, QString port, QString token) {
    QString LessorURL = "ws://" + LessorIP + ":" + port;
    //WebSocket.sendTextMessage(QStringLiteral("File transmission start"));
    //qDebug() << LessorURL;
    WebSocket.close();
    WebSocket.open(QUrl(LessorURL));
    bool test = WebSocket.state() == 2;
    qDebug() << WebSocket.state();
    qDebug() << test;
    while (WebSocket.state() != 3) {
        WebSocket.maxAllowedIncomingFrameSize();
        WebSocket.state();
    }

    //fileTransmission(filename, LessorIP, port, token);
    //qDebug()<< "Transmission check";

}

void WSocketClient::fileTransmission(QString filename, QString LessorIP, QString port, QString token) {
    //QString LessorURL = "ws://" + LessorIP + ":" + port;
    QString fileNameInDirectory;
    //qDebug() << QString("filename:" + fileNameInDirectory);
    WebSocket.sendTextMessage(QString("token:" + token));
    //qDebug() << QString("token:" + token);
    QStringList list = filename.split("/");
    QStringListIterator it(list);
    while (it.hasNext()) {
        fileNameInDirectory = it.next();
    }
    WebSocket.sendTextMessage(QString("filename:" + fileNameInDirectory));
    QFile file(filename);
    QFileInfo fileInfo(file);
    fileSize = fileInfo.size();
    if (file.open(QFile::ReadOnly)) {
        QByteArray data = file.readAll();
        WebSocket.sendBinaryMessage(data);

        file.close();
        qDebug() << "File sent";// << data;
        data.clear();
    } else {

        qFatal("_CLIENT: File could not be opened!");
    }
};


void WSocketClient::onTextMessageRecieved(QString message) {
    qDebug() << "Message received:" << message;
    //WebSocket.sendTextMessage(QStringLiteral("File transmission start"));
    QStringList list = message.split("|");
    if (list.at(0) == "Settings") {
        LessorIP = list.at(1);
        port = list.at(2);
        token = list.at(3);
        qDebug() << "LessorIP:" << LessorIP << "; port:" << port << "; token:" << token;
        transmission = true;
        fileTransmissionPreparation(LessorIP, port, token);
        if (WebSocket.state() == 3) {
            //fileTransmission(filename, LessorIP, port, token);
        }
    };

}
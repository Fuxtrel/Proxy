#ifndef PROXY_WEBSOCKETCLIENT_H
#define PROXY_WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <iostream>


class WSocketClient : public QObject {

Q_OBJECT

public:
    explicit WSocketClient();

    ~WSocketClient();

signals:

    void connected();

    void closed();

private slots:

    void onConnected();

    void onTextMessageRecieved(QString message);

    void fileTransmissionPreparation(QString LessorIP, QString port, QString token);

    void fileTransmission(QString filename, QString LessorIP, QString port, QString token);

private:
    QWebSocket WebSocket;
    QString LessorIP;
    QString port;
    QString token;
    QString filename;
    qint64 fileSize;
    int i = 0;
    bool transmission = false;

};

#endif //PROXY_WEBSOCKETCLIENT_H

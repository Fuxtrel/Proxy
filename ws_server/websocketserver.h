#ifndef PROXY_WSServer_H
#define PROXY_WSServer_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <iostream>
#include <QDebug>

class WSServer;

class WSServer : public QObject {

Q_OBJECT

public:
    explicit WSServer(QObject *parent = nullptr);

    ~WSServer() override;

signals:

    void closed();

private slots:

    void NewConnection();

    void ProcessTextMessage(const QString &message);

    void RecieveFile(const QByteArray &message);

    void SocketDisconnected();

private:
    QWebSocketServer *Web_socket_server;
    QList<QWebSocket *> clients;
    QWebSocket *aSocket;
    QString token;
    QString filename;
    int i = 0;
};


#endif //PROXY_WSServer_H

#ifndef PROXY_Server_H
#define PROXY_Server_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtWebSockets/QWebSocket>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)

QT_FORWARD_DECLARE_CLASS(QWebSocket)


class Server : public QObject {
Q_OBJECT
public:
    explicit Server(quint16 port, const QUrl &url);

    ~Server();

Q_SIGNALS:

    void closed();

private Q_SLOTS:

    void onNewConnection();

    void processTextMessage(QString message);

    void processBinaryMessage(QByteArray message);

    void socketDisconnected();

    void onConnected();

    void textMessageReceived(QString message);

    void onTextMessageReceived(QString message);

private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket *m_Server;
    bool m_debug;
    QList<QWebSocket *> clients;
    QWebSocket webSocket_;
    QUrl url_;
};


#endif //PROXY_Server_H

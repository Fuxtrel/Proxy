#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QTextStream>
#include <QRandomGenerator>
#include <string>
#include <cstdlib>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class Proxy : public QObject
{
Q_OBJECT
public:
    explicit Proxy(quint16 port);
    ~Proxy() override;

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void onTextMessageReceived(QString message);
    static QString generateToken();


private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket* m_Server;
    bool m_debug;
    QList<QWebSocket *> clients;
    QHostAddress client_peer_address;
    quint16 client_peer_port;
};


#endif //PROXY_PROXY_H

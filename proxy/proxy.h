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
#include <QJsonDocument>
#include <QJsonObject>
#include <string>
#include <cstdlib>
#include <QFile>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <list>
#include <iterator>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

using namespace std;

class Timer{
public:
    Timer() = default;
    ~Timer() = default;
    void add(std::chrono::seconds delay, std::function<void ()> callback, bool asynchronous = true);
private:

};

class Work_time{
public:
    explicit Work_time(QString UserID);
    ~Work_time() = default;
    long getWorkTime();
    QString UID;
private:
    uint start_time;
    uint work_time;
};

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
    static QString generateToken();
    void socketDisconnected();
    void processBinaryMessage(QByteArray message);
    void onTextMessageReceived(QString message);
    //void processTextMessage(QString message);
private:
    QWebSocketServer *m_pWebSocketServer;
    QString UID;
    bool m_debug;
    QList<QWebSocket *> clients;
    QList<Work_time> work_time;
};




#endif //PROXY_PROXY_H

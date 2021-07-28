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
#include <QTimer>
#include <QString>
#include <QJsonParseError>
#include <QJsonArray>
#include <QVector>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

using namespace std;

class Work_time{
public:
    explicit Work_time(QString UserID);
    ~Work_time() = default;
    long getWorkTime();
    QString UID;
    void setFlagKeepAliveOff();
    void setFlagKeepAliveOn();
    bool isKeepAlive();
private:
    uint start_time;
    uint work_time;
    bool keep_alive;
};

class Proxy : public QObject
{
Q_OBJECT
public:
    explicit Proxy(quint16 port);
    ~Proxy() override;
    //Состояние портов меняется на true, когда порт свободен
    std::vector<bool> state_port;

Q_SIGNALS:
    void closed();

public slots:
    void keepAlive();

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
    //Данные о текущем подключении клиента
    QList<Work_time> work_time;
    std::vector<QString> tokens;
    std::vector<QString> session_id;
};

class ProxyTransfer : public QObject
{
Q_OBJECT
public:
    explicit ProxyTransfer(quint16 port);
    ~ProxyTransfer() override;

signals:
    void socDiscon(std::string str);

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void socketDisconnected();
    void processBinaryMessage(QByteArray message);
    void processTextMessage(QString message);
private:
    QWebSocketServer *m_pWebSocketServer;
    bool m_debug;
    QList<QWebSocket *> clients;
    QList<QString> transaction_token;

};




#endif //PROXY_PROXY_H

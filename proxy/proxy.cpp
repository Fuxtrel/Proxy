#include "proxy.h"
#include <QHostAddress>
#include <fstream>
#include <QFile>
#include <utility>



QT_USE_NAMESPACE

//! [constructor]
Proxy::Proxy(quint16 port) :
        m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"),
                                                QWebSocketServer::NonSecureMode, this)) {
    if (m_pWebSocketServer->listen(QHostAddress("0.0.0.0"), port)) {
        if (m_pWebSocketServer->isListening()) {
            qDebug() << "server listening on port" << port;
        } else {
            qDebug() << "server can not listen port" << port;
        }
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Proxy::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Proxy::closed);
        m_debug = false;
    }

    //m_Server->open(QUrl(QStringLiteral("ws://0.0.0.0:4040")));
    //connect(m_Server, &QWebSocket::connected, this, &Proxy::onConnected);

}
//! [constructor]

Proxy::~Proxy() {
    m_pWebSocketServer->close();
    qDeleteAll(clients.begin(), clients.end());
}

//! [onNewConnection]
void Proxy::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    pSocket->setObjectName("");
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Proxy::onTextMessageReceived);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Proxy::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Proxy::socketDisconnected);
    clients << pSocket;
    clients[clients.length() - 1]->sendTextMessage("Hello");
    qDebug() << "Clients count: " << clients.size() << "\n";
}
//! [onNewConnection]

//! [processTextMessage]
/*void Proxy::processTextMessage(QString message) {

}*/
//! [processTextMessage]

//! [processBinaryMessage]
void Proxy::processBinaryMessage(QByteArray message) {
    auto *pClient = qobject_cast<QWebSocket *>(sender());
    QJsonDocument doc = QJsonDocument::fromJson(QString(message).toUtf8());
    QJsonObject json = doc.object();
    UID = json["UID"].toString();
    Work_time wt(UID);
    work_time.push_back(wt);
    if (json["startRequest"] == "Im main server") {
        qDebug() << json["startRequest"].toString();
        pClient->setObjectName("MainServer");
    }else {
        pClient->setObjectName(UID);
    }
}
//! [processBinaryMessage]

//! [socketDisconnected]
void Proxy::socketDisconnected() {
    auto *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << pClient->objectName();

    for(QList<Work_time>::iterator it = work_time.begin(); it != work_time.end(); it++){
        if(it->UID == pClient->objectName()){
            work_time.erase(it);
        }
    }

    qDebug() << "Время работы клиента: " << work_time[0].getWorkTime();
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
    qDebug() << clients.size();

}

void Proxy::onTextMessageReceived(QString message) {

}

QString Proxy::generateToken() {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 84; // assuming you want random strings of 12 characters
    QString token;
    for (int i = 0; i < randomStringLength; ++i) {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        token.append(nextChar);
    }
    return token;
}
//! [socketDisconnected]
Work_time::Work_time(QString  UserID) : UID(std::move(UserID)){
    start_time = std::time(nullptr);
    work_time = 0;
}


long Work_time::getWorkTime() {
    work_time = std::time(nullptr) - start_time;
    return work_time;
}

void Timer::add(std::chrono::seconds delay, std::function<void()> callback, bool asynchronous) {
    if (asynchronous) {
        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            callback();
        }).detach();
    }
    else {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        callback();
    }
}

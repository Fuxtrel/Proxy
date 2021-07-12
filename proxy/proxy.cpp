#include "proxy.h"
#include <QHostAddress>
#include <fstream>
#include <QFile>

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
    }
    for (int i = 0; i < 3; i++) {
        state.push_back(0);
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
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Proxy::onTextMessageReceived);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Proxy::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Proxy::socketDisconnected);
    clients << pSocket;
    clients[clients.length() - 1]->sendTextMessage("Hello");
    qDebug() << "Clients count: " << clients.size() << "\n";
}
//! [onNewConnection]

//! [processTextMessage]
void Proxy::processTextMessage(QString message) {

}
//! [processTextMessage]

//! [processBinaryMessage]
void Proxy::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QJsonDocument doc = QJsonDocument::fromJson(QString(message).toUtf8());
    QJsonObject json = doc.object();
    if (json["startRequest"] == "Im main server") {
        qDebug() << pClient->peerAddress() << pClient->peerName() << pClient->peerPort();
        qDebug() << json["startRequest"].toString();
        state[0] = 1;
    } else if (json["startRequest"] == "Im client_2") {
        qDebug() << pClient->peerAddress() << pClient->peerName() << pClient->peerPort();
        qDebug() << json["startRequest"].toString();
        state[1] = 1;
    } else if (json["startRequest"] == "Im client_3") {
        qDebug() << pClient->peerAddress() << pClient->peerName() << pClient->peerPort();
        qDebug() << json["startRequest"].toString();
        state[2] = 1;
    }
    if(state[0] == 1 && state[1] == 1 && state[2] == 1) {
        for (QWebSocket *cli:clients) {
            cli->sendTextMessage("Получили массированную рассылку спама");
        }
    }
}
//! [processBinaryMessage]

//! [socketDisconnected]
void Proxy::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
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

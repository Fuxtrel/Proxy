#include "server.h"

QTextStream cout(stdout);
QTextStream cin(stdin);

QT_USE_NAMESPACE

//! [constructor]
Server::Server(quint16 port) :
        m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"),
                                                QWebSocketServer::NonSecureMode, this)) {
    if (m_pWebSocketServer->listen(QHostAddress("localhost"), port)) {
        qDebug() << "server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }
}
//! [constructor]

Server::~Server() {
    m_pWebSocketServer->close();
    qDeleteAll(clients.begin(), clients.end());
}

//! [onNewConnection]
void Server::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    //connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    clients << pSocket;
}
//! [onNewConnection]

//! [processTextMessage]
void Server::processTextMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Message received:" << message;
    if (pClient) {
        if (message[6] == "a" && message[7] == "u" && message[8] == "t" && message[9] == "h") {
            pClient->sendTextMessage("login_ok");
            cout << "login_ok" << Qt::endl;
        } else if (message == "password") {
            pClient->sendTextMessage("password_ok");
            cout << "password_ok" << Qt::endl;
        } else if (message == "getUID") {
            cout << "Request UID" << Qt::endl;
            pClient->sendTextMessage("UIDlogEmail");
        }
    }
}
//! [processTextMessage]

//! [processBinaryMessage]
void Server::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}
//! [processBinaryMessage]

//! [socketDisconnected]
void Server::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void Server::onConnected() {
    connect(m_Server, &QWebSocket::textMessageReceived, this, &Server::textMessageReceived);
}

void Server::textMessageReceived(QString message) {
    if (message == "login_ok") {
        cout << "login_ok" << Qt::endl;
        m_Server->sendTextMessage(QStringLiteral("password"));
    } else if (message == "password_ok") {
        cout << "password_ok" << Qt::endl;
        m_Server->sendTextMessage(QStringLiteral("getUID"));
    }
}

void Server::onTextMessageReceived(QString message) {

}

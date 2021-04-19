#include "proxy.h"
#include <QHostAddress>

QTextStream cout(stdout);
QTextStream cin(stdin);

QT_USE_NAMESPACE

//! [constructor]
Proxy::Proxy(quint16 port) :
        m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"),
                                                QWebSocketServer::NonSecureMode, this)) {
    if (m_pWebSocketServer->listen(QHostAddress("localhost"), port)) {
        qDebug() << "server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Proxy::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Proxy::closed);
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
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Proxy::processTextMessage);
    //connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Proxy::socketDisconnected);

    clients << pSocket;
}
//! [onNewConnection]

//! [processTextMessage]
void Proxy::processTextMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Message received:" << message;
    if (pClient) {
        if (message[6] == "a" && message[7] == "u" && message[8] == "t" && message[9] == "h") {
            cout << "auth send to server" << Qt::endl;
            pClient->sendTextMessage("auth_ok");
        } else if (message == "getUID") {
            cout << "token:\n" + generateToken() + "\nUID:\nUID_client_1" << Qt::endl;
            cout << pClient->sendTextMessage("token: " + generateToken() + "\nUID: UID_client_1");
        }
    }
}
//! [processTextMessage]

//! [processBinaryMessage]
void Proxy::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
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

void Proxy::onConnected() {
    connect(m_Server, &QWebSocket::textMessageReceived, this, &Proxy::textMessageReceived);
}

void Proxy::textMessageReceived(QString message) {
    if (message == "login_ok") {
        cout << "login_ok" << Qt::endl;
        m_Server->sendTextMessage(QStringLiteral("password"));
    } else if (message == "password_ok") {

    }
}

void Proxy::onTextMessageReceived(QString message) {

}

QString Proxy::generateToken() {
    const QString
            possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
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

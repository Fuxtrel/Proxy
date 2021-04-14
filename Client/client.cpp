#include "client.h"
#include <QtCore/QDebug>

QTextStream cout(stdout);
QTextStream cin(stdin);

QT_USE_NAMESPACE

//! [constructor]
EchoClient::EchoClient(const QUrl &url, bool debug, QObject *parent) :
        QObject(parent),
        m_url(url),
        m_debug(debug) {
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    //Отсылаем логин
    connect(&m_webSocket, &QWebSocket::connected, this, &EchoClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &EchoClient::closed);
    m_webSocket.open(QUrl(url));
}
//! [constructor]

//! [onConnected]
void EchoClient::onConnected() {
    if (m_debug)
        qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &EchoClient::textMessageReceived);
    m_webSocket.sendTextMessage(QStringLiteral("login"));
}
//! [onConnected]

//! [onTextMessageReceived]
void EchoClient::onTextMessageReceived(QString message) {
    if (m_debug)
        cout << "Message received:" << message << Qt::endl;
    m_webSocket.close();
}

void EchoClient::textMessageReceived(QString message) {
    if (message == "login_ok") {
        cout << "login_ok" << Qt::endl;
        m_webSocket.sendTextMessage(QStringLiteral("password"));
    } else if (message == "password_ok") {
        cout << "password_ok" << Qt::endl;
        m_webSocket.sendTextMessage(QStringLiteral("getUID"));
    }else if((message[0] == "U") && (message[1] == "I") && (message[2] == "D") ){
        UID = message;
        cout << "UID client: " << message << Qt::endl;
    }

}
//! [onTextMessageReceived]

#include "client.h"
#include <QtCore/QDebug>

QTextStream cout(stdout);
QTextStream cin(stdin);

QT_USE_NAMESPACE

//! [constructor]
Client::Client(const QUrl &url) : url_(url) {
    connect(&webSocket_, &QWebSocket::connected, this, &Client::onConnected);
    connect(&webSocket_, &QWebSocket::disconnected, this, &Client::closed);
    qDebug() << url;
    webSocket_.open(url_);
}
//! [constructor]

//! [onConnected]
void Client::onConnected() {
    qDebug() << "WebSocket connected";
    connect(&webSocket_, &QWebSocket::textMessageReceived, this, &Client::textMessageReceived);
    webSocket_.sendTextMessage(QStringLiteral("token|auth|login|password"));
}
//! [onConnected]

//! [onTextMessageReceived]
void Client::onTextMessageReceived(QString message) {
    cout << "Message received:" << message << Qt::endl;
    webSocket_.close();
}

void Client::textMessageReceived(QString message) {
    if (message == "auth_ok") {
        cout << "auth_ok" << Qt::endl;
        webSocket_.sendTextMessage(QStringLiteral("getUID"));
    } else if ((message[0] == "t") && (message[1] == "o") && (message[2] == "k") && (message[3] == "e") && (message[4] == "n")) {
        cout << message << Qt::endl;
    }

}
//! [onTextMessageReceived]

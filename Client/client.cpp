#include "client.h"
#include <QtCore/QDebug>

QTextStream cout(stdout);
QTextStream cin(stdin);

QT_USE_NAMESPACE

//! [constructor]
Client::Client(const QUrl &url) : url_(url) {
    qDebug() << "WebSocket server:" << url;
    //Отсылаем логин
    connect(&webSocket_, &QWebSocket::connected, this, &Client::onConnected);
    connect(&webSocket_, &QWebSocket::disconnected, this, &Client::closed);
    qDebug() << url;
    webSocket_.open(url_);
    /*qDebug() << webSocket_.localAddress();
    qDebug() << webSocket_.localPort();
    qDebug() << webSocket_.peerAddress();
    qDebug() << webSocket_.peerPort();*/
}
//! [constructor]

//! [onConnected]
void Client::onConnected() {
    qDebug() << "WebSocket connected";
    connect(&webSocket_, &QWebSocket::textMessageReceived, this, &Client::textMessageReceived);
    webSocket_.sendTextMessage(QStringLiteral("login"));
}
//! [onConnected]

//! [onTextMessageReceived]
void Client::onTextMessageReceived(QString message) {
    cout << "Message received:" << message << Qt::endl;
    webSocket_.close();
}

void Client::textMessageReceived(QString message) {
    if (message == "login_ok") {
        cout << "login_ok" << Qt::endl;
        webSocket_.sendTextMessage(QStringLiteral("password"));
    } else if (message == "password_ok") {
        cout << "password_ok" << Qt::endl;
        webSocket_.sendTextMessage(QStringLiteral("getUID"));
    } else if ((message[0] == "U") && (message[1] == "I") && (message[2] == "D")) {
        UID = message;
        cout << "UID client: " << message << Qt::endl;
    }

}
//! [onTextMessageReceived]

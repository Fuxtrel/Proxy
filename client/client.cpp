#include "client.h"
#include <QtCore/QDebug>
#include <QJsonObject>
#include <QJsonDocument>

QTextStream cout(stdout);
QTextStream cin(stdin);

QT_USE_NAMESPACE

//! [constructor]
Client::Client(const QUrl &url) : url_(url) {
    connect(&webSocket_, &QWebSocket::connected, this, &Client::onConnected);
    connect(&webSocket_, &QWebSocket::disconnected, this, &Client::closed);
    qDebug() << url_;
    webSocket_.open(url_);
    qDebug() << webSocket_.state();
}
//! [constructor]

//! [onConnected]
void Client::onConnected() {
    qDebug() << webSocket_.state();
    connect(&webSocket_, &QWebSocket::textMessageReceived, this, &Client::onTextMessageReceived);
    QJsonObject jsonObj;
    jsonObj["startRequest"] = "Im main server";
    jsonObj["UID"] = "MainServer";
    QJsonDocument doc(jsonObj);
    QByteArray data = doc.toJson();
    webSocket_.sendBinaryMessage(data);
}
//! [onConnected]

//! [onTextMessageReceived]
void Client::onTextMessageReceived(QString message) {
    cout << "Message received:" << message << Qt::endl;
    webSocket_.close();
}

//! [onTextMessageReceived]

Timer::Timer(){}

Timer::~Timer() {}

void Timer::add(std::chrono::seconds delay, std::function<void()> callback, bool asynchronous) {
    if (asynchronous) {
        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            callback();
        }).detach();
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        callback();
    }
}
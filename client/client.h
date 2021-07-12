#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QTextStream>

class Client : public QObject {
Q_OBJECT
public:
    explicit Client(const QUrl &url);

Q_SIGNALS:

    void closed();

private Q_SLOTS:

    void onConnected();

    void onTextMessageReceived(QString message);
    //void processTextMessage(QString message);


private:
    QWebSocket webSocket_;
    QUrl url_;
    QString UID;
};


#endif //PROXY_CLIENT_H

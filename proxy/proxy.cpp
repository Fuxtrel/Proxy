#include "proxy.h"
#include <QHostAddress>
#include <fstream>
#include <QFile>
#include <utility>
#include <QThread>


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
    auto *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(keepAlive()));
    timer->start(6000);
}
//! [constructor]

Proxy::~Proxy() {
    m_pWebSocketServer->close();
    qDeleteAll(clients.begin(), clients.end());
}

//! [onNewConnection]
void Proxy::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    //pSocket->setObjectName("");
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Proxy::onTextMessageReceived);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Proxy::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Proxy::socketDisconnected);
    clients << pSocket;
    qDebug() << "Clients count on Main Proxy: " << clients.size() << "\n";
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
    UID = json["SessionID"].toString();
    qDebug() << UID;
    bool is_client_created = false;
    for (QWebSocket *ws : clients) {
        if (ws->objectName() == UID) {
            is_client_created = true;
        }
    }
    if (!is_client_created) {
        Work_time wt(UID);
        work_time.push_back(wt);
        if (json["startRequest"] == "Im main server") {
            qDebug() << json["startRequest"].toString();
            pClient->setObjectName("MainServer");
        } else {
            pClient->setObjectName(UID);
        }
        QJsonObject js;
        js["IP"] = "0.0.0.0";
        js["port"] = "4041";
        js["Transaction token"] = "01234567";
        QJsonDocument document(js);
        QByteArray data = document.toJson();
        pClient->sendBinaryMessage(data);
    } else if (json["Operation name"] == "Keep alive") {
        for (Work_time wt : work_time) {
            if (wt.UID == pClient->objectName()) {
                wt.setFlagKeepAliveOn();
                qDebug() << "keepAlive";
            }
        }
    } else if (json["tokens"] != QJsonValue::Undefined && json["tokens"].isArray() &&
               json["sessions"] != QJsonValue::Undefined && json["sessions"].isArray()) {
        tokens.reserve(json["tokens"].toArray().size());
        tokens.reserve(json["sessions"].toArray().size());
        QJsonArray token = json["tokens"].toArray();
        QJsonArray session = json["sessions"].toArray();
        for(int i = 0; i < tokens.size(); i++){
            tokens[i] = token[i].toString();
            session_id[i] = session[i].toString();
        }
        for(QWebSocket *ws : clients){
            for(int i = 0; i < session_id.size(); i++){
                if("\"" + ws->objectName() + "\"" == session_id[i]){
                    QJsonObject js = doc.object();
                    js["transaction token"] = session_id[i].mid(1, session_id[i].size() - 2);
                    js["session id"] = ws->objectName();
                    js["start request"] = "true";
                    QJsonDocument document(js);
                    QByteArray data = document.toJson();
                    ws->sendBinaryMessage(data);
                }
            }
        }


    }
}
//! [processBinaryMessage]

//! [socketDisconnected]
void Proxy::socketDisconnected() {
    auto *pClient = qobject_cast<QWebSocket *>(sender());
    for (QList<Work_time>::iterator it = work_time.begin(); it != work_time.end(); it++) {
        qDebug() << it->UID;
        qDebug() << pClient->objectName();
        if (it->UID == pClient->objectName()) {
            qDebug() << "Время подключения к MainProxy" << to_string(it->getWorkTime()).c_str();
            for (QWebSocket *p_client : clients) {
                if (p_client->objectName() == "MainServer") {
                    p_client->sendTextMessage(to_string(it->getWorkTime()).c_str());
                }
            }
            work_time.erase(it);
        }
    }
    //qDebug() << "Время работы клиента: " << work_time[0].getWorkTime();
    if (m_debug)
        qDebug() << "socketDisconnected: " << pClient;
    if (pClient) {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
    qDebug() << "Количество клиентов" << clients.size();
    qDebug() << "Количество данных о клиентах" << work_time.size();

}

void Proxy::onTextMessageReceived(QString message) {

}

QString Proxy::generateToken() {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 84; // assuming random strings of 12 characters
    QString token;
    for (int i = 0; i < randomStringLength; ++i) {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        token.append(nextChar);
    }
    return token;
}

//! [socketDisconnected]
Work_time::Work_time(QString UserID) : UID(std::move(UserID)) {
    start_time = std::time(nullptr);
    work_time = 0;
    keep_alive = true;
}

long Work_time::getWorkTime() {
    work_time = std::time(nullptr) - start_time;
    return work_time;
}

void Work_time::setFlagKeepAliveOff() {
    keep_alive = false;
}

void Work_time::setFlagKeepAliveOn() {
    keep_alive = true;
}

bool Work_time::isKeepAlive() {
    if (keep_alive) {
        return true;
    } else {
        return false;
    }
}

void Proxy::keepAlive() {
    for (QWebSocket *ws : clients) {
        for (Work_time wt : work_time) {
            if (wt.UID == ws->objectName()) {
                if (!wt.isKeepAlive()) {
                    ws->close();
                }
            }
        }
    }
    qDebug() << "keepAlive is sending";
    for (Work_time wt: work_time) {
        wt.setFlagKeepAliveOff();
    }
    for (QWebSocket *ws : clients) {
        ws->sendTextMessage("KeepAlive");
    }
}


void ProxyTransfer::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &ProxyTransfer::processBinaryMessage);
    connect(pSocket, &QWebSocket::textMessageReceived, this, &ProxyTransfer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &ProxyTransfer::socketDisconnected);
    clients << pSocket;
    qDebug() << "Добавился ещё один клиент";
    qDebug() << "Теперь количество клиентов на TransferProc порт: " << m_pWebSocketServer->serverPort() << "достигло"
             << clients.size() << "\n";
}

void ProxyTransfer::socketDisconnected() {
    auto *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        for (QWebSocket *ws : clients) {
            if (pClient != ws) {
                if (ws->objectName() == pClient->objectName()) {
                    clients.removeAll(ws);
                    ws->deleteLater();
                }
            }
        }
        clients.removeAll(pClient);
        pClient->deleteLater();
    }

}

//Пересылка json файлов и прочих данных от отправителя к получателю
void ProxyTransfer::processBinaryMessage(QByteArray message) {
    auto *pClient = qobject_cast<QWebSocket *>(sender());
    QJsonDocument doc = QJsonDocument::fromJson(QString(message).toUtf8());
    QJsonObject json = doc.object();
    if (pClient->objectName().isEmpty()) {
        if (json["Transfer token"] != QJsonValue::Undefined) {
            pClient->setObjectName(json["Transaction token"].toString());
        }
        for (QWebSocket *ws : clients) {
            if (pClient != ws) {
                if (ws->objectName() == pClient->objectName()) {
                    pClient->sendBinaryMessage("Ready for send");
                    ws->sendBinaryMessage("Ready for send");
                }
            }
        }
    } else {
        bool is_second_client_exist = false;
        for (QWebSocket *ws : clients) {
            if (pClient != ws) {
                if (ws->objectName() == pClient->objectName()) {
                    is_second_client_exist = true;
                    ws->sendBinaryMessage(message);
                    qDebug() << "Сообщение успешно переправлено!\u2713";
                    break;
                }
            }
        }
        if (!is_second_client_exist) {
            pClient->sendBinaryMessage("Second client doesn't yet exist");
            qDebug() << "Второй клиент пока что отсутствует";
        }
    }

}

ProxyTransfer::ProxyTransfer(quint16 port) :
        m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"),
                                                QWebSocketServer::NonSecureMode, this)) {
    if (m_pWebSocketServer->listen(QHostAddress("0.0.0.0"), port)) {
        if (m_pWebSocketServer->isListening()) {
            qDebug() << "Сервер слушает порт: " << port;
        } else {
            qDebug() << "Сервер слушает порт: " << port;
        }
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &ProxyTransfer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &ProxyTransfer::closed);
        m_debug = false;
    }
}

ProxyTransfer::~ProxyTransfer() {
    m_pWebSocketServer->close();
    qDeleteAll(clients.begin(), clients.end());
}

void ProxyTransfer::processTextMessage(QString message) {
    if (clients.size() == 2) {
        for (QWebSocket *ws : clients) {
            if (ws->objectName() == "Sender") {
                ws->sendTextMessage(message);
            }
        }
    }
}



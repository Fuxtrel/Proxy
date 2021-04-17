#include "websocketserver.h"
#include <QFile>

static QString getIdentifier(QWebSocket *peer) {
    return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(),
                                       QString::number(peer->peerPort()));
}

WSServer::WSServer(QObject *parent) :
        QObject(parent),
        Web_socket_server(new QWebSocketServer(QStringLiteral("WebSocketServer"), QWebSocketServer::NonSecureMode)) {

    qDebug() << "Initialising listen";
    if (Web_socket_server->listen(QHostAddress("0.0.0.0"), 40901)) {
        QObject::connect(Web_socket_server, SIGNAL(newConnection()), this, SLOT(NewConnection()));
        qDebug() << "Server started";
    } else {
        qDebug() << "Failedto listen port";
        //throw std::runtime_error("WebSocketServer: failed to listen port");
    }
}

WSServer::~WSServer() {
    Web_socket_server ->close();
}

void WSServer::NewConnection() {
    qInfo() << "WebSocketServer::NewConnection";
    aSocket = Web_socket_server->nextPendingConnection();
    //aSocket->sendTextMessage("You are connected to the server");
    qInfo() << getIdentifier(aSocket) << "connected!\n";
    QObject::connect(aSocket, SIGNAL(textMessageReceived(const QString&)), this,
                     SLOT(ProcessTextMessage(const QString&)));
    QObject::connect(aSocket, SIGNAL(binaryMessageReceived(const QByteArray&)), this,
                     SLOT(RecieveFile(const QByteArray&)));
    QObject::connect(aSocket, SIGNAL(disconnected()), this, SLOT(SocketDisconnected()));

    clients << aSocket;
    if (clients.size()>1){
        clients.at(clients.size() - 2)->sendTextMessage(QString("test message for 1st client"));
    }
}

void WSServer::ProcessTextMessage(const QString &message) {
    int sender = clients.size() - 1;
    QStringList list = message.split(":");
    qInfo() << "WebSocketServer::ProcessTextMessage: " << message;
    if (list.at(0) == "File transmission start" && i == 0) {
        //qDebug()<< "debug";
        //aSocket->sendTextMessage("Settings|0.0.0.0|40901|token");
        clients.at(sender)->sendTextMessage("Settings|0.0.0.0|40901|token");
        i++;
    }

    if (list.at(0) == "token") {
        token = list.at(1);
        qDebug() << "token :" << token;
    }

    if (list.at(0) == "filename") {
        filename = list.at(1);
        //filename = "../get.png";
        qDebug() << filename;
    }
    //filename = "../get.txt";

}

void WSServer::RecieveFile(const QByteArray &message) {
    //qDebug() << message;
    qDebug() << "Recieving file";
    QFile file(filename);
    file.open(QFile::WriteOnly);
    file.write(message);
    file.close();
}

void WSServer::SocketDisconnected() {
    qInfo() << "WebSocketServer::SocketDisconnected";

}
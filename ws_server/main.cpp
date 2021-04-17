#include <QCoreApplication>
#include <QDataStream>
#include <QThread>
#include "websocketserver.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    //qDebug()<< "io";
    WSServer server;
    return a.exec();

}


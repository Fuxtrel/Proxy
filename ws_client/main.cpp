#include <QCoreApplication>
#include "websocketclient.cpp"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    WSocketClient client;
    return QCoreApplication::exec();
}
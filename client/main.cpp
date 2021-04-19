#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "client.h"


int main(int argc, char *argv[]) {
    QTextStream cout(stdout);
    QTextStream cin(stdin);
    QCoreApplication a(argc, argv);
    QString ip = argv[1];
    QString port = argv[2];
    QString url = "ws://" + ip + ":" + port;
    Client client((QUrl(url)));
    QObject::connect(&client, &Client::closed, &a, &QCoreApplication::quit);

    return QCoreApplication::exec();
}


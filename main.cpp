#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <cstring>
#include <string>
#include "proxy.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString ip = argv[1];
    QString port = argv[2];

    Server server(quint16());
    return QCoreApplication::exec();
}

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineOption>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString ip = argv[1];
    QString port = argv[2];

    Server server(quint16(4040));
    return QCoreApplication::exec();
}

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineOption>
#include "proxy.h"
#include <thread>
#include <QThread>

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
//    QString ip = argv[1];
//    QString port = argv[2];
//    //Запуск основного Main Proxy
//    unsigned int count_of_available_threads = std::thread::hardware_concurrency();
//    Proxy proxy(quint16(4040));
//
//    //Запуск таймера для Keep Alive
//
//    //Запуск потоков с Proxy Transfer
//    QThread *thread[count_of_available_threads - 2];
//    QList<ProxyTransfer *> vec;
//    for(int i = 0; i < (count_of_available_threads - 2); i++){
//        auto proxy_transfer = new ProxyTransfer(quint16(4041 + i));
//        vec << proxy_transfer;
//        thread[i] = new QThread();
//        proxy_transfer->moveToThread(thread[i]);
//        thread[i]->start();
//    }
//
//    return QCoreApplication::exec();

    QString str("{\"tokens\":[\"10000000\",\"10000001\",\"10000002\",\"10000003\",\"10000004\",\"10000005\",\"10000006\",\"10000007\",\"10000008\",\"10000009\"],\"sessions\":[\"80000000\",\"80000001\",\"80000002\",\"80000003\",\"80000004\",\"80000005\",\"80000006\",\"80000007\",\"80000008\",\"80000009\"]}");
    QJsonDocument doc = QJsonDocument::fromJson(QString(str).toUtf8());
    QJsonObject json = doc.object();
    QJsonArray arr = json["tokens"].toArray();
    for (int i = 0; i < arr.size(); i++){
        qDebug() << arr[i].toString();
    }
    qDebug() << arr.size();
}

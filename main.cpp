#include <QCoreApplication>
#include "server.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/alex/build-server-Desktop_Qt_5_13_1_GCC_64bit-Debug/epic_shelter.db3");
    db.open();
    if (db.isOpen())
        std::cout << "Сервер подключен к базе данных" << std::endl;
    else
        std::cout << "Сервер не подключен к базе данных" << std::endl;
    QCoreApplication a(argc, argv);
    Server server;
    server.setDatabase(db);
    server.startServer();
    return a.exec();
}

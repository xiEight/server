#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QDir>


class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();
    QTcpSocket* socket;
    QByteArray data;
    void setDatabase(QSqlDatabase dbase);
private:
    int count;
    QString filename;
    QSqlDatabase *db;
    QSqlQuery *query;
    QString client;
    QString date;
public slots:
    void startServer();
    void incomingConnection(qintptr handle) override;
    void sockReady();
    void sockDisc();
};

#endif // SERVER_H

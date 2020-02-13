#include "server.h"

Server::Server(){
    count = 0;
};

Server::~Server()
{
    db->close();
};

void Server::startServer()
{
     if (this->listen(QHostAddress::Any, 8030))
         std::cout<<"Сервер запущен!" << std::endl;
     else
         std::cout<<"Сервер не запущен!" << std::endl;
}

void Server::incomingConnection(qintptr handle)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);
    connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
    client = socket->localAddress().toString().replace("::ffff:","");
    std::cout<< client.toStdString() <<" подключен" << std::endl;

}

void Server::setDatabase(QSqlDatabase dbase)
{
    db = &dbase;
    query = new QSqlQuery(*db);
}

void Server::sockReady()
{
    if (count == 0)
      {
          if (socket->waitForConnected(500)){
              socket->waitForReadyRead(500);
          QString f;
          f = QString::fromStdString(socket->readLine(1000).toStdString());
          QStringList lst = f.split(";");
          if (lst[0] == "ADDED"){
          filename = lst[1].remove("test/");
          date = lst[2];
          std::cout <<"прием файла:" << filename.toStdString() << std::endl;
          count++;
          socket->flush();
            }
          else if (lst[0] == "REMOVED")
          {
              std::cout <<client.toStdString()<<": файл "<< lst[1].remove("test/").toStdString() << " был перемещен или удален"<<std::endl;
              query->prepare("INSERT INTO Лог(Имя_компьютера, Событие, Дата) VALUES(:name, :event, :date)");
              query->bindValue(":name", client);
              query->bindValue(":event","Удален файл " + lst[1].remove("test/"));
              query->bindValue(":date", lst[2]);
              query->exec();
              query->clear();
              socket->flush();
              socket->disconnect();
              socket->deleteLater();
          }
          }
      }
      else{
      if (socket->waitForConnected(500)){
          socket->waitForReadyRead(500);
          while(this->socket->bytesAvailable() > 0){
              data.append(socket->readAll());
              QString s = QString::number(data.size());
              std::cout << s.toStdString().c_str() << " байт скачано!" << std::endl;
          }
            socket->waitForReadyRead(500);

          if (socket->bytesAvailable() == 0)
          {
              query->prepare("INSERT INTO Лог(Имя_компьютера, Событие, Дата) VALUES(:name, :event, :date)");
              query->bindValue(":name", client);
              query->bindValue(":event","Добавлен файл " + filename);
              query->bindValue(":date", date);
              query->exec();
              query->clear();
              if (!QDir(client).exists())
                QDir().mkdir(client);
              QFile file(client + "/" + filename);
              file.open(QIODevice::WriteOnly);
              file.write(data);
              count = 0;
              data.clear();
              file.close();
              socket->flush();
              socket->disconnect();
              socket->deleteLater();
          }
      }
      }
}

void Server::sockDisc()
{
    std::cout << client.toStdString() << " отключился от сервера" << std::endl;
    socket->deleteLater();
}

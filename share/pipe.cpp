#include "pipe.h"

#include <QDebug>
#include <QSqlQuery>
#include <QVariant>

Pipe::Pipe():
  db(QSqlDatabase::addDatabase("QODBC3"))
{
  db.setDatabaseName("SE");
  db.open();
  qDebug() << db.isOpen();
}

Pipe::~Pipe()
{
  db.close();
}

void Pipe::addUser(const User &user)
{
  QSqlQuery query(db);
  query.prepare("INSERT INTO usr VALUES(:id, :pswd);");
  query.bindValue(":id", QVariant(user.id));
  query.bindValue(":pswd", QVariant(user.pswd));
  query.exec();
  qDebug() << "add user";
}

void Pipe::delUser(const User &user)
{
  QSqlQuery query(db);
  query.prepare("DELETE FROM usr WHERE usr.id = :id;");
  query.bindValue(":id", QVariant(user.id));
  query.exec();
  qDebug() << "delete user";
}

void Pipe::addRoom(const Room &room)
{
  // ************ TODO ***************
  QSqlQuery query(db);
  query.prepare("INSERT INTO room VALUES("
                ":roomId, :usrId, :temp, :settemp,"
                ":wdspd, :setwdspd, :token, :state, :mode,"
                ":duration, :pwr, :cost"
                ");");
  query.bindValue(":roomId", room.roomId);
  query.bindValue(":usrId", room.usrId);
  query.bindValue(":temp", room.temp);
  query.bindValue(":settemp", room.settemp);
  query.bindValue(":wdspd", room.wdspd);
  query.bindValue(":setwdspd", room.setwdspd);
  query.bindValue(":token", room.token);
  query.bindValue(":state", room.state);
  query.bindValue(":mode", room.mode);
  query.bindValue(":duration", room.duration);
  query.bindValue(":pwr", room.pwr);
  query.bindValue(":cost", room.cost);
  query.exec();
  qDebug() << "add room";
}

void Pipe::delRoom(const Room &room)
{
  QSqlQuery query(db);
  query.prepare("DELETE FROM room WHERE room.roomId = :roomId");
  query.bindValue(":roomId", room.roomId);
  query.exec();
  qDebug() << "del room";
}

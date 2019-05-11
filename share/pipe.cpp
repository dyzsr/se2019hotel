#include "pipe.h"

#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>

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

Room Pipe::getRoom(QString usrId)
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM room"
                "WHERE room.usrId = :usrId;");
  query.bindValue(":usrId", usrId);
  query.exec();
  Room room;
  if (query.isValid()) {
    room.roomId = query.record().value("roomId").toInt();
    room.usrId = query.record().value("usrId").toString();
    room.temp = query.record().value("temp").toDouble();
    room.settemp = query.record().value("settemp").toDouble();
    room.wdspd = query.record().value("wdspd").toDouble();
    room.setwdspd = query.record().value("setwpspd").toDouble();
    room.token = query.record().value("token").toString();
    room.state = query.record().value("state").toInt();
    room.mode = query.record().value("mode").toInt();
    room.duration = query.record().value("duration").toDateTime();
    room.pwr = query.record().value("pwr").toDouble();
    room.cost = query.record().value("cost").toDouble();
  } else {
    qDebug() << "error";
//    qDebug() << query.lastError();
  }
  return room;
}

void Pipe::addRoom(const Room &room)
{
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

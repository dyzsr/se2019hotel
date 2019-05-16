#include "pipe.h"

#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>

Pipe::Pipe():
  db(QSqlDatabase::addDatabase("QODBC3"))
{
  db.setDatabaseName("SE_GROUP");
  db.open();
  qDebug() << "open db";
  qDebug() << db.isOpen();
}

Pipe::~Pipe()
{
  db.close();
  qDebug() << "close db";
}

void Pipe::addUser(const User &user)
{
  QSqlQuery query(db);
  query.prepare("INSERT INTO tcs_app_usr VALUES(:id, :pswd);");
  query.bindValue(":id", QVariant(user.id));
  query.bindValue(":pswd", QVariant(user.pswd));
  query.exec();
  qDebug() << "add user";
}

void Pipe::delUser(const User &user)
{
  QSqlQuery query(db);
  query.prepare("DELETE FROM tcs_app_usr WHERE name = :id;");
  query.bindValue(":id", QVariant(user.id));
  query.exec();
  qDebug() << "delete user";
}

Room Pipe::getRoom(QString usrId)
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM room"
                "WHERE user_id_id = :usrId;");
  query.bindValue(":usrId", usrId);
  query.exec();
  Room room;
  if (query.isValid()) {
    room.roomId = query.record().value("id").toInt();
    room.usrId = query.record().value("user_id_id").toString();
    room.temp = query.record().value("temp").toDouble();
    room.settemp = query.record().value("settemp").toDouble();
    room.wdspd = query.record().value("wdspd").toDouble();
    room.setwdspd = query.record().value("setwpspd").toDouble();
    room.token = query.record().value("token").toString();
    room.state = query.record().value("state").toInt();
    room.mode = query.record().value("mode").toInt();
    room.duration = query.record().value("duration").toDateTime();
    room.start = query.record().value("start").toDateTime();
    room.pwr = query.record().value("power").toDouble();
    room.cost = query.record().value("costs").toDouble();
  } else {
    qDebug() << "error";
//    qDebug() << query.lastError();
  }
  return room;
}

void Pipe::addRoom(const Room &room)
{
  QSqlQuery query(db);
  query.prepare("INSERT INTO tcs_app_room VALUES("
                ":roomId, :settemp, :temp,"
                ":wdspd, :setwdspd, :token, :state, :mode,"
                ":cost, :pwr, :duration, :usrId, :start"
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
  query.bindValue(":start", room.start);
  query.exec();
  qDebug() << "add room";
}

void Pipe::delRoom(const Room &room)
{
  QSqlQuery query(db);
  query.prepare("DELETE FROM tcs_app_room WHERE id = :roomId");
  query.bindValue(":roomId", room.roomId);
  query.exec();
  qDebug() << "del room";
}

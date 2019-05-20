#include "pipe.h"

#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>

Pipe::Pipe():
  db(QSqlDatabase::addDatabase("QMYSQL","QODBC3"))
{
  db.setHostName("140.143.236.79");
  db.setPort(3306);
  db.setUserName("se");
  db.setPassword("se2019");
  db.setDatabaseName("se_group");
  db.open();
  qDebug() << "open db";
  if (db.isOpen()) {
    qDebug() << "successfully";
  } else {
    qDebug() << "failed";
  }
}

Pipe::~Pipe()
{
  db.close();
  qDebug() << "close db";
}

Room Pipe::getRoom(QString usrId)
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM tcs_app_room "
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
    qDebug() << "getRoom error";
//    qDebug() << query.lastError();
  }
  return room;
}

User Pipe::getUser(QString usrId)
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM tcs_app_user "
              "WHERE name = :usrId;");
  query.bindValue(":usrId", usrId);
  query.exec();
  User user;
  if (query.isValid()) {
    user.id = query.record().value("name").toString();
    user.id = query.record().value("pswd").toString();
  } else {
    qDebug() << "getUser error";
  }
  return User();
}

void Pipe::addUser(const User &user)
{
  QSqlQuery query(db);
  query.prepare("INSERT INTO tcs_app_user VALUES(:id, :pswd);");
  query.bindValue(":id", QVariant(user.id));
  query.bindValue(":pswd", QVariant(user.pswd));
  query.exec();
  qDebug() << "add user";
}

void Pipe::delUser(const User &user)
{
  QSqlQuery query(db);
  query.prepare("DELETE FROM tcs_app_user WHERE name = :id;");
  query.bindValue(":id", QVariant(user.id));
  query.exec();
  qDebug() << "delete user";
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

QVector<Request> Pipe::getRequests()
{
  QSqlQuery query(db);
  QSqlRecord rec;
  Request req;
  QVector<Request> q;
  query.prepare("SELECT * FROM tcs_app_request");
  query.exec();
  do {
    rec = query.record();
    req.reqId = rec.value("id").toInt();
    req.state = rec.value("state").toInt();
    req.usrId = rec.value("user_id_id").toString();
    req.settemp = rec.value("settemp").toDouble();
    req.setwdspd = rec.value("setwdspd").toInt();
    q.append(req);
  } while (query.next());
  return q;
}

void Pipe::sendRequest(const Request &request)
{
  QSqlQuery query(db);
  query.prepare("INSERT INTO tcs_app_request VALUES(:id, :settemp, :setwdspd, "
                ":state, :user_id_id);");
  query.bindValue(":id", request.reqId);
  query.bindValue(":settemp", request.settemp);
  query.bindValue(":setwdspd", request.setwdspd);
  query.bindValue(":state", request.state);
  query.bindValue(":user_id_id", request.usrId);
  query.exec();
  qDebug() << "send request";
}

void Pipe::delRequests(const QVector<Request> &requests)
{
  QSqlQuery query(db);
  query.prepare("DELETE FROM tcs_app_request WHERE id = :id");
  int i;
  for (i = 0; i < requests.length(); ++i)
  {
    query.bindValue(":id", requests.at(i).reqId);
    query.exec();
  }
  qDebug() << "del requests";
}
/*
QVector<Billing> Pipe::getBillings(QString usrId, int roomId)
{
  QSqlQuery query(db);
  QSqlRecord rec;
  Billing bil;
    QVector<Billing> q;
    query.prepare("SELECT * FROM tcs_app_bill"
                  "WHERE room_id_id = :roomId;");
    query.bindValue(":roomId", roomId);
    query.exec();
    do {
      rec = query.record();
      bil.rate = rec.value("id").toInt();

      q.append(req);
    } while (query.next());
    return q;
}
*/

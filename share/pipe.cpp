#include "pipe.h"

#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>

Pipe::Pipe():
  db(QSqlDatabase::addDatabase("QODBC3"))
{
  db.setDatabaseName("se_db");
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
  return user;
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
  qDebug() << "get requests";
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

QVector<Billing> Pipe::getBillings(QString usrId, int roomId)
{
  QSqlQuery query(db);
  QSqlRecord rec;
  Billing bil;
  QVector<Billing> q;
  query.prepare("SELECT * FROM tcs_app_bill "
                "WHERE room_id_id = :roomId;");
  query.bindValue(":roomId", roomId);
  query.exec();
  do {
    rec = query.record();
    bil.rate = rec.value("rate").toDouble();
    bil.costs = rec.value("costs").toDouble();
    bil.start = rec.value("start").toDateTime();
    bil.wdspd = rec.value("wdspd").toInt();
    bil.action = rec.value("action").toInt();
    bil.roomId = rec.value("room_id_id").toInt();
    bil.endTemp = rec.value("end_temp").toDouble();
    bil.duration = rec.value("duration").toDateTime();
    bil.billingId = rec.value("id").toInt();
    bil.startTemp = rec.value("start_temp").toDouble();
    q.append(bil);
  } while (query.next());
  qDebug() << "get billings";
  return q;
}

void Pipe::addBilling(const Billing &billing)
{
  QSqlQuery query(db);
  query.prepare("INSERT INTO tcs_app_bill VALUES(:id, :start, :duration, "
                ":costs, :wdspd, :start_temp, :end_temp, :rate, "
                ":action, :room_id_id);");
  query.bindValue(":id", billing.billingId);
  query.bindValue(":start", billing.start);
  query.bindValue(":duration", billing.duration);
  query.bindValue(":costs", billing.costs);
  query.bindValue(":wdspd", billing.wdspd);
  query.bindValue(":start_temp", billing.startTemp);
  query.bindValue(":end_temp", billing.endTemp);
  query.bindValue(":rate", billing.rate);
  query.bindValue(":action", billing.action);
  query.bindValue(":room_id_id", billing.roomId);
  query.exec();
  qDebug() << "add billing";
}

QVector<Admin> Pipe::getAdmins()
{
  QSqlQuery query(db);
  QSqlRecord rec;
  Admin adm;
  QVector<Admin> q;
  query.prepare("SELECT * FROM tcs_app_manager");
  query.exec();
  do {
    rec = query.record();
    adm.manId = rec.value("name").toString();
    adm.manpwsd = rec.value("pswd").toString();
    adm.privilege = rec.value("privilege").toInt();
    q.append(adm);
  } while (query.next());
  qDebug() << "get admins";
  return q;
}

Host Pipe::getHost()
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM tcs_app_host");
  query.exec();
  Host host;
  if (query.isValid()) {
    host.mode = query.record().value("mode").toInt();
    host.state = query.record().value("state").toInt();
    host.hostId = query.record().value("id").toInt();
    host.lowRate = query.record().value("low_rate").toDouble();
    host.maxTemp = query.record().value("max_temp").toInt();
    host.midRate = query.record().value("middle_rate").toDouble();
    host.minTemp = query.record().value("min_temp").toInt();
    host.paraLow = query.record().value("para_low").toDouble();
    host.paraMid = query.record().value("para_mid").toDouble();
    host.highRate = query.record().value("high_rate").toDouble();
    host.modeSche = query.record().value("mode_sche").toInt();
    host.paraHigh = query.record().value("para_high").toDouble();
    host.startTemp = query.record().value("start_temp").toInt();
    host.defaultTemp = query.record().value("default_temp").toInt();
    host.defaultWdspd = query.record().value("default_wdspd").toInt();
  } else {
    qDebug() << "getHost error";
  }
  return host;
}

QVector<User> Pipe::getUsers()
{
  QSqlQuery query(db);
  QSqlRecord rec;
  User user;
  QVector<User> q;
  query.prepare("SELECT * FROM tcs_app_user");
  query.exec();
  do {
    rec = query.record();
    user.id = rec.value("name").toString();
    user.pswd = rec.value("pswd").toString();
    q.append(user);
  } while (query.next());
  qDebug() << "get users";
  return q;
}

void Pipe::setUsers(const QVector<User> &users)
{
  QSqlQuery query(db);
  query.prepare("DELETE from tcs_app_user");
  query.exec();
  int i;
  for (i = 0; i < users.length(); ++i)
  {
    addUser(users.at(i));
  }
  qDebug() << "set users";
}

void Pipe::setRooms(const QVector<Room> &rooms)
{
  QSqlQuery query(db);
  query.prepare("DELETE from tcs_app_room");
  query.exec();
  int i;
  for (i = 0; i < rooms.length(); ++i)
  {
    addRoom(rooms.at(i));
  }
  qDebug() << "set rooms";
}

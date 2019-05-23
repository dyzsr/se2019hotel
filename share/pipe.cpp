#include "pipe.h"

#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>

Pipe Pipe::pipe;

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

Pipe *Pipe::getInstance()
{
  return &pipe;
}

Room Pipe::getRoom(QString usrId)
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM tcs_app_room "
                "WHERE user_id_id = :usrId;");
  query.bindValue(":usrId", usrId);
  Room room;
  if (query.exec() && query.next()) {
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
    qDebug() << "get room";
  } else {
//    qDebug() << "getRoom error";
    qDebug() << query.lastError();
  }
  return room;
}

User Pipe::getUser(QString usrId)
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM tcs_app_user "
                "WHERE name = :usrId;");
  query.bindValue(":usrId", usrId);

  User user;
  if (query.exec() && query.next()) {
    user.id = query.record().value(0).toString();
    user.pswd = query.record().value(1).toString();
    qDebug() << "get user";
  } else {
//    qDebug() << "getUser error";
    qDebug() << query.lastError();
  }
  return user;
}

void Pipe::addUser(const User &user)
{
  QSqlQuery query(db);
  query.prepare("INSERT INTO tcs_app_user VALUES(:id, :pswd);");
  query.bindValue(":id", user.id);
  query.bindValue(":pswd", user.pswd);
  if (query.exec())
    qDebug() << "add user";
  else
    qDebug() << query.lastError();
}

void Pipe::delUser(const User &user)
{
  QSqlQuery query(db);
  query.prepare("DELETE FROM tcs_app_user WHERE name = :id;");
  query.bindValue(":id", user.id);
  if (query.exec())
    qDebug() << "del user";
  else
    qDebug() << query.lastError();
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
  if (query.exec())
    qDebug() << "add room";
  else
    qDebug() << query.lastError();
}

void Pipe::delRoom(const Room &room)
{
  QSqlQuery query(db);
  query.prepare("DELETE FROM tcs_app_room WHERE id = :roomId");
  query.bindValue(":roomId", room.roomId);
  if (query.exec())
    qDebug() << "del room";
  else
    qDebug() << query.lastError();
}

QVector<Request> Pipe::getRequests()
{
  QSqlQuery query(db);
  QSqlRecord rec;
  Request req;
  QVector<Request> q;
  query.prepare("SELECT * FROM tcs_app_request");
  query.exec();
  while (query.next()) {
    rec = query.record();
    req.reqId = rec.value("id").toInt();
    req.state = rec.value("state").toInt();
    req.usrId = rec.value("user_id_id").toString();
    req.settemp = rec.value("settemp").toDouble();
    req.setwdspd = rec.value("setwdspd").toInt();
    q.append(req);
  }
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
  if (query.exec())
    qDebug() << "send request";
  else
    qDebug() << query.lastError();
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

QVector<Billing> Pipe::getBillings(int roomId)
{
  QSqlQuery query(db);
  QSqlRecord rec;
  Billing bil;
  QVector<Billing> q;
  query.prepare("SELECT * FROM tcs_app_bill "
                "WHERE room_id_id = :roomId;");
  query.bindValue(":roomId", roomId);
  query.exec();
  while (query.next()) {
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
  }
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
  if (query.exec())
    qDebug() << "add billing";
  else
    qDebug() << query.lastError();
}

QVector<Admin> Pipe::getAdmins()
{
  QSqlQuery query(db);
  QSqlRecord rec;
  Admin adm;
  QVector<Admin> q;
  query.prepare("SELECT * FROM tcs_app_manager");
  query.exec();
  while (query.next()) {
    rec = query.record();
    adm.manId = rec.value("name").toString();
    adm.manpwsd = rec.value("pswd").toString();
    adm.privilege = rec.value("privilege").toInt();
    q.append(adm);
  }
  qDebug() << "get admins";
  return q;
}

Admin Pipe::getAdmin(QString name)
{
  QSqlQuery query(db);
  Admin adm;
  query.prepare("SELECT * FROM tcs_app_manager "
                "WHERE name = :name;");
  query.bindValue(":name", name);
  if (query.exec() && query.next()) {
    adm.manId = query.record().value("name").toString();
    adm.manpwsd = query.record().value("pswd").toString();
    adm.privilege = query.record().value("privilege").toInt();
    qDebug() << "get admin";
  } else {
    qDebug() << query.lastError();
  }
  return adm;
}

Host Pipe::getHost()
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM tcs_app_host");
  Host host;
  if (query.exec()) {
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

QVector<Room> Pipe::getRooms()
{
  QSqlQuery query(db);
  query.prepare("SELECT * FROM tcs_app_room;");
  QVector<Room> rooms;
  if (query.exec()) {
    while (query.next()) {
      Room room;
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
      rooms.append(room);
    }
    qDebug() << "get rooms";
  } else {
    qDebug() << query.lastError();
  }
  return rooms;
}

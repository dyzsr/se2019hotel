#include "server.h"

#include <QTimer>
#include <QDebug>

Server::Server(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance())
{
  init();

  QTimer *timer_p = new QTimer(this);
  connect(timer_p, &QTimer::timeout, this, &Server::process);
  timer_p->start(1000);

  QTimer *timer_h = new QTimer(this);
  connect(timer_h, &QTimer::timeout, this, &Server::handleRequests);
  timer_h->start(3000);
}

Server::~Server() {}

void Server::init()
{
  info = pipe->getHost();

  room_lock.lockForWrite();

  rooms = pipe->getRooms();
  req_rooms = rooms.mid(0);
  for (Room room : rooms) {
    if (!room.usrId.isEmpty()) {
      user2room[room.usrId] = room.roomId;
    }
  }
  // init new_reqs
  new_reqs.resize(rooms.size());
  new_reqs.fill(-1);

  billings_cnt = pipe->getCurrentBillingId();
  billings.resize(rooms.size());
  for (int i = 0; i < rooms.size(); i++) {
    double rate = 0;
    if (rooms[i].setwdspd == 1)
      rate = info.lowRate;
    else if (rooms[i].setwdspd == 2)
      rate = info.midRate;
    else if (rooms[i].setwdspd == 3)
      rate = info.highRate;

    billings[i] = Billing(
                    billings_cnt++, // billing id
                    rooms[i].roomId,   // room id
                    QDateTime::currentDateTime(), // start
                    QDateTime::currentDateTime(), // duration
                    0.0, // costs
                    rooms[i].wdspd, // windspeed
                    rooms[i].temp, // start temperature
                    rooms[i].temp, // end temperature
                    rate,   // rate
                    new_reqs[i]   // action
                    );
  }

  room_lock.unlock();
}

int Server::allocateRoom(QString usrId)
{
  room_lock.lockForWrite();

  int roomId = -1;
  for (Room &room : req_rooms) {
    if (room.usrId.isEmpty()) {
      user2room[usrId] = room.roomId;
      room.usrId = usrId;
      room.start = QDateTime::currentDateTime();
      roomId = room.roomId;
      break;
    }
  }

  room_lock.unlock();
  return roomId;
}

void Server::handleRequests()
{
  req_lock.lockForWrite();

  qDebug() << "[Server] ";
  QVector<Request> _requests = pipe->getRequests();
  requests = _requests;
  qDebug() << "[Server] ";
  pipe->delRequests(_requests);

  qDebug() << "[Server] handle requests";
  for (Request q : requests) {
    int roomId = -1;
    if (user2room.contains(q.usrId))
      roomId = user2room[q.usrId];
    else
      roomId = allocateRoom(q.usrId);

    if (roomId != -1) {
      if (q.state != rooms[roomId].state && q.state == 3)
        new_reqs[roomId] = 0;
      if (q.state != rooms[roomId].state && q.state == 1)
        new_reqs[roomId] = 1;
      if (qAbs(q.settemp - rooms[roomId].settemp) < 1e-3)
        new_reqs[roomId] = 2;
      if (q.setwdspd != rooms[roomId].setwdspd)
        new_reqs[roomId] = 3;
      req_rooms[roomId].settemp = q.settemp;
      req_rooms[roomId].setwdspd = q.setwdspd;
      req_rooms[roomId].state = q.state;
    }
  }

  req_lock.unlock();
}

void Server::process()
{
  updateRooms();
  uploadRooms();

  updateBillings();
  uploadBillings();

  requestRooms();
}

void Server::checkOut(int roomId)
{
  room_lock.lockForWrite();

  user2room.remove(rooms[roomId].usrId);
  req_rooms[roomId].usrId = "";
  new_reqs[roomId] = true;

  room_lock.unlock();
}

Room Server::getRoom(int roomId)
{
  return rooms[roomId];
}

void Server::updateRooms()
{
  // 只更新rooms 不上传
  room_lock.lockForWrite();

  for (Room room : rooms) {
    // empty room
    if (!room.usrId.isEmpty()) {
      // do nothing
      room.usrId = "";
    }
    // air conditioner ON
    else if (room.state == 1) {
      room.wdspd = room.setwdspd;
      // heating
      if (room.temp < room.settemp) {
        room.mode = 1;
        if (room.wdspd == 1)
          room.temp += info.paraLow;
        else if (room.wdspd == 2)
          room.temp += info.paraMid;
        else if (room.wdspd == 3)
          room.temp += info.paraHigh;
        if (room.temp > room.settemp)
          room.temp = room.settemp;
      }
      // refregerating
      else if (room.temp > room.settemp) {
        room.mode = 0;
        if (room.wdspd == 1)
          room.temp -= info.paraLow;
        else if (room.wdspd == 2)
          room.temp -= info.paraMid;
        else if (room.wdspd == 3)
          room.temp -= info.paraHigh;
        if (room.temp < room.settemp)
          room.temp = room.settemp;
      }
    }
  }

  room_lock.unlock();
}

void Server::updateBillings()
{
  // 只更新billings 不添加新条目 也不上传
  for (int i = 0; i < rooms.size(); i++) {
    if (!rooms[i].usrId.isEmpty()) {
      if (rooms[i].state == 1) {
        // do something to billings[room.roomId]
        billings[i].duration = QDateTime::currentDateTime();
        int64_t secs = billings[i].start.secsTo(billings[i].duration);
        billings[i].costs = billings[i].rate * secs;
        billings[i].endTemp = rooms[i].temp;
      }
    }
  }
}

void Server::uploadRooms()
{
  // 上传room
  qDebug() << "[Server]";
  pipe->updateRooms(rooms);
}

void Server::uploadBillings()
{
  // 上传billings
  qDebug() << "[Server]";
  pipe->updateBillings(billings);
}

void Server::requestRooms()
{
  // 更新rooms 添加新的billings并上传
  room_lock.lockForWrite();

  for (int i = 0; i < rooms.size(); i++) {
    // change the states of the room in response to the requests
    if (new_reqs[i] >= 0) {
      rooms[i].usrId = req_rooms[i].usrId;
      rooms[i].token = req_rooms[i].token;
      rooms[i].settemp = req_rooms[i].settemp;
      rooms[i].setwdspd = req_rooms[i].setwdspd;
      rooms[i].state = req_rooms[i].state;
      rooms[i].start = req_rooms[i].start;

      double rate = 0;
      if (rooms[i].setwdspd == 1)
        rate = info.lowRate;
      else if (rooms[i].setwdspd == 2)
        rate = info.midRate;
      else if (rooms[i].setwdspd == 3)
        rate = info.highRate;

      billings[i] = Billing(
                      billings_cnt++, // billing id
                      rooms[i].roomId,   // room id
                      QDateTime::currentDateTime(), // start
                      QDateTime::currentDateTime(), // duration
                      0.0, // costs
                      rooms[i].wdspd, // windspeed
                      rooms[i].temp, // start temperature
                      rooms[i].temp, // end temperature
                      rate,   // rate
                      new_reqs[i]   // action
                      );
      pipe->addBilling(billings[i]);
      qDebug() << "add billing" << i;

      new_reqs[i] = -1;
    }
  }

  room_lock.unlock();
}

﻿#include "server.h"

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
  timer_h->start(1000);
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
    rooms[i].pwr = 0;
    if (rooms[i].setwdspd == 1)
      rooms[i].pwr = info.lowRate;
    else if (rooms[i].setwdspd == 2)
      rooms[i].pwr = info.midRate;
    else if (rooms[i].setwdspd == 3)
      rooms[i].pwr = info.highRate;

    billings[i] = Billing(
                    billings_cnt++, // billing id
                    rooms[i].roomId,   // room id
                    QDateTime::currentDateTime(), // start
                    QDateTime::currentDateTime(), // duration
                    0.0, // costs
                    rooms[i].wdspd, // windspeed
                    rooms[i].temp, // start temperature
                    rooms[i].temp, // end temperature
                    rooms[i].pwr,   // rate
                    0  // action
                    );
  }

  room_lock.unlock();
}

void Server::handleRequests()
{
  req_lock.lockForWrite();

  qDebug() << "[Server] ";
  QList<Request> _requests = pipe->getRequests();
  for (Request _req : _requests) {
    requests.append(_req);
  }
  qDebug() << "[Server] ";
  pipe->delRequests(_requests);

  qDebug() << "[Server] handle requests";
  for (auto it = requests.begin(); it != requests.end(); ) {
    Request q = *it;

    int roomId = -1;
    if (user2room.contains(q.usrId))
      roomId = user2room[q.usrId];
    else
      roomId = checkIn(q.usrId);

<<<<<<< HEAD
    if (!dispatch.contains(roomId) && dispatch.size() < dispatch_size) {
      dispatch.append(roomId);
    }
    if (dispatch.contains(roomId)) {
      if (roomId != -1) {
        if (q.state != rooms[roomId].state && q.state == 3)
          new_reqs[roomId] = 0;
        if (q.state != rooms[roomId].state && q.state == 1)
          new_reqs[roomId] = 1;
        if (qAbs(q.settemp - rooms[roomId].settemp) > 1e-3)
          new_reqs[roomId] = 2;
        if (q.setwdspd != rooms[roomId].setwdspd)
          new_reqs[roomId] = 3;
        req_rooms[roomId].settemp = q.settemp;
        req_rooms[roomId].setwdspd = q.setwdspd;
        req_rooms[roomId].state = q.state;
      }
      it = requests.erase(it);
    }
    else {
      ++it;
=======
    if (roomId != -1) {
      if (q.state != rooms[roomId].state && q.state == 3)
        new_reqs[roomId] = 0;
      if (q.state != rooms[roomId].state && q.state == 1)
        new_reqs[roomId] = 1;
      if (qAbs(q.settemp - rooms[roomId].settemp) > 1e-3)
        new_reqs[roomId] = 2;
      if (q.setwdspd != rooms[roomId].setwdspd)
        new_reqs[roomId] = 3;
      req_rooms[roomId].settemp = q.settemp;
      req_rooms[roomId].setwdspd = q.setwdspd;
      req_rooms[roomId].state = q.state;
>>>>>>> parent of 2c02f2c... Add files via upload
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

int Server::checkIn(QString usrId)
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

void Server::checkOut(int roomId)
{
  room_lock.lockForWrite();

  user2room.remove(rooms[roomId].usrId);
  req_rooms[roomId] = Room(roomId);
  new_reqs[roomId] = 0;

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

  for (Room &room : rooms) {
    // empty room
    if (room.usrId.isEmpty()) {
      // do nothing
      continue;
    }

    // air conditioner ON
    if (room.state == 1) {
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

    room.cost += room.pwr;
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
    if (new_reqs[i] == 0) {
      rooms[i] = Room(rooms[i].roomId);
    }
    else if (new_reqs[i] > 0) {
      rooms[i].usrId = req_rooms[i].usrId;
      rooms[i].token = req_rooms[i].token;
      rooms[i].settemp = req_rooms[i].settemp;
      rooms[i].setwdspd = req_rooms[i].setwdspd;
      rooms[i].state = req_rooms[i].state;
      rooms[i].start = req_rooms[i].start;
      if (rooms[i].setwdspd == 0)
        rooms[i].pwr = 0.;
      else if (rooms[i].setwdspd == 1)
        rooms[i].pwr = info.lowRate;
      else if (rooms[i].setwdspd == 2)
        rooms[i].pwr = info.midRate;
      else if (rooms[i].setwdspd == 3)
        rooms[i].pwr = info.highRate;

      billings[i] = Billing(
                      billings_cnt++, // billing id
                      rooms[i].roomId,   // room id
                      QDateTime::currentDateTime(), // start
                      QDateTime::currentDateTime(), // duration
                      0.0, // costs
                      rooms[i].wdspd, // windspeed
                      rooms[i].temp, // start temperature
                      rooms[i].temp, // end temperature
                      rooms[i].pwr,   // rate
                      new_reqs[i]   // action
                      );
      pipe->addBilling(billings[i]);
      qDebug() << "add billing for room" << i;

      new_reqs[i] = -1;
    }
  }

  room_lock.unlock();
}

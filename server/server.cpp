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
  connect(timer_h, &QTimer::timeout, this, &Server::process);
  timer_h->start(3000);
}

Server::~Server() {}

void Server::init()
{
  info = pipe->getHost();

  room_lock.lockForWrite();
  rooms = pipe->getRooms();
  req_rooms = rooms;
  for (Room room : rooms) {
    if (!room.usrId.isEmpty()) {
      user2room[room.usrId] = room.roomId;
    }
  }
  new_reqs.resize(rooms.size());
  new_reqs.fill(false);
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
      new_reqs[roomId] = true;
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
  rooms[roomId].usrId = "";
  room_lock.unlock();
}

Room Server::getRoom(int roomId)
{
  return rooms[roomId];
}

void Server::updateRooms()
{
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
  // TODO:
  // 针对每个room更新billing
  // room.usrId如果为空则是空房间 不计费
  // 只更新每个room时间最新的一条记录
  // newBilling与rooms一一对应
  // 表示对于这个房间是否需要增加新的一条记录
  // 如果newBilling[i]为真 则对于room[i]应该新加一条billing记录
  for (Room room : rooms) {
    if (!room.usrId.isEmpty()) {
      if (room.state == 1) {

      }
    }
  }
}

void Server::uploadRooms()
{
  qDebug() << "[Server]";
  pipe->updateRooms(rooms);
}

void Server::uploadBillings()
{

}

void Server::requestRooms()
{
  for (int i = 0; i < rooms.size(); i++) {
    if (new_reqs[i]) {
      rooms[i].usrId = req_rooms[i].usrId;
      rooms[i].token = req_rooms[i].token;
      rooms[i].settemp = req_rooms[i].settemp;
      rooms[i].setwdspd = req_rooms[i].setwdspd;
      rooms[i].state = req_rooms[i].state;
      rooms[i].start = req_rooms[i].start;
    }
  }
}

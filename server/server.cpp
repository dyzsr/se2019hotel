#include "server.h"

#include <QTimer>
#include <QDebug>

Server::Server(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance())
{
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &Server::process);
  timer->start(3000);
}

Server::~Server() {}

void Server::init()
{
  room_lock.lockForWrite();
  rooms = pipe->getRooms();
  for (Room room : rooms) {
    if (!room.usrId.isEmpty()) {
      user2room[room.usrId] = room.roomId;
    }
  }
  need2AddDocument.resize(rooms.size());
  need2AddDocument.fill(false);
  room_lock.unlock();
}

int Server::allocateRoom(QString usrId)
{
  room_lock.lockForWrite();
  int roomId = -1;
  for (Room room : rooms) {
    if (room.usrId.isEmpty()) {
      user2room[usrId] = room.roomId;
      room.usrId = usrId;
      roomId = room.roomId;
      break;
    }
  }
  room_lock.unlock();
  return roomId;
}

void Server::process()
{
  fetchRequests();
  handleRequests();
  updateRooms();
  updateBillings();
}

void Server::checkOut(int roomId)
{
  // TODO
}

void Server::fetchRequests()
{
  req_lock.lockForWrite();
  qDebug() << "[Server] ";
  QVector<Request> _requests = pipe->getRequests();
  requests = _requests;
  qDebug() << "[Server] ";
  pipe->delRequests(_requests);
  req_lock.unlock();
}

void Server::handleRequests()
{
  req_lock.lockForRead();
  qDebug() << "[Server] handle requests";
  for (Request q : requests) {
    int roomId = -1;
    if (user2room.contains(q.usrId))
      roomId = user2room[q.usrId];
    else
      roomId = allocateRoom(q.usrId);

    if (roomId != -1) {
      rooms[roomId].settemp = q.settemp;
      rooms[roomId].setwdspd = q.setwdspd;
      rooms[roomId].state = q.state;
    }
  }
  req_lock.unlock();
}

void Server::updateRooms()
{
  // TODO:
  // 按照settemp setwdspd state的值
  // 更新temp wdspd
  // room.usrId如果为空则是空房间
}

void Server::updateBillings()
{
  // TODO:
  // 针对每个room更新billing
  // room.usrId如果为空则是空房间 不计费
  // 只更新每个room时间最新的一条记录
  // need2AddDocument与rooms一一对应
  // 表示对于这个房间是否需要增加新的一条记录
  // 如果need2AddDocument[i]为真 则对于room[i]应该新加一条billing记录
}

void Server::uploadData()
{

}

QVector<Billing> Server::getBillings(QDateTime start, QDateTime end)
{
  int front = 0, rear = billings.size();
  if (start.isValid()) {
    for (Billing billing : billings) {
      if (billing.start >= start)
        break;
      front++;
    }
  }
  if (end.isValid()) {
    for (auto it = billings.rbegin();
         it != billings.rend(); ++it) {
      if (it->start <= end)
        break;
      rear--;
    }
  }
  return billings.mid(front, rear - front);
}

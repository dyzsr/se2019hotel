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
  connect(timer_h, &QTimer::timeout, this, &Server::fetchRequests);
  timer_h->start(1000);
}

Server::~Server() {}

void Server::init()
{
  info = pipe->getHost();

  room_lock.lockForWrite();

  // download rooms from database
  rooms = pipe->getRooms();
  req_rooms = rooms.mid(0);
  for (Room room : rooms) {
    if (!room.usrId.isEmpty()) {
      user2room[room.usrId] = room.roomId;
    }
  }
  // init flags for new requests of each room
  new_reqs.resize(rooms.size());
  new_reqs.fill(-1);

  // init empty billings for each room
  billings_cnt = pipe->getCurrentBillingId();
  billings.resize(rooms.size());
  // init billing
  for (int i = 0; i < rooms.size(); i++) {
    rooms[i].pwr = getRate(rooms[i].setwdspd);

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

void Server::fetchRequests()
{
  req_lock.lockForWrite();

  // fetch requests from database, store them to local
  QList<Request> _requests = pipe->getRequests();
  requests = _requests;
  pipe->delRequests(_requests);

  qDebug() << "fetch requests";

  // map requests to rooms
  for (Request q : requests) {
    int roomId = -1;
    // if the user has a room currently
    if (user2room.contains(q.usrId))
      roomId = user2room[q.usrId];
    else  // allocate a new room
      roomId = checkIn(q.usrId);

    if (roomId != -1) {

      // turn on the request flag of the corresponding room
      // TODO
      if (q.state == 0) {
        req_rooms[roomId].state = 0;
        new_reqs[roomId] = 0;
      } else if (q.state == 2) {
        if (req_rooms[roomId].state == 0)
          req_rooms[roomId].state = 2;
        new_reqs[roomId] = 1;
      }
      if (qAbs(q.settemp - rooms[roomId].settemp) > 1e-3)
        new_reqs[roomId] = 2;
      if (q.setwdspd != rooms[roomId].setwdspd)
        new_reqs[roomId] = 3;

      // set the request of the room
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

  updateService();
}

int Server::checkIn(QString usrId)
{
  room_lock.lockForWrite();

  int roomId = -1;
  for (int i = 0; i < req_rooms.size(); i++) {
    // find an empty room for the user
    if (req_rooms[i].usrId.isEmpty()) {
      user2room[usrId] = req_rooms[i].roomId;
      req_rooms[i].usrId = usrId;
      req_rooms[i].start = QDateTime::currentDateTime();
      roomId = req_rooms[i].roomId;
      break;
    }
  }

  room_lock.unlock();
  return roomId;
}

void Server::checkOut(int roomId)
{
  room_lock.lockForWrite();

  // remove the user from the room
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
    // 房间在拥有服务对象时才可以接收服务
    if (services.contains(room.roomId)) {
      room.wdspd = room.setwdspd;
      // 制热
      if (room.temp < room.settemp) {
        room.mode = 1;
        room.temp += getPara(room.wdspd);
        room.temp = qMin(room.temp, room.settemp);
        if (serviceCompleted(room.roomId)) {
          // 服务完成 删除服务对象
          services.removeOne(room.roomId);
        }
      }
      // 制冷
      else if (room.temp > room.settemp) {
        room.mode = 0;
        room.temp -= getPara(room.wdspd);
        room.temp = qMax(room.temp, room.settemp);
        if (serviceCompleted(room.roomId)) {
          // 服务完成 删除服务对象
          services.removeOne(room.roomId);
        }
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
  pipe->updateRooms(rooms);
}

void Server::uploadBillings()
{
  // 上传billings
  for (int i = 0; i < rooms.size(); i++) {
    if (!rooms[i].usrId.isEmpty()) {
      pipe->updateBilling(billings[i]);
    }
  }
}

void Server::updateService()
{
  // 更新rooms 添加新的billings并上传
  room_lock.lockForWrite();

  // 添加新的服务对象
  for (int i = 0; i < new_reqs.size(); i++) {
    if (new_reqs[i] >= 0 && services.size() < MAX_SERVICE_NUM) {
      services.append(i);
    }
  }

  for (int i : services) {
    // 为房间提供服务
    if (new_reqs[i] >= 0) {
      rooms[i].usrId = req_rooms[i].usrId;
      rooms[i].token = req_rooms[i].token;
      rooms[i].settemp = req_rooms[i].settemp;
      rooms[i].setwdspd = req_rooms[i].setwdspd;
      rooms[i].state = req_rooms[i].state;
      rooms[i].start = req_rooms[i].start;
      rooms[i].pwr = getRate(rooms[i].setwdspd);

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

      new_reqs[i] = -1;
    }
  }

  room_lock.unlock();
}

bool Server::serviceCompleted(int roomId)
{
  return (qAbs(rooms[roomId].temp - req_rooms[roomId].settemp) <= 1e-3);
}

double Server::getRate(int wdspd)
{
  if (wdspd == 1)
    return info.lowRate;
  if (wdspd == 2)
    return info.midRate;
  if (wdspd == 3)
    return info.highRate;
  // wdspd == 0
  return 0.;
}

double Server::getPara(int wdspd)
{
  if (wdspd == 1)
    return info.paraLow;
  if (wdspd == 2)
    return info.paraMid;
  if (wdspd == 3)
    return info.paraHigh;
  return 0.;
}

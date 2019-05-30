#include "server.h"

#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QTime>
#pragma execution_character_set("utf-8")

Server::Server(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance())
{
  pipe->turnOnHost();
}

Server::~Server()
{
  pipe->turnOffHost();
}

void Server::init()
{
  info = pipe->getHost();
  users = pipe->getUsers();

  room_lock.lockForWrite();

  // 从数据库读取房间数据
  rooms = pipe->getRooms();
  for (Room room : rooms) {
    if (!room.usrId.isEmpty()) {
      user2room[room.usrId] = room.roomId;
    }
  }
  // 从房间初始化调度对象
  dsps.resize(rooms.size());
  for (int i = 0; i < rooms.size(); i++) {
    dsps[i].update(rooms[i]);
  }

  // 初始化账单
  billings_cnt = pipe->getCurrentBillingId();
  billings.resize(rooms.size());

  room_lock.unlock();

  emit sgn_init(rooms.size());

  QTimer *timer_p = new QTimer(this);
  connect(timer_p, &QTimer::timeout, this, &Server::process);
  timer_p->start(1000);

  QTimer *timer_h = new QTimer(this);
  connect(timer_h, &QTimer::timeout, this, &Server::fetchRequests);
  timer_h->start(1000);
}

void Server::fetchRequests()
{
  room_lock.lockForWrite();
//  qDebug() << QTime::currentTime() << "fetch requests start";

  // 从数据库读取请求 保存在本地
  QList<Request> requests = pipe->getRequests();
  pipe->delRequests(requests);

  // 处理读取的请求
  for (Request q : requests) {
    if (!user2room.contains(q.usrId)) {
      // 新用户
      checkIn(q.usrId);
    }

    int roomId = user2room[q.usrId];

    // 关机
    if (q.state == 0) {
      rooms[roomId].state = 0;
      dsps[roomId].hasRequest = false;
      dsps[roomId].update(rooms[roomId]);
      services.removeOne(roomId);
    }
    // 除关机外的请求
    else {
      dsps[roomId].hasRequest = true;
      dsps[roomId].requestType = 1;
      dsps[roomId].state = 3;
      dsps[roomId].waitingTime = waiting_time;

      // 设置温度
      if (!tempInRange(q.settemp, dsps[roomId].settemp, 1e-3)) {
        dsps[roomId].requestType = 2;
        dsps[roomId].settemp = q.settemp;
      }
      // 设置风速
      if (q.setwdspd != dsps[roomId].setwdspd) {
        dsps[roomId].requestType = 3;
        dsps[roomId].setwdspd = q.setwdspd;
      }
    }
  }

//  qDebug() << QTime::currentTime() << "fetch requests finish";
  room_lock.unlock();
}

void Server::process()
{
  room_lock.lockForWrite();
  updateRooms();
  updateService();
  room_lock.unlock();
}

bool Server::checkInFromServer(int roomId, QString usrId)
{
  room_lock.lockForWrite();

  if (!rooms[roomId].usrId.isEmpty() || user2room.contains(usrId)) {
    room_lock.unlock();
    return false;
  }

  user2room[usrId] = roomId;
  rooms[roomId].usrId = usrId;
  rooms[roomId].settemp = info.defaultTemp;
  rooms[roomId].temp = info.defaultTemp;
  rooms[roomId].setwdspd = info.defaultWdspd;
  rooms[roomId].wdspd = info.defaultWdspd;
  rooms[roomId].state = 2;
  rooms[roomId].mode = 0;
  rooms[roomId].start = QDateTime::currentDateTime();
  rooms[roomId].duration = QDateTime::currentDateTime();
  rooms[roomId].pwr = 0;
  rooms[roomId].cost = 0;
  dsps[roomId].update(rooms[roomId]);

  pipe->updateRoom(rooms[roomId]);

  qDebug() << "check in by server";
  room_lock.unlock();
  return true;
}

void Server::checkIn(QString usrId)
{
  for (int i = 0; i < rooms.size(); i++) {
    // 寻找空房间并分配
    if (rooms[i].usrId.isEmpty()) {
      user2room[usrId] = rooms[i].roomId;
      rooms[i].usrId = usrId;
      rooms[i].settemp = info.defaultTemp;
      rooms[i].temp = info.defaultTemp;
      rooms[i].setwdspd = info.defaultWdspd;
      rooms[i].wdspd = info.defaultWdspd;
      rooms[i].state = 2;
      rooms[i].mode = 0;
      rooms[i].start = QDateTime::currentDateTime();
      rooms[i].duration = QDateTime::currentDateTime();
      rooms[i].pwr = 0;
      rooms[i].cost = 0;
      dsps[i].update(rooms[i]);

      pipe->updateRoom(rooms[i]);
      break;
    }
  }
}

void Server::checkOut(int roomId)
{
  room_lock.lockForWrite();
  qDebug() << QTime::currentTime() << "checkout start";

  // 从用户房间映射表中删除用户
  user2room.remove(rooms[roomId].usrId);
  // 保留此房间上一个用户的服务起始时间
  rooms[roomId].usrId = "";
  rooms[roomId].settemp = 25.;
  rooms[roomId].temp = 25.;
  rooms[roomId].setwdspd = 0;
  rooms[roomId].wdspd = 0;
  rooms[roomId].mode = 0;
  rooms[roomId].state = 0;
  rooms[roomId].duration = QDateTime::currentDateTime();
  dsps[roomId].update(rooms[roomId]);
  dsps[roomId].hasRequest = false;
  if (services.contains(roomId)) {
    services.removeOne(roomId);
  }
  pipe->updateRoom(rooms[roomId]);

  qDebug() << QTime::currentTime() << "checkout finish";
  room_lock.unlock();
}

QStringList Server::getUsrIds()
{
  QStringList usrIds;
  for (User user : users) {
    usrIds.append(user.id);
  }
  return usrIds;
}

bool Server::addNewUser(QString usrId, QString pswd)
{
  return false;
}

Room Server::getRoom(int roomId)
{
  return rooms[roomId];
}

QString Server::getUsrId(int roomId)
{
  if (rooms[roomId].usrId.isEmpty()) {
    return "无人";
  }
  return rooms[roomId].usrId;
}

void Server::updateRooms()
{
  // 更新rooms

  for (Room &room : rooms) {
    if (!room.usrId.isEmpty()) {
      room.temp = pipe->getRoomTemp(room.roomId);
      room.wdspd = room.setwdspd;

      // 房间在拥有服务对象时才可以接收服务
      // state == 1 OR state == 3
      if (services.contains(room.roomId) &&
          !tempInRange(room.temp, room.settemp, 1e-3)) {
        // 制热
        if (room.temp < room.settemp) {
          room.mode = 1;
          room.temp += getPara(room.wdspd);
          room.temp = qMin(room.temp, room.settemp);
        }
        // 制冷
        else if (room.temp > room.settemp) {
          room.mode = 0;
          room.temp -= getPara(room.wdspd);
          room.temp = qMax(room.temp, room.settemp);
        }

        // 更新账单
        billings[room.roomId].duration = QDateTime::currentDateTime();
        int64_t secs = billings[room.roomId].start.secsTo(billings[room.roomId].duration);

        // 房间记账
        billings[room.roomId].costs = billings[room.roomId].rate * secs;
        billings[room.roomId].endTemp = rooms[room.roomId].temp;
        room.cost = dsps[room.roomId].cost + room.pwr * secs;

        // 更新服务时间
        dsps[room.roomId].serviceTime += 1;

        // 上传至数据库
        pipe->updateRoom(room);
        pipe->updateBilling(billings[room.roomId]);
      }
      // 在等待队列中等待
      else if (room.state == 3) {
        // 更新等待时间
        if (dsps[room.roomId].waitingTime > 0)
          dsps[room.roomId].waitingTime -= 1;
      }
      else if (room.state == 2 &&
               !tempInRange(room.temp, room.settemp, 1.) &&
               !dsps[room.roomId].hasRequest) {
        dsps[room.roomId].state = 3;
        dsps[room.roomId].hasRequest = true;
        dsps[room.roomId].requestType = 2;
        dsps[room.roomId].waitingTime = waiting_time;
      }

      room.duration = QDateTime::currentDateTime();
      pipe->updateRoomExceptTemp(room);
    }
  }

}

void Server::updateService()
{
  // 更新rooms 添加新的billings并上传

  // 清除已完成服务的对象
  for (auto it = services.begin(); it != services.end(); ) {
    int i = *it;
    // state == 1 表示再无请求
    if (serviceCompleted(i) && dsps[i].state == 1) {
      // 服务完成 删除服务对象
      it = services.erase(it);
      dsps[i].serviceTime = 0;
      dsps[i].hasRequest = false;
      rooms[i].state = 2;
    }
    else
      ++it;
  }

  // 调度服务对象
  while (services.size() < max_nr_service) {
    // 寻找等待队列中最优先的调度对象
    Dispatchable top_dsp = Dispatchable();
    for (int i = 0; i < dsps.size(); i++) {
      if (!services.contains(dsps[i].roomId) && dsps[i] < top_dsp)
        top_dsp = dsps[i];
    }
    if (top_dsp.hasRequest) {
      services.append(top_dsp.roomId);
    }
    else
      break;
  }

  if (services.size() == max_nr_service) {
    for (int j = 0; j < max_nr_service; j++) {
      // 寻找最可能被替换的服务对象
      Dispatchable dsp_swap_out = dsps[services[0]];
      for (int i = 1; i < services.size(); i++) {
        if (dsp_swap_out < dsps[services[i]])
          dsp_swap_out = dsps[services[i]];
      }
      // 寻找等待队列中最优先的调度对象
      Dispatchable top_dsp = Dispatchable();
      for (int i = 0; i < dsps.size(); i++) {
        if (!services.contains(dsps[i].roomId) && dsps[i] < top_dsp)
          top_dsp = dsps[i];
      }

      // 替换
      if (top_dsp < dsp_swap_out) {
        services.removeOne(dsp_swap_out.roomId);
        services.append(top_dsp.roomId);
        // 移入
        dsps[top_dsp.roomId].waitingTime = 0;
        dsps[top_dsp.roomId].serviceStart = QDateTime::currentDateTime();
        // 从等待队列中取出
        dsps[top_dsp.roomId].hasRequest = false;
        // 移出
        dsps[dsp_swap_out.roomId].waitingTime = waiting_time;
        dsps[dsp_swap_out.roomId].serviceTime = 0;
        // state == 3 表示进入等待队列
        dsps[dsp_swap_out.roomId].state = 3;
      }
      else
        break;
    }
  }

  // 判断服务对象是否需要新开账单
  for (int i : services) {
    // state == 3 表示有新的请求
    if (dsps[i].state == 3) {
      rooms[i].usrId = dsps[i].usrId;
      rooms[i].settemp = dsps[i].settemp;
      rooms[i].setwdspd = dsps[i].setwdspd;
      // state == 1 表示进入服务队列 且已经记录下了最新请求
      rooms[i].state = dsps[i].state = 1;
      rooms[i].start = dsps[i].start;
      rooms[i].pwr = getRate(rooms[i].setwdspd);

      dsps[i].cost = rooms[i].cost;

      billings[i] = Billing(
                      0, // billing id
                      rooms[i].roomId,   // room id
                      QDateTime::currentDateTime(), // start
                      QDateTime::currentDateTime(), // duration
                      0.0, // costs
                      rooms[i].setwdspd, // windspeed
                      rooms[i].temp, // start temperature
                      rooms[i].temp, // end temperature
                      rooms[i].pwr,   // rate
                      dsps[i].requestType   // action
                      );
      billings[i].billingId = pipe->addBilling(billings[i]);
    }
  }
  qDebug() << QTime::currentTime() << "serv" << services;
}

bool Server::serviceCompleted(int roomId)
{
  return tempInRange(rooms[roomId].temp, rooms[roomId].settemp, 1e-3);
}

bool Server::tempInRange(double temp1, double temp2, double range)
{
  return (qAbs(temp1 - temp2) < range);
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

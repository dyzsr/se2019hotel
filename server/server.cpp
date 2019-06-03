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
  _mode = 0;

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
    if (!rooms[i].usrId.isEmpty()) {
      if (rooms[i].state >= 1) {
        if (serviceCompleted(i)) {
          rooms[i].state = 2;
          dsps[i].hasRequest = false;
        }
        else {
          rooms[i].state = 3;
          dsps[i].hasRequest = true;
          dsps[i].requestType = 1;
        }
        dsps[i].update(rooms[i]);
      }
    }
  }
  pipe->updateRooms(rooms);

  // 初始化账单
  billings_cnt = pipe->getCurrentBillingId();
  billings.resize(rooms.size());

  // 第一次调度
  updateService();

  emit sgn_init(rooms.size());

  QTimer *timer_h = new QTimer(this);
  connect(timer_h, &QTimer::timeout, this, &Server::fetchRequests);
  timer_h->start(1000);

  QTimer *timer_r = new QTimer(this);
  connect(timer_r, &QTimer::timeout, this, &Server::updateRooms);
  timer_r->start(1000);
}

void Server::fetchRequests()
{
  qDebug() << QTime::currentTime() << "fetch requests start";

  // 从数据库读取请求 保存在本地
  QList<Request> requests = pipe->getRequests();
  pipe->delRequests(requests);

  // 处理读取的请求
  for (Request q : requests) {
    // 未知用户跳过
    if (!user2room.contains(q.usrId)) {
      continue;
    }

    int roomId = user2room[q.usrId];

    // 关机
    if (q.state == 0) {
      rooms[roomId].state = 0;
      dsps[roomId].hasRequest = false;
      dsps[roomId].update(rooms[roomId]);
    }
    // 除关机外的请求
    else {
      dsps[roomId].hasRequest = true;
      dsps[roomId].requestType = 1;
      if (rooms[roomId].state != 1) {
        if (rooms[roomId].state != 3)
          dsps[roomId].waitingTime = waiting_time;

        rooms[roomId].state = 3;
      }

      // 设置温度
      if (q.settemp >= 0 &&
          !tempInRange(q.settemp, rooms[roomId].settemp, 1e-3)) {
        dsps[roomId].requestType = 2;
        rooms[roomId].settemp = q.settemp;
      }
      // 设置风速
      if (q.setwdspd != -1 &&
          q.setwdspd != rooms[roomId].wdspd) {
        dsps[roomId].requestType = 3;
        rooms[roomId].wdspd = q.setwdspd;
        rooms[roomId].pwr = getRate(rooms[roomId].wdspd);
      }
      dsps[roomId].update(rooms[roomId]);

      qDebug() << "request: " << rooms[roomId].roomId
               << rooms[roomId].state << rooms[roomId].settemp << rooms[roomId].wdspd;
    }
  }

  qDebug() << QTime::currentTime() << "fetch requests finish";

  if (requests.size())
    updateService();
}

bool Server::checkInFromServer(int roomId, QString usrId)
{
  if (!rooms[roomId].usrId.isEmpty() || user2room.contains(usrId)) {
    return false;
  }

  user2room[usrId] = roomId;
  rooms[roomId].usrId = usrId;
  rooms[roomId].settemp = info.defaultTemp;
  rooms[roomId].temp = info.defaultTemp;
  rooms[roomId].setwdspd = info.defaultWdspd;
  rooms[roomId].wdspd = info.defaultWdspd;
  rooms[roomId].state = 0;
  rooms[roomId].mode = 0;
  rooms[roomId].start = QDateTime::currentDateTime();
  rooms[roomId].duration = QDateTime::currentDateTime();
  rooms[roomId].pwr = 0;
  rooms[roomId].cost = 0;
  dsps[roomId].update(rooms[roomId]);
  dsps[roomId].hasRequest = true;

  pipe->updateRoom(rooms[roomId]);

  qDebug() << "check in" << roomId << usrId;

  updateService();
  return true;
}

bool Server::checkOut(int roomId)
{
  // 从用户房间映射表中删除用户
  user2room.remove(rooms[roomId].usrId);
  // 保留此房间上一个用户的服务起始时间
  rooms[roomId].usrId = "";
  rooms[roomId].temp = rooms[roomId].settemp = info.defaultTemp;
  rooms[roomId].wdspd = rooms[roomId].setwdspd = 0;
  rooms[roomId].mode = 0;
  rooms[roomId].state = 0;
  rooms[roomId].duration = QDateTime::currentDateTime();
  dsps[roomId].update(rooms[roomId]);
  dsps[roomId].hasRequest = false;
  bool success = pipe->updateRoom(rooms[roomId]);

  qDebug() << QTime::currentTime() << "checkout" << roomId;

  updateService();
  return success;
}

void Server::setMode(int _mode)
{
  this->_mode = _mode;
}

QStringList Server::getUsrIds()
{
  QStringList usrIds;
  users = pipe->getUsers();

  for (User user : users) {
    usrIds.append(user.id);
  }
  return usrIds;
}

QStringList Server::getAvailUsrIds()
{
  QStringList usrIds;
  QStringList excludes;

  for (Room room : rooms) {
    if (!room.usrId.isEmpty())
      excludes.append(room.usrId);
  }

  for (User user : users) {
    if (!excludes.contains(user.id))
      usrIds.append(user.id);
  }
  return usrIds;
}

bool Server::addUser(QString usrId, QString pswd)
{
  for (User user : users) {
    if (usrId == user.id) {
      return false;
    }
  }
  bool success = pipe->addUser(User(usrId, pswd));
  return success;
}

bool Server::delUser(QString usrId, QString pswd)
{
  for (User user : users) {
    if (usrId == user.id) {
      bool success = pipe->delUser(User(usrId, pswd));
      return success;
    }
  }
  return false;
}

QVector<Room> Server::getAllRooms()
{
  return rooms;
}

Room Server::getRoom(int roomId)
{
  return rooms[roomId];
}

QString Server::getUsrId(int roomId)
{
  return rooms[roomId].usrId;
}

int Server::getNrRooms()
{
  return rooms.size();
}

void Server::serve(int i)
{
  if (dsps[i].hasRequest) {
    // 添加新的账单
    rooms[i].pwr = getRate(rooms[i].setwdspd);
    dsps[i].update(rooms[i]);

    billings[i] = Billing(
                    0, // billing id
                    rooms[i].roomId,   // room id
                    QDateTime::currentDateTime(), // start
                    QDateTime::currentDateTime(), // duration
                    0.0, // costs
                    rooms[i].wdspd, // windspeed
                    rooms[i].temp, // start temperature
                    rooms[i].temp, // end temperature
                    rooms[i].pwr,   // rate
                    dsps[i].requestType   // action
                    );
    billings[i].billingId = pipe->addBilling(billings[i]);

    dsps[i].hasRequest = false;
  }

  // 温度未到设定温度
  if (!tempInRange(rooms[i].temp, rooms[i].settemp, 1e-3)) {
    // 制热
    if ((_mode == 1 || _mode == 2) && rooms[i].temp < rooms[i].settemp) {
      rooms[i].temp += getPara(rooms[i].wdspd);
      rooms[i].temp = qMin(rooms[i].temp, rooms[i].settemp);
    }
    // 制冷
    if ((_mode == 0 || _mode == 2) && rooms[i].temp > rooms[i].settemp) {
      rooms[i].temp -= getPara(rooms[i].wdspd);
      rooms[i].temp = qMax(rooms[i].temp, rooms[i].settemp);
    }

    // 更新账单
    billings[i].duration = QDateTime::currentDateTime();
    int64_t secs = billings[i].start.secsTo(billings[i].duration);

    // 房间记账
    billings[i].costs = billings[i].rate * secs;
    billings[i].endTemp = rooms[i].temp;
    rooms[i].cost = dsps[i].cost + rooms[i].pwr * secs;

    // 更新服务时间
    dsps[i].serviceTime += 1;
  }
}

void Server::wait(int i)
{
  // 更新等待时间
  dsps[i].hasRequest = true;
  if (dsps[i].waitingTime > 0)
    dsps[i].waitingTime -= 1;
}

void Server::standby(int i)
{
  if ((_mode == 0 && rooms[i].temp - rooms[i].settemp >= 1.) ||
      (_mode == 1 && rooms[i].settemp - rooms[i].temp >= 1.) ||
      _mode == 2) {
    rooms[i].state = 3;
    dsps[i].hasRequest = true;
    dsps[i].requestType = 2;
  }
}

Dispatchable Server::findFirst()
{
  Dispatchable first = Dispatchable();
  for (int i = 0; i < dsps.size(); i++) {
    if (rooms[i].state == 3 && dsps[i] < first)
      first = dsps[i];
  }
  return first;
}

Dispatchable Server::findLast()
{
  bool flag = true;
  Dispatchable last;
  for (int i = 0; i < dsps.size(); i++) {
    if (rooms[i].state == 1) {
      if (flag || last < dsps[i]) {
        last = dsps[i];
        flag = false;
      }
    }
  }
  return last;
}

void Server::updateRooms()
{
  // 更新rooms
  qDebug() << QTime::currentTime() << "room";

  auto temps = pipe->getRoomTemps();

  for (int i = 0; i < rooms.size(); i++) {
    rooms[i].temp = temps[i];

    if (!rooms[i].usrId.isEmpty()) {
      // 设置空调模式
      if (_mode == 0)
        rooms[i].mode = 0;
      else if (_mode == 1)
        rooms[i].mode = 1;
      else
        rooms[i].mode = rooms[i].temp < rooms[i].settemp;

      // 房间在拥有服务对象时才可以接收服务
      if (rooms[i].state == 1) {
        serve(i);
      }
      // 在等待队列中等待
      else if (rooms[i].state == 3) {
        wait(rooms[i].roomId);
      }
      // 待机状态 判断是否重发请求
      else if (rooms[i].state == 2 &&
               !tempInRange(rooms[i].temp, rooms[i].settemp, 1.) &&
               !dsps[rooms[i].roomId].hasRequest) {
        standby(rooms[i].roomId);
      }

      rooms[i].duration = QDateTime::currentDateTime();
    }
  }
  pipe->updateRooms(rooms);
  pipe->updateBillings(billings);

  QVector<int> services;
  for (int i = 0; i < rooms.size(); i++) {
    if (rooms[i].state == 1)
      services.append(i);
  }
  qDebug() << QTime::currentTime() << "in service" << nr_services;
  qDebug() << services;

  // 调度
  updateService();
}

void Server::updateService()
{
  // count services
  nr_services = 0;
  for (int i = 0; i < rooms.size(); i++) {
    if (rooms[i].state == 1)
      nr_services++;
  }

  for (int i = 0; i < rooms.size(); i++) {
    if (rooms[i].state == 1 && serviceCompleted(i)) {
      rooms[i].state = 2;
      nr_services--;
      qDebug() << i << "out" << nr_services;
    }
  }

  // 调度服务对象
  while (nr_services < max_nr_service) {
    // 寻找等待队列中最优先的调度对象
    Dispatchable swap_in = findFirst();
    if (!swap_in.usrId.isEmpty()) {
      int i = swap_in.roomId;
      rooms[i].state = 1;
      dsps[i].waitingTime = 0;
      dsps[i].serviceTime = 0;

      nr_services++;
      qDebug() << i << "in" << nr_services;
    }
    else break;
  }

  if (nr_services == max_nr_service) {
    for (int k = 0; k < max_nr_service; k++) {
      // 寻找最可能被替换的服务对象
      Dispatchable swap_out = findLast();
      // 寻找等待队列中最优先的调度对象
      Dispatchable swap_in = findFirst();

      // 替换
      if (!swap_in.usrId.isEmpty() && swap_in < swap_out) {
        int i = swap_out.roomId;
        rooms[i].state = 3;
        dsps[i].waitingTime = waiting_time;
        dsps[i].serviceTime = 0;

        int j = swap_in.roomId;
        rooms[j].state = 1;
        dsps[j].waitingTime = 0;
        dsps[j].serviceTime = 0;
        qDebug() << i << "out" << j << "in" << nr_services;
      }
      else break;
    }
  }

  qDebug() << QTime::currentTime() << "update services";
}

bool Server::serviceCompleted(int roomId)
{
  return ((_mode == 0 && rooms[roomId].temp <= rooms[roomId].settemp) ||
          (_mode == 1 && rooms[roomId].temp >= rooms[roomId].settemp) ||
          tempInRange(rooms[roomId].temp, rooms[roomId].settemp, 1e-3));
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

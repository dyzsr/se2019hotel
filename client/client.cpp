﻿#include "client.h"

#include <QTimer>
#include <QDebug>

Client::Client(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance()),
  timer(this),
  recoverTimer(this)
{
  info = pipe->getHost();
  emit sgn_init(info.minTemp, info.maxTemp);

  Q_ASSERT(pipe != nullptr);
  connect(&recoverTimer, &QTimer::timeout, this, &Client::forceRoomChanged);
  recoverFlag = false;
}

Client::~Client()
{
}

void Client::fetchData()
{
  room = pipe->getRoom(user.id);
  if (room.usrId.isEmpty())
    emit sgn_disable();
  else
    emit sgn_refresh(room);
}

void Client::fetchDataAndCheck()
{
    fetchData();
    recoverTemp();
}

bool Client::signIn(QString usrId, QString passwd)
{
  user = pipe->getUser(usrId);
  if (user.pswd == passwd) {
    room = pipe->getRoom(usrId);

    if (room.usrId == usrId) {
      // 设置房间初始温度
      pipe->updateRoomTemp(room.roomId, getDefaultTemp());

      connect(&timer, &QTimer::timeout, this, &Client::fetchDataAndCheck);
      timer.start(500);
      qDebug() << QTime::currentTime() << "sign in";
      return true;
    }
    else {
      qDebug() << "No room assigned for" << usrId;
    }
  }
  qDebug() << "input: " << usrId << " " << passwd;
  qDebug() << "database: " << user.id << " " << user.pswd;
  return false;
}

bool Client::signOut()
{
  timer.stop();
  qDebug() << QTime::currentTime() << "sign out";
  return true;
}

void Client::getInfo(int &state, double &settemp, int &setwdspd)
{
  state = info.state > 0 ? 1 : 0;
  settemp = info.defaultTemp;
  setwdspd = info.defaultWdspd;
}

void Client::sendRequest(int state, double settemp, int setwdspd)
{
  this->_wdspd = setwdspd;
  pipe->sendRequest(Request(0, user.id, state ? 1 : 0, settemp, setwdspd));
}

void Client::recoverTemp()
{

    bool recoverCondition = (room.state == 0 || room.state == 2);
    if (recoverCondition)
    {
        if (!recoverFlag)
        {
            recoverTimer.start(3000);
            recoverFlag = true;
        }
    }
    else
    {
        recoverTimer.stop();
        recoverFlag = false;
    }
}

void Client::forceRoomChanged()
{
    double fixedTemp = getDefaultTemp();
    double fixedDiff = 0.008333;
    fetchData();
    if (fixedTemp - room.temp > 0.000001)
    {
        if (room.temp <= fixedTemp - fixedDiff)
            pipe->updateRoomTemp(room.roomId, room.temp+fixedDiff);
        else
            pipe->updateRoomTemp(room.roomId, fixedTemp);
    }
    else
    {
        if (room.temp >= fixedTemp + fixedDiff)
            pipe->updateRoomTemp(room.roomId, room.temp-fixedDiff);
        else
            pipe->updateRoomTemp(room.roomId, fixedTemp);
    }

    if (room.state >= 1) {
      if ((room.mode == 0 && room.temp - room.settemp >= 1.) ||
          (room.mode == 1 && room.settemp - room.temp >= 1.)) {
        pipe->sendRequest(Request(0, room.usrId, 1, room.settemp, this->_wdspd));
      }
    }
}

int Client::getDefaultTemp()
{
    int setTime = emit sgn_getRecoverSpeed();
    return setTime;
}

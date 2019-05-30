#include "client.h"

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
    connect(&timer, &QTimer::timeout, this, &Client::fetchDataAndCheck);
    timer.start(2000);
    qDebug() << QTime::currentTime() << "sign in";
    return true;
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
  pipe->sendRequest(Request(0, user.id, state ? 1 : 0, settemp, setwdspd));
}

void Client::recoverTemp()
{

    bool recoverCondition = (room.state == 0 || room.state == 2);
    if (recoverCondition)
    {
        if (!recoverFlag)
        {
            recoverTimer.start(getRecoverTime()*1000);
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
    double fixedTemp = 30;
    fetchData();
    if (room.temp <= fixedTemp - 1)
        pipe->updateRoomTemp(room.roomId, room.temp+1);
    else if (room.temp < fixedTemp)
        pipe->updateRoomTemp(room.roomId, fixedTemp);
}

int Client::getRecoverTime()
{
    int setTime = emit sgn_getRecoverSpeed();
    return setTime;
}

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
    pipe->sendRequest(Request(0, user.id, 1, 25, 1));

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

void Client::getIn(int state, double settemp, int setwdspd)
{
  pipe->sendRequest(Request(0, user.id, state ? 1 : 0, settemp, setwdspd));
}

void Client::setTemp(int temp)
{
  Request request(0, user.id, 1, temp, room.setwdspd);
  pipe->sendRequest(request);
}

void Client::setWdspd(int wdspd)
{
  Request request(0, user.id, 1, room.settemp, wdspd);
  pipe->sendRequest(request);
}

void Client::setState(int state)
{
  Request request(0, user.id, state, room.settemp, room.setwdspd);
  pipe->sendRequest(request);
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
    fetchData();
    pipe->updateRoomTemp(room.roomId, room.temp+1);
}

int Client::getRecoverTime()
{
    int setTime = 60;
    QString str = emit sgn_getRecoverStr();
    if (str.length() == 0)
        return setTime;
    const char *s = str.toLatin1().data();
    while (*s && *s>='0' && *s<='9') s++;
    if (*s)
    {
        return setTime;
    }
    else
    {
        return str.toInt();
    }
}

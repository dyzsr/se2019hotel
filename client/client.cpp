#include "client.h"

#include <QTimer>
#include <QDebug>

Client::Client(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance()),
  timer(this),
  recoverTimer(this)
{
  Q_ASSERT(pipe != nullptr);
  connect(&recoverTimer, &QTimer::timeout, this, &Client::forceRoomChanged);
  recoverFlag = false;
}

Client::~Client()
{
}

void Client::fetchData()
{
  qDebug() << "[Client]";
  room = pipe->getRoom(user.id);
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
    pipe->sendRequest(Request(0, user.id, 2, 25, 1));

    connect(&timer, &QTimer::timeout, this, &Client::fetchDataAndCheck);
    timer.start(2000);
    qDebug() << "[Client] sign in";
    return true;
  }
  qDebug() << "[Client] input: " << usrId << " " << passwd;
  qDebug() << "[Client] database: " << user.id << " " << user.pswd;
  return false;
}

bool Client::signOut()
{
  timer.stop();
  qDebug() << "[Client] sign out";
  return true;
}

void Client::getIn(int state, double settemp, int setwdspd)
{
  pipe->sendRequest(Request(0, user.id, state, settemp, setwdspd));
}

void Client::setTemp(int temp)
{
  Request request(0, user.id, room.state, temp, room.setwdspd);
  qDebug() << "[Client]";
  pipe->sendRequest(request);
}

void Client::setWdspd(int wdspd)
{
  Request request(0, user.id, room.state, room.settemp, wdspd);
  qDebug() << "[Client]";
  pipe->sendRequest(request);
}

void Client::setState(int state)
{
  Request request(0, user.id, state, room.settemp, room.setwdspd);
  qDebug() << "[Client]";
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

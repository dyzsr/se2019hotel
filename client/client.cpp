#include "client.h"

#include <QTimer>
#include <QDebug>

/* 回温的坑：
 * 回温判断条件：目前是state==1
 * 回温模拟温度变化的方式：1分钟升1度
 * 回温检查当前室温：与settemp温差超过tempDiff=1，则重新发request_setTemp，不知道主机是否需要从机这样
 * 2秒刷新（检查state）一次，可能2秒发一次request，会不会过于频繁
 * 不测试不知道有没有语法bug
 *
 * 回温的逻辑：
 * 每2秒调用一次recoverTemp(),
 * recoverTemp():
 *     if 需要回温
 *         定时升温forceRoomChanged()
 *         检查温差，重发请求
 *     else
 *         取消定时升温
 * forceRoomChanged():
 *     在pipe新写了一个函数，修改roomId房间的temp
 */


Client::Client(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance()),
  timer(this),
  recoverTimer(this)
{
  Q_ASSERT(pipe != nullptr);
  connect(&recoverTimer, &QTimer::timeout, this, &Client::forceRoomChanged);
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
    double tempDiff = 1;
    bool recoverCondition = (room.state == 1);
    if (recoverCondition)
    {
        recoverTimer.start(60*1000);
        //重新发送请求
        if (room.temp > room.settemp + tempDiff || room.temp < room.settemp - tempDiff)
        {
            Request request(0, user.id, room.state, room.settemp, room.wdspd);
            qDebug() << "[Client]";
            pipe->sendRequest(request);
        }
    }
    else
    {
        recoverTimer.stop();
    }
}

void Client::forceRoomChanged()
{
    fetchData();
    pipe->updateRoomTemp(room.roomId, room.temp+1);
}

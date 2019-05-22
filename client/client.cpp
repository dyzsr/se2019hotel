#include "client.h"

#include <QTimer>
#include <QDebug>

Client::Client(QObject *parent, Pipe *_pipe):
  QObject(parent),
  pipe(_pipe),
  timer(this)
{
  Q_ASSERT(pipe != nullptr);
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

bool Client::signIn(QString usrId, QString passwd)
{
  user = pipe->getUser(usrId);
  if (user.pswd == passwd) {
    fetchData();
    emit sgn_setRoom(room);
    connect(&timer, &QTimer::timeout, this, &Client::fetchData);
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

void Client::setTemp(int temp)
{
  Request request(0, user.id, room.state, temp, room.wdspd);
  qDebug() << "[Client]";
  pipe->sendRequest(request);
}

void Client::setWdspd(int wdspd)
{
  Request request(0, user.id, room.state, room.temp, wdspd);
  qDebug() << "[Client]";
  pipe->sendRequest(request);
}

void Client::setState(int state)
{
  Request request(0, user.id, state, room.temp, room.wdspd);
  qDebug() << "[Client]";
  pipe->sendRequest(request);
}

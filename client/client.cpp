#include "client.h"

#include <QTimer>
#include <QDebug>

Client::Client(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance()),
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

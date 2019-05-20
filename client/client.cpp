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
  room = pipe->getRoom(user.id);
  emit sgn_refresh(room);
}

bool Client::signIn(QString usrId, QString passwd)
{
  user = pipe->getUser(usrId);
  // TODO: remove following 2 lines
  // user.id = "310f";
  // user.pswd = "password";
  if (user.pswd == passwd) {
    connect(&timer, &QTimer::timeout, this, &Client::fetchData);
    timer.start(2000);
    return true;
  }
  qDebug() << usrId << " " << passwd;
  qDebug() << user.id << " " << user.pswd;
  return false;
}

void Client::signOut()
{
  timer.stop();
}

void Client::incTemp()
{

}

void Client::decTemp()
{

}

void Client::incWdspd()
{

}

void Client::decWdspd()
{

}

void Client::setState(int state)
{

}

void Client::setMode(int mode)
{

}

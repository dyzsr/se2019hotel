#include "client.h"

#include <QTimer>

Client::Client(QObject *parent):
  QObject(parent),
  user("310f", "qweasdzxc")
{}

Client::~Client()
{
  if (status)
    stop();
}

void Client::setPipe(Pipe *_pipe)
{
  pipe = _pipe;
}

void Client::start()
{
  pipe->addUser(user);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &Client::getData);
  timer->start(2000);
  status = true;
}

void Client::stop()
{
  pipe->delUser(user);
}

void Client::getData()
{
  room = pipe->getRoom(room.usrId);
  emit sgn_refresh(room);
}

#include "client.h"

#include <QTimer>

Client::Client(QObject *parent):
  QObject(parent)
{
  user = User("310f", "qweasdzxc");
  pipe.addUser(user);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &Client::getData);
  timer->start(2000);
}

Client::~Client()
{
  pipe.delUser(user);
}

void Client::getData()
{
  room = pipe.getRoom(room.usrId);
  emit sgn_refresh(room);
}

#include "client.h"

Client::Client(QObject *parent):
  QObject(parent)
{
  user.id = QString("310f");
  user.pswd = QString("qweasdzxc");
  pipe.addUser(user);
}

Client::~Client()
{
  pipe.delUser(user);
}

void Client::getData()
{
}

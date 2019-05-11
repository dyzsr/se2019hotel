#include "client.h"

Client::Client(QObject *parent):
  QObject(parent)
{
  user = User("310f", "qweasdzxc");
  pipe.addUser(user);
}

Client::~Client()
{
  pipe.delUser(user);
}

void Client::getData()
{
}

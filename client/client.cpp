#include "client.h"

Client::Client(QObject *parent):
  QObject(parent),
  pipe(this)
{
  pipe.start();
}

Client::~Client()
{
  pipe.stop();
}

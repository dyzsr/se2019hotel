#include "server.h"

#include <QTimer>
#include <QDebug>

Server::Server(QObject *parent):
  QObject(parent)
{
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &Server::getData);
  timer->start(1000);
}

Server::~Server() {}

void Server::getData()
{

}

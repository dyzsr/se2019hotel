#include "server.h"

#include <QTimer>
#include <QDebug>

Server::Server(QObject *parent, Pipe *_pipe):
  QObject(parent),
  pipe(_pipe)
{
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &Server::fetchRequest);
  timer->start(1000);
}

Server::~Server() {}

void Server::fetchRequest()
{
  qDebug() << "[Server] ";
  QVector<Request> _request = pipe->getRequests();
  requests.append(_request);
  qDebug() << "[Server] ";
  pipe->delRequests(_request);
}

void Server::handleRequest()
{

}

QVector<Billing> Server::getBillings(QDateTime start, QDateTime end)
{
  // TODO
  return billings;
}

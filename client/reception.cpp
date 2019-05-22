#include "reception.h"

#include <QDebug>

Reception::Reception(QObject *parent, Pipe *_pipe):
  QObject(parent),
  pipe(_pipe)
{
  Q_ASSERT(pipe != nullptr);
}

void Reception::fetchBillings()
{
  billings = pipe->getBillings(room.usrId, room.roomId);
  qDebug() << "[Reception] fetch billing";
}

void Reception::setRoom(Room _room)
{
  room = _room;
  qDebug() << "[Reception] set room";
}

QVector<Billing> Reception::getBillings(QDateTime start, QDateTime end)
{
  int front = 0, rear = billings.size();
  if (start.isValid()) {
    for (const Billing &billing: billings) {
      if (start < billing.start)
        break;
      front++;
    }
  }
  if (end.isValid()) {
    for (auto it_billing = billings.rbegin();
         it_billing != billings.rend();
         ++it_billing) {
      if (it_billing->start < end)
        break;
      rear--;
    }
  }
  qDebug() << "[Reception] get billing";
  return billings.mid(front, rear - front);
}

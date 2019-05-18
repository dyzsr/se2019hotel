#include "reception.h"

Reception::Reception(QObject *parent, Pipe *_pipe):
  QObject(parent),
  pipe(_pipe)
{
  Q_ASSERT(pipe != nullptr);
}

void Reception::setRoom(const Room &_room)
{
  room = _room;
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
  return billings.mid(front, rear - front);
}

void Reception::fetchBillings()
{
  pipe->getBillings(room.usrId, room.roomId);
}

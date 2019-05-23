#include "frontdesk.h"

#include <QDebug>

FrontDesk::FrontDesk(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance())
{
  Q_ASSERT(pipe != nullptr);
}

void FrontDesk::checkOut(int roomId)
{
  qDebug() << "[FrontDesk] check out" << roomId;
  emit sgn_checkout(roomId);
}

QVector<Billing> FrontDesk::getBillings(int roomId)
{
  qDebug() << "[FrontDesk] get billings" << roomId;
  return pipe->getBillings(roomId);
}

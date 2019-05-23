#ifndef FRONTDESK_H
#define FRONTDESK_H

#include "../share/objects.h"
#include "../share/pipe.h"

#include <QObject>
#include <QVector>

class FrontDesk : public QObject
{
  Q_OBJECT
public:
  explicit FrontDesk(QObject *parent = nullptr);

signals:

public slots:
  // 用户离开 在room表里将roomId对应的user id修改为空字符串
  void checkOut(int roomId);
  // 获取roomId对应房间的所有billing
  QVector<Billing> getBillings(int roomId);

private:
  Pipe *pipe;

  QVector<Room> rooms;
  QVector<Billing> billings;
};

#endif // FRONTDESK_H

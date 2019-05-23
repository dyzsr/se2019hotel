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

private:
  Pipe *pipe;

  QVector<Room> rooms;
  QVector<Billing> billings;
};

#endif // FRONTDESK_H

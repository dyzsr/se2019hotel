#ifndef RECEPTION_H
#define RECEPTION_H

#include "../share/objects.h"
#include "../share/pipe.h"

#include <QObject>
#include <QVector>

class Reception : public QObject
{
  Q_OBJECT
public:
  explicit Reception(QObject *parent = nullptr, Pipe *_pipe = nullptr);

signals:

public slots:
  void setRoom(Room room);

  QVector<Billing> getBillings(QDateTime start = QDateTime(),
                               QDateTime end = QDateTime());

private:
  Pipe *pipe;
  Room room;
  QVector<Billing> billings;

  void fetchBillings();
};

#endif // RECEPTION_H

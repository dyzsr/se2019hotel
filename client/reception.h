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

  void setRoom(const Room &room);

  QVector<Billing> getBillings(QDateTime start = QDateTime(),
                               QDateTime end = QDateTime());

signals:
  void sgn_showBilling(QVector<Billing> billings);
  void sgn_showDetails(QVector<Billing> billings);

public slots:
  void fetchBillings();

private:
  Pipe *pipe;
  Room room;
  QVector<Billing> billings;
};

#endif // RECEPTION_H

#ifndef MANAGER_H
#define MANAGER_H

#include "../share/objects.h"
#include "../share/pipe.h"

#include <QObject>
#include <QVector>

class Manager : public QObject
{
  Q_OBJECT
public:
  explicit Manager(QObject *parent = nullptr, Pipe *pipe = nullptr);

signals:
  QVector<Billing> sgn_getBilling(QDateTime, QDateTime);

public slots:
  // TODO
  QVector<Billing> getDayReport();
  // TODO
  QVector<Billing> getWeekReport();
  // TODO
  QVector<Billing> getMonthReport();
  QVector<Billing> getReport(QDateTime start, QDateTime end);

private:
  Pipe *pipe;
  QVector<Billing> billings;

  void fetchBillings(QDateTime start, QDateTime end);
};

#endif // MANAGER_H

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
  explicit Manager(QObject *parent = nullptr);

signals:

public slots:
  void fetchBillings();

  QVector<Billing> getDayReport();
  QVector<Billing> getWeekReport();
  QVector<Billing> getMonthReport();
  QVector<Billing> getReport(QDateTime start, QDateTime end);

private:
  QVector<Billing> billings;
};

#endif // MANAGER_H

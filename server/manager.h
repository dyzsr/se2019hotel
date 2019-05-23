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
  ~Manager();

signals:
  QVector<Billing> sgn_getBilling(QDateTime, QDateTime);

public slots:
  bool signIn(QString name, QString pswd);
  bool signOut();

  QVector<Billing> getDayReport();
  QVector<Billing> getWeekReport();
  QVector<Billing> getMonthReport();
  QVector<Billing> getReport(QDateTime start, QDateTime end);

private:
  Pipe *pipe;

  Admin adm;
  QVector<Billing> billings;

  void fetchBillings(QDateTime start, QDateTime end);
};

#endif // MANAGER_H

#include "manager.h"

#include <QDateTime>

Manager::Manager(QObject *parent, Pipe *_pipe):
  QObject(parent),
  pipe(_pipe)
{
  Q_ASSERT(pipe != nullptr);
}

void Manager::fetchBillings(QDateTime start, QDateTime end)
{
  billings = emit sgn_getBilling(start, end);
}

QVector<Billing> Manager::getDayReport()
{
  // TODO: 用fetchData获取billing 指定起止时间
  QDateTime end；
  QDateTime start;
  end = QDateTime::currentDateTime();
  Qstring sstart = QDateTime::currentDateTime().addDays(-1).toString("yyyy-MM-dd hh:mm:ss");
  start = QDateTime::fromTime_t(sstart);
  fetchBillings(start, end);
  return billings;
}

QVector<Billing> Manager::getWeekReport()
{
  // TODO: 用fetchData获取billing 指定起止时间
  QDateTime end；
  QDateTime start;
  end = QDateTime::currentDateTime();
  Qstring sstart = QDateTime::currentDateTime().addDays(-7).toString("yyyy-MM-dd hh:mm:ss");
  start = QDateTime::fromTime_t(sstart);
  fetchBillings(start, end);
  return billings;
}

QVector<Billing> Manager::getMonthReport()
{
  // TODO: 用fetchData获取billing 指定起止时间
  QDateTime end；
  QDateTime start;
  end = QDateTime::currentDateTime();
  Qstring sstart = QDateTime::currentDateTime().addMonths(-1).toString("yyyy-MM-dd hh:mm:ss");
  start = QDateTime::fromTime_t(sstart);
  fetchBillings(start, end);
  return billings;
}

QVector<Billing> Manager::getReport(QDateTime start, QDateTime end)
{
  // 用fetchData获取billing 指定起止时间
  fetchBillings(start, end);
  return billings;
}

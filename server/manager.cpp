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
  return QVector<Billing>();
}

QVector<Billing> Manager::getWeekReport()
{
  // TODO: 用fetchData获取billing 指定起止时间
  return QVector<Billing>();
}

QVector<Billing> Manager::getMonthReport()
{
  // TODO: 用fetchData获取billing 指定起止时间
  return QVector<Billing>();
}

QVector<Billing> Manager::getReport(QDateTime start, QDateTime end)
{
  // 用fetchData获取billing 指定起止时间
  fetchBillings(start, end);
  return billings;
}

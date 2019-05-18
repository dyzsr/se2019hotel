#include "manager.h"

#include <QDateTime>

Manager::Manager(QObject *parent) : QObject(parent)
{

}

void Manager::fetchBillings()
{

}

QVector<Billing> Manager::getDayReport()
{
  // TODO
  return QVector<Billing>();
}

QVector<Billing> Manager::getWeekReport()
{
  // TODO
  return QVector<Billing>();
}

QVector<Billing> Manager::getMonthReport()
{
  // TODO
  return QVector<Billing>();
}

QVector<Billing> Manager::getReport(QDateTime start, QDateTime end)
{
  // TODO
  return QVector<Billing>();
}

#include "manager.h"

#include <QDateTime>
#include <QDebug>

Manager::Manager(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance())
{
  Q_ASSERT(pipe != nullptr);
  qDebug() << "[Manager] new";
}

Manager::~Manager()
{
  qDebug() << "[Manager] destroy";
}

bool Manager::signIn(QString name, QString pswd)
{
  adm = pipe->getAdmin(name);
  if (pswd == adm.manpwsd) {
    return true;
  } else {
    qDebug() << "<Input>" << name << pswd;
    qDebug() << "<Database>" << adm.manId << adm.manpwsd;
    return false;
  }
}

bool Manager::signOut()
{
  return true;
}

void Manager::fetchBillings(QDateTime start, QDateTime end)
{
  billings = emit sgn_getBilling(start, end);
}

QVector<Billing> Manager::getDayReport()
{
  // TODO: 用fetchData获取billing 指定起止时间
  QDateTime end;
  QDateTime start;
  end = QDateTime::currentDateTime();
  QString sstart = QDateTime::currentDateTime().addDays(-1).toString("yyyy-MM-dd hh:mm:ss");
  start = QDateTime::fromString(sstart);
  fetchBillings(start, end);
  return billings;
}

QVector<Billing> Manager::getWeekReport()
{
  // TODO: 用fetchData获取billing 指定起止时间
  QDateTime end;
  QDateTime start;
  end = QDateTime::currentDateTime();
  QString sstart = QDateTime::currentDateTime().addDays(-7).toString("yyyy-MM-dd hh:mm:ss");
  start = QDateTime::fromString(sstart);
  fetchBillings(start, end);
  return billings;
}

QVector<Billing> Manager::getMonthReport()
{
  // TODO: 用fetchData获取billing 指定起止时间
  QDateTime end;
  QDateTime start;
  end = QDateTime::currentDateTime();
  QString sstart = QDateTime::currentDateTime().addMonths(-1).toString("yyyy-MM-dd hh:mm:ss");
  start = QDateTime::fromString(sstart);
  fetchBillings(start, end);
  return billings;
}

QVector<Billing> Manager::getReport(QDateTime start, QDateTime end)
{
  // 用fetchData获取billing 指定起止时间
  fetchBillings(start, end);
  return billings;
}

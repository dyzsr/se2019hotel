#include "pipeserver.h"

#include <QDebug>
#include <QTimer>

PipeServer::PipeServer(QObject *parent):
  QObject(parent)
{
  db = QSqlDatabase::database("QPDBC");
}

void PipeServer::start()
{
  db.open();
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(getData()));
  timer->start(2000);
}

void PipeServer::getData()
{
  qDebug() << "getData";
}

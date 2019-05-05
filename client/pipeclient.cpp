#include "pipeclient.h"

#include <QDebug>
#include <QTimer>

PipeClient::PipeClient(QObject *parent):
  QObject(parent)
{
  db = QSqlDatabase::database("QPDBC");
}

void PipeClient::start()
{
  db.open();
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(getData()));
  timer->start(2000);
}

void PipeClient::getData()
{
  qDebug() << "getData";
}

#ifndef PIPESERVER_H
#define PIPESERVER_H

#include <QObject>
#include <QSqlDatabase>

class PipeServer: public QObject
{
  Q_OBJECT

public:
  explicit PipeServer(QObject *parent = nullptr);

  void start();

private slots:
  void getData();

private:
  QSqlDatabase db;
};

#endif // PIPESERVER_H

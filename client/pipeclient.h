#ifndef PIPECLIENT_H
#define PIPECLIENT_H

#include <QObject>
#include <QSqlDatabase>

class PipeClient: public QObject
{
  Q_OBJECT

public:
  explicit PipeClient(QObject *parent = nullptr);

  void start();

private slots:
  void getData();

private:
  QSqlDatabase db;
};

#endif // PIPECLIENT_H

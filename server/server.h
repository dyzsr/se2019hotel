#ifndef SERVER_H
#define SERVER_H

#include "../share/objects.h"
#include "../share/pipe.h"

#include <QObject>
#include <QVector>

class Server : public QObject
{
  Q_OBJECT
public:
  explicit Server(QObject *parent = nullptr, Pipe *_pipe = nullptr);
  ~Server();

signals:

private slots:
  void fetchRequest();
  void handleRequest();
  QVector<Billing> getBillings(QDateTime start = QDateTime(), QDateTime end = QDateTime());

private:
  Pipe *pipe;
  QVector<User> users;
  QVector<Room> rooms;

  QVector<Request> requests;
  QVector<Billing> billings;
};

#endif // SERVER_H

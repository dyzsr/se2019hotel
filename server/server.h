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
  explicit Server(QObject *parent = nullptr);
  ~Server();

  void setPipe(Pipe *_pipe);

signals:

private slots:
  void getData();

private:
  Pipe *pipe;
  QVector<User> users;
  QVector<Room> rooms;
};

#endif // SERVER_H

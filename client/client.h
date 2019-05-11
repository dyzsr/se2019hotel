#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "../share/objects.h"
#include "../share/pipe.h"

class Client: public QObject
{
  Q_OBJECT

public:
  explicit Client(QObject *parent = nullptr);
  ~Client();

private slots:
  void getData();

private:
  Pipe pipe;
  Room room;
  User user;
};

#endif // CLIENT_H

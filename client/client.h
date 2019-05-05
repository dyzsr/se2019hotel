#ifndef CLIENT_H
#define CLIENT_H

#include "pipeclient.h"


class Client: public QObject
{
  Q_OBJECT

public:
  explicit Client(QObject *parent = nullptr);
  ~Client();

private:
  PipeClient pipe;
};

#endif // CLIENT_H

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTimer>

#include "../share/objects.h"
#include "../share/pipe.h"

class Client: public QObject
{
  Q_OBJECT

public:
  explicit Client(QObject *parent = nullptr, Pipe *_pipe = nullptr);
  ~Client();

signals:
  void sgn_refresh(Room room);

public slots:
  bool signIn(QString usrId, QString passwd);
  void signOut();

  void incTemp();
  void decTemp();

  void incWdspd();
  void decWdspd();

  void setState(int state);
  void setMode(int mode);

private slots:
  void fetchData();

private:
  Pipe *pipe;
  QTimer timer;
  Room room;
  User user;
};

#endif // CLIENT_H

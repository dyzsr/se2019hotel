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
  explicit Client(QObject *parent = nullptr);
  ~Client();

signals:
  void sgn_refresh(Room room);
  void sgn_setRoom(Room room);

public slots:
  bool signIn(QString usrId, QString passwd);
  bool signOut();

  void setTemp(int temp);
  void setWdspd(int wdspd);
  void setState(int state);

private slots:
  void fetchData();

private:
  Pipe *pipe;
  QTimer timer;
  Room room;
  User user;
};

#endif // CLIENT_H

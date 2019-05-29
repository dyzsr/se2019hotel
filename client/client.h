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
  void sgn_init(double minTemp, double maxTemp);
  void sgn_refresh(Room room);
  void sgn_disable();
  QString sgn_getRecoverStr();

public slots:
  bool signIn(QString usrId, QString passwd);
  bool signOut();

  void getIn(int state, double settemp, int setwdspd);

  void setTemp(int temp);
  void setWdspd(int wdspd);
  void setState(int state);

private slots:
  void fetchDataAndCheck();
  void forceRoomChanged();

private:
  Pipe *pipe;

  Host info;

  QTimer timer, recoverTimer;
  Room room;
  User user;
  bool recoverFlag;

  void recoverTemp();
  void fetchData();
  int getRecoverTime();
};

#endif // CLIENT_H

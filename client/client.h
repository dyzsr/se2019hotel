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
  int sgn_getRecoverSpeed();

public slots:
  bool signIn(QString usrId, QString passwd);
  bool signOut();

  void getInfo(int &state, double &settemp, int &setwdspd);

  void sendRequest(int state, double settemp, int setwdspd);

private slots:
  void fetchDataAndCheck();
  void forceRoomChanged();

private:
  Pipe *pipe;

  Host info;

  int _wdspd;

  QTimer timer, recoverTimer;
  Room room;
  User user;
  bool recoverFlag;

  void recoverTemp();
  void fetchData();
  int getRecoverTime();
};

#endif // CLIENT_H

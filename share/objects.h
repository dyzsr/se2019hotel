#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <QDateTime>

struct User
{
  QString id;
  QString pswd;

  User(QString _id = "haha", QString _pswd = "hehe"): id(_id), pswd(_pswd) {}
};

struct Room
{
  int roomId;
  QString usrId;
  double temp;
  double settemp;
  double wdspd;
  double setwdspd;
  QString token;
  int state;
  int mode;
  QDateTime duration;
  double pwr;
  double cost;

  Room(int _roomId = 0, QString _usrId = "haha",
       double _temp = 25, double _settemp = 25,
       double _wdspd = 1, double _setwdspd = 1,
       QString _token = "", int _state = 0, int _mode = 0,
       QDateTime _duration = QDateTime(),
       double _pwr = 0, double _cost = 0):
    roomId(_roomId), usrId(_usrId), temp(_temp), settemp(_settemp),
    wdspd(_wdspd), setwdspd(_setwdspd), token(_token), state(_state),
    mode(_mode), duration(_duration), pwr(_pwr), cost(_cost)
  {}
};

struct Request
{
  int reqId;
  QString usrId;
  int state;
  double settemp;
  double setwdspd;

  Request(int _reqId = 0, QString _usrId = "haha",
          int _state = 0, int _settemp = 0, int _setwdspd = 0):
    reqId(_reqId), usrId(_usrId),
    state(_state), settemp(_settemp), setwdspd(_setwdspd)
  {}
};

struct Billing
{
  int billingId;
  int roomId;
  QString userId;
  QDateTime start;
  QDateTime duration;
  double costs;
  double wdspd;
  double startTemp;
  double endTemp;
  double rate;
};

#endif // OBJECTS_H

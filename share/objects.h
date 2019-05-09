#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <QDateTime>

struct User
{
  QString id;
  QString pswd;
};

struct Room
{
  int roomId;
  QString usrId;
  double temp;
  double s_temp;
  double wdspd;
  double s_wdspd;
  QString token;
  int state;
  int mode;
  QDateTime duration;
  double pwr;
  double cost;
};

struct Request
{
  int reqId;
  QString userId;
  int state;
  int s_temp;
  int s_wdspd;
};

struct Billing
{
  int billingId;
  int roomId;
  QString userId;
  QDateTime start;
  QDateTime duration;
  double costs;
  int wdspd;
  int startTemp;
  int endTemp;
  double rate;
};

#endif // OBJECTS_H

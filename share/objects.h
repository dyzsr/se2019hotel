#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <QDateTime>

struct User
{
  std::string id;
  std::string pswd;
};

struct Room
{
  int roomId;
  std::string usrId;
  int temp;
  int s_temp;
  int wdspd;
  int s_wdspd;
  std::string token;
  int state;
  int mode;
  QDateTime duration;
  double pwr;
  double cost;
};

struct Request
{
  int reqId;
  std::string userId;
  int state;
  int s_temp;
  int s_wdspd;
};

struct Billing
{
  int billingId;
  int roomId;
  std::string userId;
  QDateTime start;
  QDateTime duration;
  double costs;
  int wdspd;
  int startTemp;
  int endTemp;
  double rate;
};

#endif // OBJECTS_H

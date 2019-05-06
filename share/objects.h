#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

struct User
{
  int usrId;
  std::string name;
  std::string pswd;
};

struct Room
{
  int roomId;
  int usrId;
  bool usrIn;
  bool acOn;
  int temp;
  int s_temp;
  int wdspd;
  int s_wdspd;
  int mode;
  double pwr;
  double cost;
};

struct Request
{
  int reqId;
  int usrId;
  int _temp;
  int _wdspd;
  int _mode;
};

struct Billing
{
};

#endif // OBJECTS_H

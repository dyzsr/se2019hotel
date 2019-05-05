#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

class User
{
public:
  int usrId;
  std::string name;
  std::string pswd;
};

class Room
{
public:
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

class Request
{
public:
  int reqId;
  int usrId;
  int _temp;
  int _wdspd;
  int _mode;
};

#endif // OBJECTS_H

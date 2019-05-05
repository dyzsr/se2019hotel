#ifndef ROOM_H
#define ROOM_H


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

#endif // ROOM_H

#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <QDateTime>

struct User
{
  QString id;
  QString pswd;

  User(QString _id = "", QString _pswd = ""): id(_id), pswd(_pswd) {}
};

struct Room
{
  int roomId;
  QString usrId;
  double temp;
  double settemp;
  int wdspd;
  int setwdspd;
  QString token;
  int state;
  int mode;
  QDateTime duration;
  QDateTime start;
  double pwr;
  double cost;

  Room(int _roomId = 0, QString _usrId = "",
       double _temp = 25, double _settemp = 25,
       int _wdspd = 0, int _setwdspd = 0,
       QString _token = "", int _state = 0, int _mode = 0,
       QDateTime _duration = QDateTime(), QDateTime _start = QDateTime(),
       double _pwr = 0, double _cost = 0):
    roomId(_roomId), usrId(_usrId), temp(_temp), settemp(_settemp),
    wdspd(_wdspd), setwdspd(_setwdspd), token(_token), state(_state),
    mode(_mode), duration(_duration), start(_start),
    pwr(_pwr), cost(_cost)
  {}
};

struct Dispatchable : public Room {
  bool hasRequest;
  int requestType;
  QDateTime serviceStart;
  int64_t serviceTime;
  int64_t waitingTime;

  Dispatchable():
    hasRequest(false), requestType(0),
    serviceStart(QDateTime()), serviceTime(0), waitingTime(0)
  {}

  void update(const Room &room) {
    roomId = room.roomId;
    usrId = room.usrId;
    temp = room.temp;
    settemp = room.settemp;
    wdspd = room.wdspd;
    setwdspd = room.setwdspd;
    token = room.token;
    state = room.state;
    mode = room.mode;
    duration = room.duration;
    start = room.start;
    pwr = room.pwr;
    cost = room.cost;
  }

  bool operator< (const Dispatchable &o) {
    if (o.usrId.isEmpty()) return true;
    if (usrId.isEmpty()) return false;
    if (!o.hasRequest) return true;
    if (!hasRequest) return false;
    if (wdspd > o.wdspd) return true;
    if (wdspd < o.wdspd) return false;
    if (serviceTime < o.serviceTime) return true;
    if (serviceTime > o.serviceTime) return false;
    if (waitingTime < o.waitingTime) return true;
    if (waitingTime > o.waitingTime) return false;
    return serviceStart < o.serviceStart;
  }

};

struct Request
{
  int reqId;
  QString usrId;
  int state;
  double settemp;
  int setwdspd;

  Request(int _reqId = 0, QString _usrId = "",
          int _state = 0, double _settemp = 0, int _setwdspd = 0):
    reqId(_reqId), usrId(_usrId),
    state(_state), settemp(_settemp), setwdspd(_setwdspd)
  {}
};

struct Billing
{
  int billingId;
  int roomId;
  QDateTime start;
  QDateTime duration;
  double costs;
  int wdspd;
  double startTemp;
  double endTemp;
  double rate;
  int action;
  
  Billing(int _billingId = 0, int _roomId=0, QDateTime _start=QDateTime(),
          QDateTime _duration=QDateTime(), double _costs=0, int _wdspd=0, double _startTemp=0,
          double _endTemp=0, double _rate=0, int _action=0):
    billingId(_billingId), roomId(_roomId), start(_start), duration(_duration),
    costs(_costs), wdspd(_wdspd), startTemp(_startTemp), endTemp(_endTemp), rate(_rate), action(_action)
	{}
};

struct Admin {
	QString manId;
	QString manpwsd;
	int privilege;
	
  Admin (QString _manId="", QString _manpwsd="", int _privilege=0):
		manId(_manId), manpwsd(_manpwsd), privilege(_privilege)
	{} 
};

struct Host{
	int hostId;
	int startTemp;
	int mode;
	double paraLow;
	double paraMid;
	double paraHigh;
	double lowRate;
	double midRate;
	double highRate;
	int maxTemp;
	int minTemp;
	int defaultTemp;
	int defaultWdspd;
	int modeSche;
	int state;
	
  Host(int _hostId=0, int _startTemp=0, int _mode=0, double _paraLow=0, double _paraMid=0,
			double _paraHigh=0, double _lowRate=0, double _midRate=0, double _highRate=0, int _maxTemp=0,
			int _minTemp=0, int _defaultTemp=0, int _defaultWdspd=0, int _modeSche=0, int _state=0):
		hostId(_hostId), startTemp(_startTemp), mode(_mode), paraLow(_paraLow), paraMid(_paraMid),
		paraHigh(_paraHigh), lowRate(_lowRate), midRate(_midRate), highRate(_highRate), maxTemp(_maxTemp),
		minTemp(_minTemp), defaultTemp(_defaultTemp), defaultWdspd(_defaultWdspd), modeSche(_modeSche),
		state(_state)
		{}	
};

#endif // OBJECTS_H

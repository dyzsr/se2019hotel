#ifndef SERVER_H
#define SERVER_H

#include "../share/objects.h"
#include "../share/pipe.h"

#include <QMap>
#include <QObject>
#include <QVector>
#include <QReadWriteLock>

class Server : public QObject
{
  Q_OBJECT
public:
  explicit Server(QObject *parent = nullptr);
  ~Server();

  void init();

signals:
  void sgn_init(int nr_rooms);

public slots:
  void fetchRequests();
  void process();
  void checkOut(int roomId);

  Room getRoom(int roomId);
  QString getUsrId(int roomId);

private:
  void checkIn(QString usrId);

  void updateRooms();
  void updateService();

  bool serviceCompleted(int roomId);

  bool tempInRange(double temp1, double temp2, double range);

  double getRate(int wdspd);

  double getPara(int wdspd);

private:
  Pipe *pipe;

  // 主机信息
  Host info;

  // 用户
  QVector<User> users;

  int waiting_time = 120;
  int max_nr_service = 3;
  QVector<int> services;

  // 房间
  QReadWriteLock room_lock;
  QVector<Room> rooms;

  // 调度对象
  QVector<Dispatchable> dsps;

  // 用户名到房间的映射
  QMap<QString, int> user2room;

  // 账单计数
  int billings_cnt;
  // 表示是否需要增加一条billing记录
  QVector<Billing> billings;

};

#endif // SERVER_H

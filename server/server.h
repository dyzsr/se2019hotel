#ifndef SERVER_H
#define SERVER_H

#include "../share/objects.h"
#include "../share/pipe.h"

#include <QMap>
#include <QObject>
#include <QVector>
#include <QReadWriteLock>
#include <QQueue>

class Server : public QObject
{
  Q_OBJECT
public:
  explicit Server(QObject *parent = nullptr);
  ~Server();

signals:

public slots:
  void fetchRequests();
  void process();

  // TODO
  void checkOut(int roomId);

  Room getRoom(int roomId);

private:
  void init();

  int checkIn(QString usrId);

  void updateRooms();
  void uploadRooms();

  void updateBillings();
  void uploadBillings();

  void updateService();

  bool serviceCompleted(int roomId);

  double getRate(int wdspd);

  double getPara(int wdspd);

private:
  Pipe *pipe;

  // 主机信息
  Host info;

  // 用户
  QVector<User> users;

  const int MAX_SERVICE_NUM = 3;
  QVector<int> services;

  // 房间
  QReadWriteLock room_lock;
  QVector<Room> rooms;
  // 房间的请求（包含正在服务的和等待中的所有请求）
  QVector<Room> req_rooms;
  // 该房间是否有请求 request flag
  QVector<int> new_reqs;

  // 用户名到房间的映射
  QMap<QString, int> user2room;

  // 从机发来的请求
  QReadWriteLock req_lock;
  QList<Request> requests;

  // 账单计数
  int billings_cnt;
  // 表示是否需要增加一条billing记录
  QVector<Billing> billings;

};

#endif // SERVER_H

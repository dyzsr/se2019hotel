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
  void handleRequests();
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

  void requestRooms();

private:
  Pipe *pipe;

  Host info;

  QVector<User> users;

  const int dispatch_size = 3;
  QVector<int> dispatch;

  QVector<Room> rooms;
  QVector<Room> req_rooms;
  QVector<int> new_reqs;

  QReadWriteLock room_lock;

  QMap<QString, int> user2room;

  QLinkedList<Request> requests;
  QReadWriteLock req_lock;

  // 表示是否需要增加一条billing记录
  QVector<Billing> billings;

  int billings_cnt;
};

#endif // SERVER_H

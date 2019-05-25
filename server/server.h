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

signals:

public slots:
  void handleRequests();
  void process();

  // TODO
  void checkOut(int roomId);

  Room getRoom(int roomId);

private:
  void init();
  int allocateRoom(QString usrId);

  void updateRooms();
  void updateBillings();
  void addBillings();

  void uploadRooms();
  void uploadBillings();
  void uploadNewBillings();

private:
  Pipe *pipe;

  Host info;

  QVector<User> users;

  QVector<Room> rooms;
  QReadWriteLock room_lock;

  QMap<QString, int> user2room;

  QVector<Request> requests;
  QReadWriteLock req_lock;

  // 表示是否需要增加一条billing记录
  QVector<bool> newBilling;
  QVector<Billing> billings;
};

#endif // SERVER_H

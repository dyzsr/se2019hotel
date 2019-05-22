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
  explicit Server(QObject *parent = nullptr, Pipe *_pipe = nullptr);
  ~Server();

signals:

public slots:
  void process();
  QVector<Billing> getBillings(QDateTime start = QDateTime(), QDateTime end = QDateTime());

private:
  void init();
  int allocateRoom(QString usrId);
  void fetchRequests();
  void handleRequests();
  void updateRooms();
  void updateBillings();
  void uploadData();

private:
  Pipe *pipe;

  QVector<User> users;
  QVector<Room> rooms;
  QMap<QString, int> user2room;

  QReadWriteLock room_lock;
  QReadWriteLock req_lock;

  QVector<Request> requests;
  QVector<Billing> billings;
};

#endif // SERVER_H

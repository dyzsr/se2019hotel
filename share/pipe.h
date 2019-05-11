#ifndef PIPE_H
#define PIPE_H

#include <QSqlDatabase>
#include "objects.h"

class Pipe
{
public:
  Pipe();
  ~Pipe();

//#ifdef CLIENT_H

  void addUser(const User &user);
  void delUser(const User &user);

  Room getRoom(QString usrId);

  int getBillings(Billing *billings);
  Billing getBilling(int roomId);

  void sendRequest(const Request &request);

//#elif SERVER_H

  void setRooms(const Room *rooms, int n);
  void addRoom(const Room &room);
  void delRoom(const Room &room);

  int getUsers(User *users);
  void setUsers(const User *users, int n);

  int getRequests(Request *requests);
  void delRequests(const Request *requests, int n);

//#endif

private:
  QSqlDatabase db;
};

#endif // PIPE_H

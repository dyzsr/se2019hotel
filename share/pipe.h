#ifndef PIPE_H
#define PIPE_H

#include <QSqlDatabase>
#include "objects.h"

class Pipe
{
public:
  Pipe();
  ~Pipe();

#ifdef CLIENT_H

  int getRooms(Room *rooms);
  Room getRoom(int roomId);

  void addUser(const User &user);
  void addRoom(const Room &room);

  int getBillings(Billing *billings);
  Billing getBilling(int roomId);

  void sendRequest(const Request &request);

#elif SERVER_H

  void setRooms(const Room *rooms, int n);

  int getUsers(User *users);
  void setUsers(const User *users, int n);

  int getRequests(Request *requests);
  void delRequests(const Request *requests, int n);

#endif

private:
  QSqlDatabase db;
};

#endif // PIPE_H

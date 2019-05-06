#ifndef PIPE_H
#define PIPE_H

#include <QSqlDatabase>
#include "objects.h"

class Pipe
{
public:
  Pipe();
  ~Pipe();

  int getRooms(Room *rooms);
  int getUsers(User *users);
  int getRequests(Request *requests);

  void setRooms(const Room *rooms, int n);
  void setUsers(const User *users, int n);

  void sendRequest(Request request);

private:
  QSqlDatabase db;
};

#endif // PIPE_H

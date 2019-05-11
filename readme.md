Title: 说明


[TITLE]

# 平台说明

* OS: Windows 10
* Framework: Qt 5.12
* Compiler: MSVC 2017 64bit


# 项目目录

```
se2019hotel
├── client
│   ├── client.cpp
│   ├── client.h
│   ├── client.pro
│   ├── client.pro.user
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   └── mainwindow.ui
├── server
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   ├── server.pro
│   └── server.pro.user
├── share
│   ├── objects.h
│   ├── pipe.cpp
│   ├── pipe.h
│   └── tables.sql
├── project.html
└── readme.md
```

`se2019hotel`为项目根目录，添加在git中：`git@github.com:dyzsr/se2019hotel.git`。

`client`和`server`为两个子项目。

`share`中存放共享文件。

# share目录

## 数据对象



## 共享接口

pipe.h

```  CPP
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
  void delUser(const User &user);

  int getBillings(Billing *billings);
  Billing getBilling(int roomId);

  void sendRequest(const Request &request);

#elif SERVER_H

  void setRooms(const Room *rooms, int n);
  void addRoom(const Room &room);
  void delRoom(const Room &room);

  int getUsers(User *users);
  void setUsers(const User *users, int n);

  int getRequests(Request *requests);
  void delRequests(const Request *requests, int n);

#endif

private:
  QSqlDatabase db;
};

#endif // PIPE_H  
```


# Client子项目


# Server子项目

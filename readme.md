# 说明

## 平台说明

* 框架: Qt 5.12
* 数据库：Mysql 8.0

### 数据库配置

使用ODBC连接，配置细节见odbc.txt。

## 任务列表

- [ ] 根据`share/db.sql`中表的结构修改`share/objects.h`中类的定义。
- [ ] 完善`share/objects.h`定义的接口。
- [ ] 建立server子项目的界面。

## 项目目录

```
project
├── client
│   ├── client.cpp
│   ├── client.h
│   ├── client.pro
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   ├── reception.cpp
│   └── reception.h
├── server
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   ├── manager.cpp
│   ├── manager.h
│   ├── server.cpp
│   ├── server.h
│   ├── server.pro
├── share
│   ├── db.sql
│   ├── objects.h
│   ├── pipe.cpp
│   ├── pipe.h
│   └── tables.sql
└── readme.md
```

`se2019hotel`为项目根目录，添加在git中：`git@github.com:dyzsr/se2019hotel.git`。

`client`和`server`为两个子项目。

`share`中存放共享文件。

## share目录

### 数据库表格式

见`share/db.sql`。

### 数据对象

见`share/objects.h`。

### 共享接口

`share/pipe.h`

用于提供数据通信功能，对数据库的访问做了一层封装。使用时无需考虑数据库的通信问题。

```  CPP
#ifndef PIPE_H
#define PIPE_H

#include <QSqlDatabase>
#include "objects.h"

class Pipe
{
public:
  int getRooms(Room *rooms);
  Room getRoom(int roomId);

  void addUser(const User &user);
  void delUser(const User &user);

  int getBillings(Billing *billings);
  Billing getBilling(int roomId);

  void sendRequest(const Request &request);

  void setRooms(const Room *rooms, int n);
  void addRoom(const Room &room);
  void delRoom(const Room &room);

  int getUsers(User *users);
  void setUsers(const User *users, int n);

  int getRequests(Request *requests);
  void delRequests(const Request *requests, int n);  
};

#endif // PIPE_H  
```


## Client子项目

在Qt Creator中选择`client/client.pro`打开client项目。

### Client类

Client类代表客户。

待实现的成员方法：

- [x] `fetchData()`，通过pipe获得房间的数据
- [ ] `incTemp()`，通过pipe发送升温请求
- [ ] `decTemp()`，
- [ ] `incWdspd()`，
- [ ] `decWdspd()`，
- [ ] `setState(int state)`，
- [ ] `setMode(int mode)`，

### Reception类

Reception类代表前台。

待实现的成员方法：

- [ ] `fetchBillings()`，通过pipe获得房间的消费数据


## Server子项目

在Qt Creator中选择`server/server.pro`打开server项目。

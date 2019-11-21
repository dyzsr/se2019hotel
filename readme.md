# 说明

## 平台说明

* 框架: Qt 5.12
* 数据库：Mysql 8.0

### 数据库配置

使用ODBC连接，配置细节见odbc.txt。

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
│   └── mainwindow.ui
├── server
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   ├── managercontrol.cpp
│   ├── managercontrol.h
│   ├── manager.cpp
│   ├── manager.h
│   ├── managerwindow.cpp
│   ├── managerwindow.h
│   ├── managerwindow.ui
│   ├── recordscontrol.cpp
│   ├── recordscontrol.h
│   ├── records.cpp
│   ├── records.h
│   ├── recordswindow.cpp
│   ├── recordswindow.h
│   ├── recordswindow.ui
│   ├── server.cpp
│   ├── server.h
│   └── server.pro
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

见`share/pipe.h`

用于提供数据通信功能，对数据库的访问做了一层封装。使用时无需考虑数据库的通信问题。


## Client子项目

在Qt Creator中选择`client/client.pro`打开client项目。


## Server子项目

在Qt Creator中选择`server/server.pro`打开server项目。

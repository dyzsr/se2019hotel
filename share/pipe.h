#ifndef PIPE_H
#define PIPE_H

#include <QLinkedList>
#include <QSqlDatabase>
#include "objects.h"

class Pipe
{
private:
  Pipe();
  ~Pipe();

  static Pipe pipe;

public:

  static Pipe *getInstance();
  /*
   * Methods for Clients
   * */

  // 获取用户数据
  User getUser(QString usrId);

  // 获取房间数据
  Room getRoom(QString usrId);

  // 向数据库里写请求（添加在request表的末尾）
  void sendRequest(const Request &request);

  /*
   * Methods for Server end
   * */

  // 添加用户
  void addUser(const User &user);

  // 删除用户
  void delUser(const User &user);

  // 获取所有用户数据
  QVector<User> getUsers();

  // 统一设置所有用户数据
  void setUsers(const QVector<User> &users);

  // 统一设置所有房间数据（删除原有的房间数据）
  void addRooms(const QVector<Room> &rooms);

  bool updateRoom(const Room &room);

  void updateRooms(const QVector<Room> &rooms);

  bool updateRoomExceptTemp(const Room &room);

  void updateRoomTemp(int roomId, double temp);

  double getRoomTemp(int roomId);

  // 获取所有房间信息
  QVector<Room> getRooms();

  // 增添一个房间
  void addRoom(const Room &room);

  // 删除一个房间
  void delRoom(const Room &room);

  // 获取数据库中的所有请求
  QList<Request> getRequests();

  // 删除数据库中给定的请求
  void delRequests(const QList<Request> &requests);

  int getCurrentBillingId();

  // 获取账目列表（以roomId来筛选）
  QVector<Billing> getBillings(int roomId);

  // 获取全部bill
  QVector<Billing> getAllBillings();

  // 更新多条账目记录
  void updateBillings(const QVector<Billing> &billings);

  // 更新一条账目记录
  void updateBilling(const Billing &billing);

  // 添加一条账目记录
  void addBilling(const Billing &billing);

  // 获取管理员信息列表
  QVector<Admin> getAdmins();

  Admin getAdmin(QString name);

  // 获取主机配置信息
  Host getHost();

private:
  QSqlDatabase db;
};

#endif // PIPE_H

#ifndef MANAGER_H
#define MANAGER_H

#include "../share/objects.h"
#include "../share/pipe.h"

#include <QObject>
#include <QVector>

class Manager : public QObject
{
  Q_OBJECT
public:
  explicit Manager(QObject *parent = nullptr);
  ~Manager();
    QVector<QString> getRoomStateStr();
    QVector<Room> getAllRooms();
    Room getOneRoom(int roomId);
    void setTheRoom(Room room);
    Room getTheRoom();

signals:

public slots:
  bool signIn(QString name, QString pswd);
  bool signOut();

private:
  Pipe *pipe;
  Room theAskedRoom;

  Admin adm;
  QString stateToStr(int state);
};

#endif // MANAGER_H

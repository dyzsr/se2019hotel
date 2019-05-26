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
    Room getOneRoom(int roomId);

signals:

public slots:
  bool signIn(QString name, QString pswd);
  bool signOut();

private:
  Pipe *pipe;

  Admin adm;
  QVector<Room> getAllRooms();
  QString stateToStr(int state);
};

#endif // MANAGER_H

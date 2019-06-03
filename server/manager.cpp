#include "manager.h"

#include <QDateTime>
#include <QDebug>
#pragma execution_character_set("utf-8")

Manager::Manager(QObject *parent):
  QObject(parent),
  pipe(Pipe::getInstance())
{
  Q_ASSERT(pipe != nullptr);
  qDebug() << "[Manager] new";
}

Manager::~Manager()
{
  qDebug() << "[Manager] destroy";
}

bool Manager::signIn(QString name, QString pswd)
{
  adm = pipe->getAdmin(name);
  if (pswd == adm.manpwsd) {
    return true;
  } else {
    qDebug() << "<Input>" << name << pswd;
    qDebug() << "<Database>" << adm.manId << adm.manpwsd;
    return false;
  }
}

bool Manager::signOut()
{
  return true;
}

QVector<Room> Manager::getAllRooms()
{
    return pipe->getRooms();
}

Room Manager::getOneRoom(int roomId)
{
    QVector<Room> q = getAllRooms();
    for (int i=0; i<q.length(); ++i)
        if (q.at(i).roomId == roomId)
            return q.at(i);
    return Room();
}

QVector<QString> Manager::getRoomStateStr(const QVector<Room> rooms)
{
    QVector<QString> q;
    QString str;
    int i, j;
    for (i=0; i<rooms.length(); ++i)
    {
        //第一行
        str.clear();
        str.append("房间号：");
        str.append(QString::number(rooms.at(i).roomId));
        str.append("  住户：");
        str.append(rooms.at(i).usrId);
        q.append(str);
        //第二行
        str.clear();
        str.append("状态：");
        str.append(stateToStr(rooms.at(i).state));
        str.append("  当前温度：");
        str.append(QString::number(rooms.at(i).temp));
        str.append("  目标温度：");
        str.append(QString::number(rooms.at(i).settemp));
        q.append(str);
        //第三行
        str.clear();
        str.append("风速：");
        str.append(QString::number(rooms.at(i).wdspd));
        str.append("  费率：");
        str.append(QString::number(rooms.at(i).pwr));
        str.append("  时长：");
        //时长
        int64_t du;
        du = rooms.at(i).start.secsTo(rooms.at(i).duration);
        //QString Duration;
        //Duration = billings.at(i).duration.toString("yyyy-MM-dd hh:mm:ss");
        int64_t h = du / 3600;
        int h1 = du % 3600;
        str.append(QString::number(h));
        str.append("时");
        int m = h1 / 60;
        int m1 = h1 % 60;
        str.append(QString::number(m));
        str.append("分");
        str.append(QString::number(m1));
        str.append("秒");
        //else
        str.append("  费用：");
        str.append(QString::number(rooms.at(i).cost));
        q.append(str);
        //分隔行
        str.clear();
        for (j=0; j<63; ++j)
            str.append("-");
        q.append(str);
    }
    return q;
}

QString Manager::stateToStr(int state)
{
    switch (state) {
    case 0: return QString("关机");
    case 1: return QString("运行");
    case 2: return QString("待机");
    case 3: return QString("调度");
    default: return QString("不明");
    }
}

void Manager::setTheRoom(Room room)
{
    theAskedRoom = room;
}

Room Manager::getTheRoom()
{
    return theAskedRoom;
}

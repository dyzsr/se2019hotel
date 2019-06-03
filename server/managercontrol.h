#ifndef MANAGERCONTROL_H
#define MANAGERCONTROL_H

#include "manager.h"
#include "managerwindow.h"

#include <QObject>

class ManagerControl: public QObject
{
  Q_OBJECT

public:
  explicit ManagerControl(QObject *parent = nullptr);
    Manager manager;

signals:
    Room sgn_getRoom(int roomId);
    QVector<Room> sgn_getAllRooms();
    void sgn_showSimpleBill(Room room);
    void sgn_showDetailedBill(Room room);
    void sgn_showReportForm();

public slots:
  void openNewWindow();
  void askManagerFunc();
  bool slot_checkRoomIdValid(int roomId);
  void slot_askSimpleBill_clicked();
  void slot_askDetailedBill_clicked();
  void slot_askReportForm_clicked();

private:
  ManagerWindow window;
};

#endif // MANAGERCONTROL_H

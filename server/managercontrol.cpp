#include "managercontrol.h"

ManagerControl::ManagerControl(QObject *parent):
  QObject(parent),
  manager(parent),
  window(static_cast<QWidget *>(parent))
{
  connect(&window, &ManagerWindow::sgn_signIn, &manager, &Manager::signIn);
  connect(&window, &ManagerWindow::sgn_signOut, &manager, &Manager::signOut);
  connect(&window, &ManagerWindow::sgn_askManagerFunc,
          this, &ManagerControl::askManagerFunc);
  connect(&window, &ManagerWindow::sgn_askReportForm_clicked,
          this, &ManagerControl::slot_askReportForm_clicked);
  window.hide();
}

void ManagerControl::openNewWindow()
{
  window.init();
  window.show();
}

void ManagerControl::askManagerFunc()
{
    window.showManagerFunc(manager.getRoomStateStr());
}

bool ManagerControl::slot_checkRoomIdValid(int roomId)
{
  manager.setTheRoom(emit sgn_getRoom(roomId));
  return true;
  /*
    QVector<Room> rooms = manager.getAllRooms();
    for (int i=0; i<rooms.length(); ++i)
    {
        if (rooms.at(i).roomId == roomId)
        {
            manager.setTheRoom(rooms.at(i));
            return true;
        }
    }
    return false;
    */
}

void ManagerControl::slot_askSimpleBill_clicked()
{
    emit sgn_showSimpleBill(manager.getTheRoom());
}

void ManagerControl::slot_askDetailedBill_clicked()
{
    emit sgn_showDetailedBill(manager.getTheRoom());
}

void ManagerControl::slot_askReportForm_clicked()
{
    emit sgn_showReportForm();
}

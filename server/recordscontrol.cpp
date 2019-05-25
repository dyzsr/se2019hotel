#include "recordscontrol.h"

RecordsControl::RecordsControl(QObject *parent):
  QObject(parent),
  records(parent),
  recordsWindow(static_cast<QWidget *>(parent))
{
}

void RecordsControl::slot_simpleBill(int roomId)
{
    Room room;
    room.usrId = "name";
    //room.roomId = 0;
    simpleBillWindow.showSimpleBill(room, records.getSimpleBills(roomId));
}

void RecordsControl::slot_detailedBill(int roomId)
{
    Room room;
    room.usrId = "name";
    recordsWindow.showDetailedBill(room, records.getDetailedBill(roomId));


}

void RecordsControl::slot_reportForm()
{
    reportFormWindow.showReportForm(records.getReportForm());
}

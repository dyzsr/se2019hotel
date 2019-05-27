#include "recordscontrol.h"

RecordsControl::RecordsControl(QObject *parent):
  QObject(parent),
  records(parent),
  recordsWindow(static_cast<QWidget *>(parent))
{
}

void RecordsControl::slot_showSimpleBill(Room room)
{
    simpleBillWindow.showSimpleBill(room, records.getSimpleBills(room.roomId));
}

void RecordsControl::slot_showDetailedBill(Room room)
{
    recordsWindow.showDetailedBill(room, records.getDetailedBill(room.roomId));
}

void RecordsControl::slot_showReportForm()
{
    reportFormWindow.showReportForm(records.getReportForm());
}

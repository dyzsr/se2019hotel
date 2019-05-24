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
    simpleBillWindow.showSimpleBill(room);
}

void RecordsControl::slot_detailedBill(int roomId)
{
    Room room;
    recordsWindow.showDetailedBill(room, records.getDetailedBill(roomId));
}

void RecordsControl::slot_reportForm()
{
    reportFormWindow.showReportForm(records.getReportForm());
}

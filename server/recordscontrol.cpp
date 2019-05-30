#include "recordscontrol.h"

RecordsControl::RecordsControl(QObject *parent):
  QObject(parent),
  records(parent),
  recordsWindow(static_cast<QWidget *>(parent)),
  simpleBillWindow(static_cast<QWidget *>(parent)),
  reportFormWindow(static_cast<QWidget *>(parent))
{
  recordsWindow.hide();
  simpleBillWindow.hide();
  reportFormWindow.hide();
}

void RecordsControl::slot_showSimpleBill(Room room)
{
    simpleBillWindow.showSimpleBill(room, records.getSimpleBills(room.roomId));
}

void RecordsControl::slot_showDetailedBill(Room room)
{
    recordsWindow.showDetailedBill(room, records.getDetailedBill(room));
}

void RecordsControl::slot_showReportForm()
{
    QDateTime ini;
    ini = QDateTime::currentDateTime().addDays(-1);
    QDateTime ini2;
    ini2 = QDateTime::currentDateTime();
    records.getInfoOnce();
    reportFormWindow.showReportForm(records.getReportForm(ini,ini2));
}

#include "recordscontrol.h"

RecordsControl::RecordsControl(QObject *parent):
  QObject(parent),
  records(parent),
  recordsWindow(static_cast<QWidget *>(parent)),
  simpleBillWindow(static_cast<QWidget *>(parent)),
  reportFormWindow(static_cast<QWidget *>(parent), &records)
{
  recordsWindow.hide();
  simpleBillWindow.hide();
  reportFormWindow.hide();
}

void RecordsControl::slot_showSimpleBill(Room room)
{
    simpleBillWindow.showSimpleBill(room, records.getSimpleBills(room));
}

void RecordsControl::slot_showDetailedBill(Room room)
{
    recordsWindow.showDetailedBill(room, records.getDetailedBill(room));
}

void RecordsControl::slot_showReportForm()
{
  records.setNr_rooms(emit sgn_nr_rooms());
//  QDateTime ini;
//  ini = QDateTime::currentDateTime().addDays(-1);
//  QDateTime ini2;
//  ini2 = QDateTime::currentDateTime();
  reportFormWindow.on_OK_clicked();
  reportFormWindow.show();
  //    reportFormWindow.showReportForm(records.getReportForm(ini,ini2));
}

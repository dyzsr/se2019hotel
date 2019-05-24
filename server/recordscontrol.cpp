#include "recordscontrol.h"

RecordsControl::RecordsControl(QObject *parent) : QObject(parent)
{

}

void RecordsControl::slot_simpleBill(int roomId)
{
    recordsWindow.showSimpleBill(records.getSimpleBill(roomId));
}

void RecordsControl::slot_detailedBill(int roomId)
{
    Room room;
    recordsWindow.showDetailedBill(room, records.getDetailedBill(roomId));
}

void RecordsControl::slot_reportForm(QDateTime start, QDateTime end)
{
    recordsWindow.showReportForm(records.getReportForm(start, end));
}

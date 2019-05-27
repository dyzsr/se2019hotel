#ifndef RECORDSCONTROL_H
#define RECORDSCONTROL_H

#include "records.h"
#include "recordswindow.h"
#include "simplebillwindow.h"
#include "reportformwindow.h"

#include <QObject>

class RecordsControl : public QObject
{
    Q_OBJECT
public:
    explicit RecordsControl(QObject *parent = nullptr);

signals:

public slots:
    void slot_showSimpleBill(Room room);
    void slot_showDetailedBill(Room room);
    void slot_showReportForm();

private:
    Records records;
    RecordsWindow recordsWindow;
    SimpleBillWindow simpleBillWindow;
    ReportFormWindow reportFormWindow;
};

#endif // RECORDSCONTROL_H

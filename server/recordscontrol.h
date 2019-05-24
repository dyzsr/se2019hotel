#ifndef RECORDSCONTROL_H
#define RECORDSCONTROL_H

#include "records.h"
#include "recordswindow.h"

#include <QObject>

class RecordsControl : public QObject
{
    Q_OBJECT
public:
    explicit RecordsControl(QObject *parent = nullptr);

signals:

public slots:
    void slot_simpleBill(int roomId);
    void slot_detailedBill(int roomId);
    void slot_reportForm(QDateTime start, QDateTime end);

private:
    Records records;
    RecordsWindow recordsWindow;
};

#endif // RECORDSCONTROL_H

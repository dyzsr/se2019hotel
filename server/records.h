﻿#ifndef RECORDS_H
#define RECORDS_H

#include "../share/objects.h"
#include "../share/pipe.h"

#include <QObject>
#include <QVector>

class Records : public QObject
{
    Q_OBJECT
public:
    explicit Records(QObject *parent = nullptr);

    QVector<QString> getDetailedBill(Room room);
    QVector<QString> getSimpleBills(int roomId);
    QVector<QString> getReportForm(QDateTime start , QDateTime end);
    void getInfoOnce();

signals:

public slots:

private:
    Pipe *pipe;
    QString calcDurationStr(long long duration);
    QVector<Billing> billingss;
    QVector<Room> rooms;
};

#endif // RECORDS_H

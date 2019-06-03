#ifndef RECORDS_H
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
    QVector<QString> getSimpleBills(Room room);
    QVector<QString> getReportForm(QDateTime start , QDateTime end);
    void setNr_rooms(int value);

    QVector<int> op;
    QVector<int> temp;
    QVector<int> speed;
    QVector<int> record;
    QVector<int64_t> duratio;
    QVector<double> fee;

signals:

public slots:

private:
    Pipe *pipe;
    QString calcDurationStr(long long duration);
    int nr_rooms;
};

#endif // RECORDS_H

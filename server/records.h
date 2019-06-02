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
    QVector<QString> getSimpleBills(int roomId);
    QVector<QString> getReportForm(QDateTime start , QDateTime end);
    void getInfoOnce();
    int *op;//开关次数
    int *temp;//调温
    int *speed;//调风
    int *record;//详单
    int64_t *duratio;//时长
    double *fee;//总消费
signals:

public slots:

private:
    Pipe *pipe;
    QString calcDurationStr(long long duration);
    QVector<Billing> billingss;
    QVector<Room> rooms;
};

#endif // RECORDS_H

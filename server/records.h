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

    QVector<QString> getSimpleBill(int roomId);
    QVector<QString> getDetailedBill(int roomId);
    QVector<QString> getReportForm(QDateTime start, QDateTime end);

signals:

public slots:

private:
    Pipe *pipe;
    void getBillings(int roomId, QDateTime start, QDateTime end);
};

#endif // RECORDS_H

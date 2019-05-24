#include "records.h"

Records::Records(QObject *parent) :
    QObject(parent),
    pipe(Pipe::getInstance())
{
    Q_ASSERT(pipe != nullptr);
}

QVector<QString> Records::getSimpleBill(int roomId)
{
}

QVector<QString> Records::getDetailedBill(int roomId)
{
    QVector<Billing> billings = pipe->getBillings(roomId);
    QVector<QString> data;
    QString str;
    for (int i=0; i<billings.length(); ++i)
    {
        str.clear();
        //序号
        str.append(QString::number(i+1));
        str.append(". ");
        if (str.length() == 3)
            str.append(" ");
        //持续时间
        str.append(billings.at(i).duration.toString());
        str.append(" ");
        //风速
        str.append(QString::number(billings.at(i).wdspd));
        str.append(" ");
        //费率
        str.append(QString::number(billings.at(i).rate));
        str.append(" ");
        //费用
        str.append(QString::number(billings.at(i).costs));

        data.append(str);
    }
    return data;
}

QVector<QString> Records::getReportForm(QDateTime start, QDateTime end)
{

}

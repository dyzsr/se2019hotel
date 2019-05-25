#include "records.h"
#pragma execution_character_set("utf-8");

Records::Records(QObject *parent) :
    QObject(parent),
    pipe(Pipe::getInstance())
{
    Q_ASSERT(pipe != nullptr);
}

QVector<QString> Records::getDetailedBill(int roomId)
{
    QVector<Billing> billings = pipe->getBillings(roomId);
/*
    Billing b;
    b.duration = QDateTime();
    billings.append(b);
*/
    QVector<QString> data;
    QString str;
    for (int i=0; i<billings.length(); ++i)
    {
        str.clear();
        //序号
        str.append(QString::number(i+1));
        str.append(".  ");
        if (str.length() == 4)
            str.append("  ");
        //风速
        str.append("风速：");
        str.append(QString::number(billings.at(i).wdspd));
        //费率
        str.append("  费率：");
        str.append(QString::number(billings.at(i).rate));
        //持续时间
        str.append("  时长：");
        str.append(calcDurationStr(billings.at(i).duration));
        //费用
        str.append("  费用：");
        str.append(QString::number(billings.at(i).costs));
        str.append("元");

        data.append(str);
    }
    return data;
}

QVector<Billing> Records::getReportForm()
{
  // TODO
  return pipe->getAllBillings();
}

QString Records::calcDurationStr(long long duration)
{
    QString str;
    if (duration < 60)
    {
        str.append("少于1m");
    }
    else
    {
        long long d = duration;
        int h, m;
        h = d / 3600;
        d = d % 3600;
        m = d / 60;
        if (h != 0)
        {
            str.append(QString::number(h));
            str.append("时");
        }
        str.append(QString::number(m));
        str.append("分");
    }
    return str;
}

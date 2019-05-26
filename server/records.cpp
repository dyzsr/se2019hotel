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
        int du;
        du = billings.at(i).start.secsTo(billings.at(i).duration);
        //QString Duration;
        //Duration = billings.at(i).duration.toString("yyyy-MM-dd hh:mm:ss");
        int h = du / 3600;
        int h1 = du % 3600;
        str.append(QString::number(h));
        str.append("时");
        int m = h1 / 60;
        int m1 = h1 % 60;
        str.append(QString::number(m));
        str.append("分");
        str.append(QString::number(m1));
        str.append("秒");
        //str.append(calcDurationStr(billings.at(i).duration));
        //费用
        str.append("  费用：");
        str.append(QString::number(billings.at(i).costs));
        str.append("元");

        data.append(str);
    }
    return data;
}

QVector<QString> Records::getSimpleBills(int roomId)
{
  // TODO
    QVector<Billing> billings = pipe->getBillings(roomId);
    QVector<QString> data;
    QString str;
    QDateTime max = QDateTime::fromString("00-00-00 00:00:00");
    int maxi = 0;
    uint stime;
    uint etime;
    int tRet;
    for (int i=0; i<billings.length(); ++i)
    {
        stime = max.toTime_t();
        etime = billings.at(i).start.toTime_t();
        tRet = stime - etime;
        if(tRet < 0)
        {
            max = billings.at(i).start;
            maxi = i;
        }
    }
    /*str.append(QString::number(1));
    str.append(". ");
    if (str.length() == 3)*/
        str.append("开始时间：");
    //开始时间
    QString startime;
    startime = billings.at(maxi).start.toString("yyyy-MM-dd hh:mm:ss");
    str.append(startime);
    str.append(" 结束时间：");
    //结束时间
    QString Duration;
    Duration = billings.at(maxi).duration.toString("yyyy-MM-dd hh:mm:ss");
    str.append(Duration);
    str.append(" 费用：");
    //费用
    str.append(QString::number(billings.at(maxi).costs));
    str.append("元");
    data.append(str);
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

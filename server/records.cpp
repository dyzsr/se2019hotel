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
    QVector<Billing> billing = pipe->getBillings(roomId);
    QVector<QString> data;
    QString str;
    QDateTime max = QDateTime::fromString("00-00-00 00:00:00");
    int maxi = 0;
    uint stime;
    uint etime;
    int tRet;
    for (int i=0; i<billing.length(); ++i)
    {
        stime = max.toTime_t();
        etime = billing.at(i).start.toTime_t();
        tRet = stime - etime;
        if(tRet < 0)
        {
            max = billing.at(i).start;
            maxi = i;
        }
    }
    str.append("开始时间：");
    //开始时间
    QString startime;
    startime = billing.at(maxi).start.toString("yyyy-MM-dd hh:mm:ss");
    str.append(startime);
    str.append(" 结束时间：");
    //结束时间
    QString Duration;
    Duration = billing.at(maxi).duration.toString("yyyy-MM-dd hh:mm:ss");
    str.append(Duration);
    str.append(" 费用：");
    //费用
    str.append(QString::number(billing.at(maxi).costs));
    str.append("元");
    data.append(str);
    return data;
}

QVector<QString> Records::getReportForm(QDateTime start , QDateTime end)
{
  // TODO
    QVector<Billing> billingss = pipe->getAllBillings();
    QVector<Room> rooms =  pipe->getRooms();
    QVector<QString> data;
    QString str;
    uint stim;
    uint etim;
    uint etim2;
    int tRet,tRet2;
    int *op;//开关次数
    int *temp;//调温
    int *speed;//调风
    int *record;//详单
    int *duratio;//时长
    int *fee;//总消费
    op=(int*)calloc(rooms.size(),sizeof(int));
    temp=(int*)calloc(rooms.size(),sizeof(int));
    speed=(int*)calloc(rooms.size(),sizeof(int));
    record=(int*)calloc(rooms.size(),sizeof(int));
    duratio=(int*)calloc(rooms.size(),sizeof(int));
    fee=(int*)calloc(rooms.size(),sizeof(int));
    for(int j=0; j<billingss.length(); ++j)
    {
        int dus;//持续时间
        stim = start.toTime_t();
        etim = billingss.at(j).start.toTime_t();
        etim2 = end.toTime_t();
        tRet = stim - etim;
        tRet2 = etim2 - etim;
        if(tRet < 0 && tRet2 > 0)
        {
            dus = billingss.at(j).start.secsTo(billingss.at(j).duration);
            record[billingss.at(j).roomId]++;
            if(billingss.at(j).action == 0 || billingss.at(j).action == 1)
            {
                op[billingss.at(j).roomId]++;
            }
            else if(billingss.at(j).action == 2)
            {
                temp[billingss.at(j).roomId]++;
            }
            else if(billingss.at(j).action == 3)
            {
                speed[billingss.at(j).roomId]++;
            }
            duratio[billingss.at(j).roomId] = duratio[billingss.at(j).roomId] + dus;
            fee[billingss.at(j).roomId] = fee[billingss.at(j).roomId] + billingss.at(j).costs;
         }
        }
    for (int i=0; i<rooms.size(); ++i)
    {
        str.clear();
        str.append(QString::number(i+1));
        str.append(".  ");
            str.append("房间id：");
            //房间编号
            str.append(QString::number(i));
            str.append(" 使用时长：");
            //空调运行时间
            int du;
            du = duratio[i];
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
            str.append(" 费用：");
            //总费用
            str.append(QString::number(fee[i]));
            str.append("元");
            str.append(" 调风次数：");
            //调风次数
            str.append(QString::number(speed[i]));
            str.append(" 开关次数：");
            //空调开关次数
            str.append(QString::number(op[i]));
            str.append(" 调温次数：");
            //调温次数
            str.append(QString::number(temp[i]));
            str.append(" 详单数：");
            //详单数
            str.append(QString::number(record[i]));
            data.append(str);
    }
        return data;
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

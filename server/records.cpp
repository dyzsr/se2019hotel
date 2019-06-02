#include "records.h"
#pragma execution_character_set("utf-8")

Records::Records(QObject *parent) :
    QObject(parent),
    pipe(Pipe::getInstance())
{
    Q_ASSERT(pipe != nullptr);
}

QVector<QString> Records::getDetailedBill(Room room)
{
    QVector<Billing> billings = pipe->getBillings(room.roomId);

    Billing b;
    b.roomId = 0;
    b.start = room.start.addDays(1);
    billings.append(b);

    QVector<QString> data;
    QString str;
    int count = 0;
    for (int i=0; i<billings.length(); ++i)
    {
        if (room.start < billings.at(i).start)
        {
            str.clear();
            //序号
            count++;
            str.append(QString::number(count));
            str.append(".  ");
            if (str.length() == 4)
                str.append("  ");
            //风速
            str.append("风速：");
            str.append(QString::number(billings.at(i).wdspd));
            //费率
            str.append("  费率：");
            str.append(QString::number(billings.at(i).rate, 'f', 2));
            //持续时间
            str.append("  时长：");
            int64_t du;
            du = billings.at(i).start.secsTo(billings.at(i).duration);
            int64_t h = du / 3600;
            int h1 = du % 3600;
            str.append(QString::number(h));
            str.append("时");
            int m = h1 / 60;
            int m1 = h1 % 60;
            str.append(QString::number(m));
            str.append("分");
            str.append(QString::number(m1));
            str.append("秒");
            //费用
            str.append("  费用：");
            str.append(QString::number(billings.at(i).costs, 'f', 2));
            str.append("元");
            data.append(str);
            //第二行
            str.clear();
            str.append("       起始温度：");
            str.append(QString::number(billings.at(i).startTemp));
            str.append("  最终温度：");
            str.append(QString::number(billings.at(i).endTemp));
            data.append(str);
        }
    }
    return data;
}

QVector<QString> Records::getSimpleBills(int roomId)
{
    QVector<Billing> billing = pipe->getBillings(roomId);
    QVector<QString> data;
    QVector<Room> roomss = pipe->getRooms();
    QString str;
    int64_t tRet;
    if(!billing.empty())
    {
        int64_t stimm;
        int64_t etimm;
        int64_t htimm;
        int64_t n;
        double cost=0;
        stimm = roomss.at(roomId).start.toTime_t();
        htimm = roomss.at(roomId).duration.toTime_t();
        n = htimm;
        for(int j=0; j<billing.length(); ++j)
        {
            etimm = billing.at(j).start.toTime_t();
            tRet = etimm - stimm;
            if(tRet <= n && tRet >= 0)
            {
                cost = cost + billing.at(j).costs;
            }
        }
        str.append("开始时间：");
        //开始时间
        QString startime;
        startime = roomss.at(roomId).start.toString("yyyy-MM-dd hh:mm:ss");
        str.append(startime);
        str.append(" 费用：");
        //费用
        str.append(QString::number(cost));
        str.append("元");
        data.append(str);
     }
    return data;
}

QVector<QString> Records::getReportForm(QDateTime start , QDateTime end)
{
  // TODO
    QVector<QString> data;
    QString str;
    int64_t stim;
    int64_t etim;
    int64_t etim2;
    int64_t tRet,tRet2;

    op=(int*)calloc(rooms.size(),sizeof(int));
    temp=(int*)calloc(rooms.size(),sizeof(int));
    speed=(int*)calloc(rooms.size(),sizeof(int));
    record=(int*)calloc(rooms.size(),sizeof(int));
    duratio=(int64_t*)calloc(rooms.size(),sizeof(int));
    fee=(double*)calloc(rooms.size(),sizeof(int));
    for(int j=0; j<billingss.length(); ++j)
    {
        int64_t dus;//持续时间
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
    str.append("开始日期：");
    QString startime;
    startime = start.toString("yyyy-MM-dd hh:mm:ss");
    str.append(startime);
    str.append("--->截止日期：");
    QString endtime;
    endtime = end.toString("yyyy-MM-dd hh:mm:ss");
    str.append(endtime);
    data.append(str);
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
            int64_t du;
            du = duratio[i];
            int64_t h = du / 3600;
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
            str.append(QString::number(fee[i], 'f', 2));
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
    data.append("----------------------------------------------------------------------------------------------------------------------");
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
        int64_t h, m;
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

void Records::getInfoOnce()
{
    billingss = pipe->getAllBillings();
    rooms = pipe->getRooms();
}

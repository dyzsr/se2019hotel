#include "reportformwindow.h"
#include "ui_reportformwindow.h"
#include "../share/pipe.h"
#include <QFile>
#include <QDebug>
#include "records.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

#pragma execution_character_set("utf-8")

ReportFormWindow::ReportFormWindow(QWidget *parent) :
  QWidget(parent, Qt::Window),
  records(parent),
    ui(new Ui::ReportFormWindow)
{
    ui->setupUi(this);
    ui->display->setViewMode(QListView::ListMode);
    ui->title->setAlignment(Qt::AlignCenter);
    ui->title->setText("查看报表");
    QString str;
    str.clear();
    str.append("开始日期：");
    ui->sstart->setText(str);
    str.clear();
    str.append("结束日期：");
    ui->eend->setText(str);
    ui->s_dateTime->setCalendarPopup(true);
    ui->e_dateTime->setCalendarPopup(true);

    ui->s_dateTime->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->e_dateTime->setDateTime(QDateTime::currentDateTime());
}


ReportFormWindow::~ReportFormWindow()
{
    delete ui;
}

void ReportFormWindow::showReportForm(QVector<QString> data)
{
    //To Do
    printDat = data;
    ui->display->clear();
    int i, num = data.length();
    for (i=0; i<num; ++i)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(data.at(i));
        ui->display->addItem(item);
    }
    this->show();
}

void ReportFormWindow::on_go_back_clicked()
{
        this->hide();
}



void ReportFormWindow::on_print_clicked()
{
    QString dir = "../records/ReportForm/";
    QString fileName = "报表_";
    //fileName.append(printRoo.usrId);
    if(rrr == 0)
    fileName.append("日.txt");
    else if(rrr == 1)
    fileName.append("周.txt");
    else if(rrr == 2)
    fileName.append("月.txt");
    QFile f(dir+fileName);
    if (!f.open(QIODevice::WriteOnly))
        qDebug() << "打开报表失败";
    else
    {
        qDebug() << "打开报表成功";
        QTextStream s(&f);
        int i;
        //第一行
        for (i=0; i<24; ++i)
            s << " ";
        if(rrr == 0)
        s << QString("日报表") << "\n";
        else if(rrr == 1)
        s << QString("周报表") << "\n";
        else if(rrr == 2)
        s << QString("月报表") << "\n";
        //第二行
        for (i=0; i<54; ++i)
            s << "-";
            s << "\n";
        //第三行
        /*QString l(ui->sim_user->text());
        for (i=0; i<25-l.length(); ++i)
            l.append(" ");
        l.append(ui->sim_room->text());
        for (i=0; i<40-l.length(); ++i)
            l.append(" ");
        l.append(ui->sim_times->text());*/
        s << "\n";
        //第四行
        for (i=0; i<54; ++i)
            s << "-";
        s << "\n";
        //List
        for (int i=0; i<printDat.length(); ++i)
        {
            s << printDat.at(i) << "\n";
        }
    }
    f.close();
}

void ReportFormWindow::Updatereport(QVector<QString> dat)
{

    printDat = dat;
    int k, num = dat.length();
    for (k=0; k<num; ++k)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(dat.at(k));
        ui->display->addItem(item);
    }
}

void ReportFormWindow::on_OK_clicked()
{
    records.getInfoOnce();
    QDateTime startt = ui->s_dateTime->dateTime();
    QDateTime endt = ui->e_dateTime->dateTime();
    ui->display->clear();
    rrr = ui->type->currentIndex();
    if(rrr == 0)
    {
        QDateTime v;
        v = startt;
        int64_t stime;
        int64_t etime;
        int64_t tRet;
        stime = v.addDays(1).toTime_t();
        etime = endt.toTime_t();
        tRet = stime - etime;
        if(tRet < 0)
        {Updatereport(records.getReportForm(v,v.addDays(1)));}
        else {

        }
        while(tRet < 0)
        {
            v = v.addDays(1);
            stime = v.addDays(1).toTime_t();
            etime = endt.toTime_t();
            tRet = stime - etime;
           if(tRet > 0)
            {
                break;
            }
            Updatereport(records.getReportForm(v,v.addDays(1)));
        }
        stime = v.toTime_t();
        etime = endt.toTime_t();
        tRet = stime - etime;
        if(tRet < 0)
        {
            Updatereport(records.getReportForm(v,endt));
        }
    }
    else if(rrr == 1)
    {
        QDateTime v1;
        v1 = startt;
        int64_t stime1;
        int64_t etime1;
        QString StrCurrentTime = startt.toString("yyyy-MM-dd hh:mm:ss ddd");
        QString x =  StrCurrentTime.mid(StrCurrentTime.length()-1,1);
        int strDayOfWeek = 0;
        int64_t tRet1;
        if(x == "一")
        {
            strDayOfWeek = 7;
        }
        else if(x == "二")
        {
            strDayOfWeek = 6;
        }
        else if(x == "三")
        {
            strDayOfWeek = 5;
        }
        else if(x == "四")
        {
            strDayOfWeek = 4;
        }
        else if(x == "五")
        {
            strDayOfWeek = 3;
        }
        else if(x == "六")
        {
            strDayOfWeek = 2;
        }
        else if(x == "日")
        {
            strDayOfWeek = 1;
        }
        stime1 = v1.addDays(strDayOfWeek).toTime_t();
        etime1 = endt.toTime_t();
        tRet1 = stime1 - etime1;
        if(tRet1 < 0)
        {Updatereport(records.getReportForm(v1,v1.addDays(strDayOfWeek)));
        v1 = v1.addDays(strDayOfWeek);
        }
        else {

        }
        while(tRet1 < 0)
        {
            stime1 = v1.addDays(7).toTime_t();
            etime1 = endt.toTime_t();
            tRet1 = stime1 - etime1;
           if(tRet1 > 0)
            {
                break;
            }
            Updatereport(records.getReportForm(v1,v1.addDays(7)));
            v1 = v1.addDays(7);
        }
        stime1 = v1.toTime_t();
        etime1 = endt.toTime_t();
        tRet1 = stime1 - etime1;
        if(tRet1 < 0)
        {
            Updatereport(records.getReportForm(v1,endt));
        }
    }
    else if(rrr == 2)
    {
        QDateTime v2;
        v2 = startt;
        int64_t stime2;
        int64_t etime2;
        QString StrCurrentTime = startt.addMonths(1).toString("yyyy-MM-dd hh:mm:ss");
        QString x =  StrCurrentTime.mid(5,2);
        QString y;
        if(x == "12")
        {
            y =  startt.addYears(1).toString("yyyy-MM-dd hh:mm:ss").mid(0,5);
            y.append("01");
        }
        else
        y =  StrCurrentTime.mid(0,7);
        int64_t tRet2;
        y.append("-01 00:00:00");
        QDateTime time1;
        time1 = QDateTime::fromString(y, "yyyy-MM-dd hh:mm:ss");
        stime2 = time1.toTime_t();
        etime2 = endt.toTime_t();
        tRet2 = stime2 - etime2;
        if(tRet2 < 0)
        {Updatereport(records.getReportForm(v2,time1));
        v2 = time1;
        }
        else {

        }
        while(tRet2 < 0)
        {
            stime2 = v2.addMonths(1).toTime_t();
            etime2 = endt.toTime_t();
            tRet2 = stime2 - etime2;
           if(tRet2 > 0)
            {
                break;
            }
            Updatereport(records.getReportForm(v2,v2.addMonths(1)));
            v2 = v2.addMonths(1);
        }
        stime2 = v2.toTime_t();
        etime2 = endt.toTime_t();
        tRet2 = stime2 - etime2;
        if(tRet2 < 0)
        {
            Updatereport(records.getReportForm(v2,endt));
        }
    }
}

QT_CHARTS_USE_NAMESPACE
void ReportFormWindow::on_viewreport_clicked()
{
    //QApplication a();
    QObject *parent = Q_NULLPTR;
    QBarSeries(Q_NULLPTR);
    // 定义柱状条
    QBarSet *set0 = new QBarSet("1");
    QBarSet *set1 = new QBarSet("2");
    QBarSet *set2 = new QBarSet("3");
    QBarSet *set3 = new QBarSet("4");
    // 柱状条赋值
    *set0 << records.op[0] << records.speed[0] << records.temp[0] << records.record[0];
    *set1 << records.op[1] << records.speed[1] << records.temp[1] << records.record[1];
    *set2 << records.op[2] << records.speed[2] << records.temp[2] << records.record[2];
    *set3 << records.op[3] << records.speed[3] << records.temp[3] << records.record[3];

    // 设置柱状集
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    // 添加图表
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Requested Charts"); // 设置图表的标题
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    // 为图表添加坐标轴内容
    QStringList categories;
    categories <<  "开关次数" << "调风时间" << "调温次数" << "详单数量" ;
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // 将图表添加到显示窗口中
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    window.setCentralWidget(chartView);
    window.resize(420, 300);
    window.show();
}

void ReportFormWindow::on_viewfee_clicked()
{
    QPieSeries *series = new QPieSeries();
    series->append("Jane", 1);
    series->append("Joe", 2);
    series->append("Andy", 3);
    series->append("Barbara", 4);
    series->append("Axel", 5);

    QPieSlice *slice = series->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple piechart example");
    chart->legend()->hide();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();

}

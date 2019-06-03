#include "reportformwindow.h"
#include "ui_reportformwindow.h"
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

ReportFormWindow::ReportFormWindow(QWidget *parent, Records *rec) :
  QWidget(parent, Qt::Window),
  records(rec),
  ui(new Ui::ReportFormWindow),
  window(this)
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
}


ReportFormWindow::~ReportFormWindow()
{
    delete ui;
}

void ReportFormWindow::showReportForm()
{
    ui->s_dateTime->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->e_dateTime->setDateTime(QDateTime::currentDateTime());
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
    QDateTime startt = ui->s_dateTime->dateTime();
    QDateTime endt = ui->e_dateTime->dateTime();
    ui->display->clear();
    rrr = ui->type->currentIndex();
    if(rrr == 0)
    {
      qDebug() << "day report";
        QDateTime v;
        v = startt;
        int64_t stime;
        int64_t etime;
        int64_t tRet;
        stime = v.addDays(1).toTime_t();
        etime = endt.toTime_t();
        tRet = stime - etime;
        if(tRet < 0)
        {
          Updatereport(records->getReportForm(v,v.addDays(1)));
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
            Updatereport(records->getReportForm(v,v.addDays(1)));
        }
        stime = v.toTime_t();
        etime = endt.toTime_t();
        tRet = stime - etime;
        if(tRet < 0)
        {
            Updatereport(records->getReportForm(v,endt));
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
        {Updatereport(records->getReportForm(v1,v1.addDays(strDayOfWeek)));
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
            Updatereport(records->getReportForm(v1,v1.addDays(7)));
            v1 = v1.addDays(7);
        }
        stime1 = v1.toTime_t();
        etime1 = endt.toTime_t();
        tRet1 = stime1 - etime1;
        if(tRet1 < 0)
        {
            Updatereport(records->getReportForm(v1,endt));
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
        {Updatereport(records->getReportForm(v2,time1));
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
            Updatereport(records->getReportForm(v2,v2.addMonths(1)));
            v2 = v2.addMonths(1);
        }
        stime2 = v2.toTime_t();
        etime2 = endt.toTime_t();
        tRet2 = stime2 - etime2;
        if(tRet2 < 0)
        {
            Updatereport(records->getReportForm(v2,endt));
        }
    }
}

QT_CHARTS_USE_NAMESPACE
void ReportFormWindow::on_viewreport_clicked()
{
    records->updateRecord(ui->s_dateTime->dateTime(), ui->e_dateTime->dateTime());

    // 设置柱状集
    QBarSeries *series = new QBarSeries(&window);

    for (int i = 0; i < records->nr_rooms; i++) {
      // 定义柱状条
      QBarSet *set = new QBarSet("room " + QString::number(i));
      // 柱状条赋值
      *set << records->op[i] << records->speed[i] << records->temp[i] << records->record[i];
      series->append(set);
    }

    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    // 添加图表
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("操作统计"); // 设置图表的标题
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
    records->updateRecord(ui->s_dateTime->dateTime(), ui->e_dateTime->dateTime());

    QPieSeries *series = new QPieSeries(this->parent());
    series->setHoleSize(0.35);

    for (int i = 0; i < records->nr_rooms; i++) {
      series->append("room" + QString::number(i), records->fee[i]);
    }

    QChartView *chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setTitle("费用统计");
    chartView->chart()->addSeries(series);
    chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
    chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView->chart()->legend()->setFont(QFont("Arial", 7));

    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();
}

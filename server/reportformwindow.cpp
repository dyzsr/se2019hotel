#include "reportformwindow.h"
#include "ui_reportformwindow.h"
#include "../share/pipe.h"
#include <QFile>
#include <QDebug>
#include "records.h"
#pragma execution_character_set("utf-8")

ReportFormWindow::ReportFormWindow(QWidget *parent) :
  QWidget(parent),
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
    ui->display->clear();
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
    rrr = ui->type->currentIndex();
    Updatereport(records.getReportForm(startt,endt));
}

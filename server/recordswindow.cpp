#include "recordswindow.h"
#include "ui_recordswindow.h"
#include <QFile>
#include <QDebug>
#pragma execution_character_set("utf-8");
RecordsWindow::RecordsWindow(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::RecordsWindow)
{
    ui->setupUi(this);
    ui->listWidget_db->setViewMode(QListView::ListMode);
    ui->lb_recordTitle->setAlignment(Qt::AlignCenter);
    ui->theColorBar->setStyleSheet("background-color:yellow;");
}

RecordsWindow::~RecordsWindow()
{
    delete ui;
}

void RecordsWindow::showDetailedBill(Room room, QVector<QString> data)
{
    printRoom = room;
    printData = data;
    //配置其他ui控件
    QString str;
    str.append("用户：");
    str.append(room.usrId);
    ui->lb_user_db->setText(str);
    str.clear();
    str.append("房间：");
    str.append(QString::number(room.roomId));
    ui->lb_room_db->setText(str);
    str.clear();
    str.append("请求次数：");
    str.append(QString::number(data.length()));
    ui->lb_times_db->setText(str);
    //添加List内容
    ui->listWidget_db->clear();
    int i, num = data.length();
    for (i=0; i<num; ++i)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(data.at(i));
        ui->listWidget_db->addItem(item);
    }
    //显示
    this->show();
}

void RecordsWindow::on_bt_return_clicked()
{
    this->hide();
}

void RecordsWindow::printDetailedBill()
{
    QString dir = "../records/DetailedBills/";
    QString fileName = "详单_";
    fileName.append(printRoom.usrId);
    fileName.append(".txt");
    QFile f(dir+fileName);
    if (!f.open(QIODevice::WriteOnly))
        qDebug() << "打开详单失败";
    else
    {
        qDebug() << "打开详单成功";
        QTextStream s(&f);
        int i;
        //第一行
        for (i=0; i<24; ++i)
            s << " ";
        s << QString("详单") << "\n";
        //第二行
        for (i=0; i<55; ++i)
            s << "-";
        s << "\n";
        //第三行
        QString line3(ui->lb_user_db->text());
        for (i=0; i<25-line3.length(); ++i)
            line3.append(" ");
        line3.append(ui->lb_room_db->text());
        for (i=0; i<40-line3.length(); ++i)
            line3.append(" ");
        line3.append(ui->lb_times_db->text());
        s << line3 << "\n";
        //第四行
        for (i=0; i<55; ++i)
            s << "-";
        s << "\n";
        //List
        for (int i=0; i<printData.length(); ++i)
        {
            s << printData.at(i) << "\n";
        }
    }
    f.close();
}

void RecordsWindow::on_bt_print_clicked()
{
    printDetailedBill();
}

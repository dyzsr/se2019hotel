#include "simplebillwindow.h"
#include "ui_simplebillwindow.h"
#include <QFile>
#include <QDebug>
#pragma execution_character_set("utf-8");

SimpleBillWindow::SimpleBillWindow(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::SimpleBillWindow)
{
    ui->setupUi(this);
    ui->sim_listWidget->setViewMode(QListView::ListMode);
    ui->sim_label->setAlignment(Qt::AlignCenter);
    ui->sim_label->setText("账单");
    ui->sim_c_widget->setStyleSheet("background-color:yellow;");
}

SimpleBillWindow::~SimpleBillWindow()
{
    delete ui;
}

void SimpleBillWindow::showSimpleBill(Room room , QVector<QString> data)
{
    printRooms = room;
    printDatas = data;
    //To be continued
    QString str;
    str.append("用户:");
    str.append(room.usrId);
    ui->sim_user->setText(str);
    str.clear();
    str.append("房间:");
    str.append(QString::number(room.roomId));
    ui->sim_room->setText(str);
    str.clear();
    str.append("请求次数:");
    str.append(QString::number(data.length()));
    ui->sim_times->setText(str);
    //list
    ui->sim_listWidget->clear();
    int i, num = data.length();
    for (i=0; i<num; ++i)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(data.at(i));
        ui->sim_listWidget->addItem(item);
    }
    this->show();
}

void SimpleBillWindow::printSimpleBill()
{
    QString dir = "../records/SimpleBills/";
    QString fileName = "账单_";
    fileName.append(printRooms.usrId);
    fileName.append(".txt");
    QFile f(dir+fileName);
    if (!f.open(QIODevice::WriteOnly))
        qDebug() << "打开账单失败";
    else
    {
        qDebug() << "打开账单成功";
        QTextStream s(&f);
        int i;
        //第一行
        for (i=0; i<24; ++i)
            s << " ";
        s << QString("账单") << "\n";
        //第二行
        for (i=0; i<54; ++i)
            s << "-";
            s << "\n";
        //第三行
        QString l(ui->sim_user->text());
        for (i=0; i<25-l.length(); ++i)
            l.append(" ");
        l.append(ui->sim_room->text());
        for (i=0; i<40-l.length(); ++i)
            l.append(" ");
        l.append(ui->sim_times->text());
        s << l << "\n";
        //第四行
        for (i=0; i<54; ++i)
            s << "-";
        s << "\n";
        //List
        for (int i=0; i<printDatas.length(); ++i)
        {
            s << printDatas.at(i) << "\n";
        }
    }
    f.close();
}

void SimpleBillWindow::on_pushButton_print_clicked()
{
    printSimpleBill();
}

void SimpleBillWindow::on_pushButton_back_clicked()
{
    this->hide();
}

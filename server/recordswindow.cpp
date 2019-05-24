#include "recordswindow.h"
#include "ui_recordswindow.h"

RecordsWindow::RecordsWindow(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::RecordsWindow)
{
    ui->setupUi(this);
    ui->listWidget_db->setViewMode(QListView::ListMode);
    ui->lb_recordTitle->setAlignment(Qt::AlignCenter);
}

RecordsWindow::~RecordsWindow()
{
    delete ui;
}

void RecordsWindow::showSimpleBill(QVector<QString> data)
{
    printData = data;
    hideOptionalItems();
    //To be continued

    this->show();
}

void RecordsWindow::showDetailedBill(Room room, QVector<QString> data)
{
    recordTitle = "详单";
    ui->lb_recordTitle->setText(recordTitle);
    printData = data;
    hideOptionalItems();
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
    ui->widget_detailedBill->show();
    ui->listWidget_db->show();
}

void RecordsWindow::showReportForm(QVector<QString> data)
{
    printData = data;
    hideOptionalItems();
    //To be continued

    this->show();
}

void RecordsWindow::hideOptionalItems()
{
    ui->widget_detailedBill->hide();
}

void RecordsWindow::on_bt_return_clicked()
{
    this->hide();
}

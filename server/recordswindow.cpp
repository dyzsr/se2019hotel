#include "recordswindow.h"
#include "ui_recordswindow.h"

RecordsWindow::RecordsWindow(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::RecordsWindow)
{
    ui->setupUi(this);
    ui->listWidget_db->setViewMode(QListView::ListMode);
    ui->lb_recordTitle->setAlignment(Qt::AlignCenter);
    ui->lb_recordTitle->setText("详单");
}

RecordsWindow::~RecordsWindow()
{
    delete ui;
}

void RecordsWindow::showDetailedBill(Room room, QVector<QString> data)
{
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

#include "managerwindow.h"
#include "ui_managerwindow.h"

ManagerWindow::ManagerWindow(QWidget *parent) :
  QWidget(parent, Qt::Window),
  ui(new Ui::ManagerWindow)
{
  ui->setupUi(this);
  init();
}

ManagerWindow::~ManagerWindow()
{
  delete ui;
}

void ManagerWindow::init()
{
  ui->stackedWidget->setCurrentIndex(0);
  ui->lb_verdict->hide();
  ui->lb_adminpswd->setText(QString());
}

void ManagerWindow::on_bt_signIn_clicked()
{
  QString name = ui->edit_adminname->text();
  QString pswd = ui->edit_adminpswd->text();
  bool success = emit sgn_signIn(name, pswd);
  if (success) {
    emit sgn_askManagerFunc();
    ui->lb_verdict->hide();
  } else {
    ui->lb_verdict->show();
  }
}

void ManagerWindow::on_bt_back_clicked()
{
  bool success = emit sgn_signOut();
  if (success) {
    ui->stackedWidget->setCurrentIndex(0);
  }
}

void ManagerWindow::showManagerFunc(QVector<QString> data)
{
    //绘制ui
    ui->lw_roomState->clear();
    int i, num = data.length();
    for (i=0; i<num; ++i)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(data.at(i));
        ui->lw_roomState->addItem(item);
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void ManagerWindow::on_bt_refresh_clicked()
{
    emit sgn_askManagerFunc();
}

void ManagerWindow::on_bt_checkReport_clicked()
{
    emit sgn_askReportForm_clicked();
}

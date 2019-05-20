#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  ui->lb_verdict->hide();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::refresh(Room room)
{
  ui->disp_temp->setNum(room.temp);
  ui->disp_settemp->setNum(room.settemp);
  ui->disp_wdspd->setNum(room.wdspd);
  ui->disp_setwdspd->setNum(room.setwdspd);
}

void MainWindow::showBilling(QVector<Billing> billings)
{

}

void MainWindow::showDetails(QVector<Billing> billings)
{

}

void MainWindow::on_bt_ok_clicked()
{
  bool success = emit sgn_signIn(ui->edit_username->text(),
                                 ui->edit_password->text());
  if (success) {
    ui->lb_verdict->hide();
    ui->stackedWidget->setCurrentIndex(1);
  } else {
    ui->lb_verdict->show();
  }
}

void MainWindow::on_bt_checkIn_clicked()
{

}

void MainWindow::on_bt_checkOut_clicked()
{

}

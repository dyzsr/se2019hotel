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
  ui->lcd_temp->display(room.temp);
  ui->lcd_settemp->display(room.settemp);
  ui->lcd_wdspd->display(room.wdspd);
  ui->lcd_setwdspd->display(room.setwdspd);
  ui->lb_fee->setNum(room.cost);
  ui->lb_feeRate->setNum(room.pwr);
}

void MainWindow::on_bt_signIn_clicked()
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

void MainWindow::on_bt_signOut_clicked()
{
  bool success = emit sgn_signOut();
  if (success) {
    ui->stackedWidget->setCurrentIndex(0);
    ui->edit_password->setText("");
  }
}

void MainWindow::on_bt_checkIn_clicked()
{

}

void MainWindow::on_bt_checkOut_clicked()
{

}

void MainWindow::on_bt_tempUp_clicked()
{
  int temp = ui->sb_settemp->value();
  ui->sb_settemp->setValue(temp + 1);
}

void MainWindow::on_bt_tempDown_clicked()
{
  int temp = ui->sb_settemp->value();
  ui->sb_settemp->setValue(temp - 1);
}

void MainWindow::on_sb_settemp_valueChanged(int temp)
{
  emit sgn_setTemp(temp);
}

void MainWindow::on_bt_wdspdUp_clicked()
{
  int wdspd = ui->sb_setwdspd->value();
  ui->sb_setwdspd->setValue(wdspd + 1);
}

void MainWindow::on_bt_wdspdDown_clicked()
{
  int wdspd = ui->sb_setwdspd->value();
  ui->sb_setwdspd->setValue(wdspd - 1);
}

void MainWindow::on_sb_setwdspd_valueChanged(int wdspd)
{
  emit sgn_setWdspd(wdspd);
}

void MainWindow::on_bt_state0_clicked(bool checked)
{
  if (!checked) {
    ui->bt_state0->setChecked(true);
    ui->bt_state1->setChecked(false);
    ui->bt_state2->setChecked(false);
    emit sgn_setState(0);
  }
}

void MainWindow::on_bt_state1_clicked(bool checked)
{
  if (!checked) {
    ui->bt_state0->setChecked(false);
    ui->bt_state1->setChecked(true);
    ui->bt_state2->setChecked(false);
    emit sgn_setState(1);
  }
}

void MainWindow::on_bt_state2_clicked(bool checked)
{
  if (!checked) {
    ui->bt_state0->setChecked(false);
    ui->bt_state1->setChecked(false);
    ui->bt_state2->setChecked(true);
    emit sgn_setState(2);
  }
}

void MainWindow::on_bt_backToDashboard_clicked()
{
  ui->stackedWidget->setCurrentIndex(1);
}

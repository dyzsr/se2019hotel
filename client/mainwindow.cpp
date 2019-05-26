#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  ui->lb_verdict->hide();

  ui->sb_settemp->setValue(25);
  ui->sb_setwdspd->setValue(1);
  ui->bt_state1->setDown(false);
  ui->bt_state0->setDown(true);
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

  if(room.mode==false)
    ui->lb_mode->setText("制冷");
  else
    ui->lb_mode->setText("制热");

  if(room.state==0)
      ui->lb_state->setText("关机");
  else if(room.state==1)
      ui->lb_state->setText("运行");
  else if(room.state==2)
      ui->lb_state->setText("待机");
  else
      ui->lb_state->setText("调度");

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

void MainWindow::on_bt_getIn_clicked()
{
  emit sgn_getIn(ui->bt_state1->isDown(),
                 ui->sb_settemp->value(),
                 ui->sb_setwdspd->value());
}

void MainWindow::on_bt_getOut_clicked()
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
    ui->bt_state0->setDown(true);
    ui->bt_state1->setDown(false);
    // 待机 == 2
    emit sgn_setState(2);
  }
}

void MainWindow::on_bt_state1_clicked(bool checked)
{
  if (!checked) {
    ui->bt_state0->setDown(false);
    ui->bt_state1->setDown(true);
    // 开机 == 1
    emit sgn_setState(1);
  }
}

void MainWindow::on_bt_backToDashboard_clicked()
{
  ui->stackedWidget->setCurrentIndex(1);
}
